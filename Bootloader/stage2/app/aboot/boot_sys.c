/*
 * Copyright (C) 2024 ravynOS Authors
 *
 * Boot from RAW ELF files from /boot partition
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 */

#include <app.h>
#include <debug.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lib/boot_sys.h>
#include <lib/elf_boot.h>
#include <dev/flash.h>
#include <dev/fbcon.h>
#include <partition_parser.h>
#include <mmc.h>
#include <target.h>
#include <arch/ops.h>

#include "boot_sys.h"
#include "elf_boot.h"

/* Buffer for reading BOOT.SYS file */
static char boot_sys_buf[BOOT_SYS_MAX_FILE_SIZE];

/* Scratch buffer for loading kernel */
static unsigned char *elf_load_buf;

/* Current framebuffer info */
static struct boot_sys_fb_info current_fb;

/* Default boot options */
static struct boot_sys_options default_opts = {
    .keep_fb = true,
    .verbose = true,
    .use_tags = true,
};

/*
 * Trim whitespace from both ends of a string
 */
static char *trim(char *str)
{
    char *end;
    
    if (!str)
        return str;
    
    /* Trim leading whitespace */
    while (*str == ' ' || *str == '\t' || *str == '\r' || *str == '\n')
        str++;
    
    if (*str == 0)
        return str;
    
    /* Trim trailing whitespace */
    end = str + strlen(str) - 1;
    while (end > str && (*end == ' ' || *end == '\t' || *end == '\r' || *end == '\n'))
        end--;
    
    *(end + 1) = 0;
    
    return str;
}

/*
 * Parse a single line from BOOT.SYS
 * Returns: 0 on success, -1 on ignore, positive on key match
 */
static int parse_boot_sys_line(char *line, struct boot_sys_config *config)
{
    char *key;
    char *value;
    char *equals;
    
    /* Skip empty lines */
    if (!line || *line == '\0' || *line == '#')
        return -1;
    
    /* Find the equals sign */
    equals = strchr(line, '=');
    if (!equals)
        return -1;
    
    *equals = '\0';
    key = trim(line);
    value = trim(equals + 1);
    
    if (strlen(key) == 0 || strlen(value) == 0)
        return -1;
    
    if (strcmp(key, "KERNEL") == 0) {
        strncpy(config->kernel_path, value, BOOT_SYS_KERNEL_PATH_LEN - 1);
        config->kernel_path[BOOT_SYS_KERNEL_PATH_LEN - 1] = '\0';
        return 1;
    }
    
    if (strcmp(key, "CMDLINE") == 0 || strcmp(key, "APPEND") == 0) {
        strncpy(config->cmdline, value, BOOT_SYS_CMDLINE_LEN - 1);
        config->cmdline[BOOT_SYS_CMDLINE_LEN - 1] = '\0';
        return 1;
    }
    
    /* Unknown key, ignore */
    return -1;
}

/*
 * Read file from partition
 */
static int read_file_from_partition(const char *partition, const char *filename,
                                    void *buf, size_t bufsize, size_t *read_size)
{
    int ptn_index;
    unsigned long long ptn_offset;
    unsigned long long ptn_size;
    struct mmc_device *mmc;
    size_t remaining;
    size_t total_read = 0;
    char *data = (char *)buf;
    char *file_start = NULL;
    char *file_end = NULL;
    size_t file_size = 0;
    
    /* Get partition info */
    ptn_index = partition_get_index(partition);
    if (ptn_index == INVALID_PTN) {
        dprintf(CRITICAL, "boot_sys: Partition '%s' not found\n", partition);
        return -1;
    }
    
    ptn_offset = partition_get_offset(ptn_index);
    ptn_size = partition_get_size(ptn_index);
    
    if (ptn_offset == 0 || ptn_size == 0) {
        dprintf(CRITICAL, "boot_sys: Invalid partition '%s'\n", partition);
        return -1;
    }
    
    mmc = (struct mmc_device *)target_get_sd_mmc();
    if (!mmc) {
        dprintf(CRITICAL, "boot_sys: Cannot get MMC device\n");
        return -1;
    }
    
    /* Read partition into buffer */
    /* Read up to bufsize or partition size */
    size_t to_read = (bufsize < ptn_size) ? bufsize : (size_t)ptn_size;
    
    if (mmc->read(ptn_offset, (uint32_t *)buf, to_read / 512 + ((to_read % 512) ? 1 : 0))) {
        dprintf(CRITICAL, "boot_sys: Failed to read partition '%s'\n", partition);
        return -1;
    }
    
    /* Search for filename in data */
    file_start = strstr(data, filename);
    if (!file_start) {
        dprintf(CRITICAL, "boot_sys: File '%s' not found in partition '%s'\n", 
                filename, partition);
        return -1;
    }
    
    /* Skip past the filename */
    file_start += strlen(filename);
    
    /* Skip any delimiter after filename */
    while (*file_start == ' ' || *file_start == '\n' || *file_start == '\r')
        file_start++;
    
    /* Find end of file content */
    file_end = strchr(file_start, '\0');
    
    /* Trim to actual file size */
    file_size = file_end - file_start;
    
    /* Make sure we don't exceed buffer */
    if (file_size > bufsize) {
        file_size = bufsize;
    }
    
    if (read_size)
        *read_size = file_size;
    
    /* Copy file content to beginning of buffer */
    memmove(buf, file_start, file_size);
    
    dprintf(INFO, "boot_sys: Read %d bytes of '%s' from '%s'\n", 
            file_size, filename, partition);
    
    return 0;
}

/*
 * Read BOOT.SYS from a partition
 */
int boot_sys_read_config(const char *partition, struct boot_sys_config *config)
{
    char *lines[BOOT_SYS_MAX_LINES];
    int line_count = 0;
    int i;
    int parsed = 0;
    size_t bytes_read = 0;
    char *buf_ptr;
    char *line_start;
    size_t remaining;
    int ret;
    
    if (!partition || !config) {
        return -1;
    }
    
    /* Initialize config */
    memset(config, 0, sizeof(*config));
    config->valid = false;
    
    /* Read BOOT.SYS file from partition */
    ret = read_file_from_partition(partition, "BOOT.SYS", 
                                   boot_sys_buf, sizeof(boot_sys_buf), &bytes_read);
    if (ret != 0) {
        return ret;
    }
    
    /* Parse lines */
    buf_ptr = boot_sys_buf;
    remaining = bytes_read;
    
    while (remaining > 0 && line_count < BOOT_SYS_MAX_LINES) {
        line_start = buf_ptr;
        
        /* Find end of line */
        while (remaining > 0 && *buf_ptr != '\n' && *buf_ptr != '\0') {
            buf_ptr++;
            remaining--;
        }
        
        /* Null terminate at newline */
        if (*buf_ptr == '\n') {
            *buf_ptr = '\0';
            buf_ptr++;
            remaining--;
        }
        
        /* Skip carriage returns */
        if (line_start[0] == '\r' && line_start[1] == '\0')
            continue;
        
        /* Parse this line */
        if (parse_boot_sys_line(line_start, config) > 0) {
            parsed++;
        }
        
        line_count++;
    }
    
    if (parsed == 0) {
        dprintf(CRITICAL, "boot_sys: No valid KERNEL= or CMDLINE= found in BOOT.SYS\n");
        return BOOT_SYS_ERR_PARSE_ERROR;
    }
    
    /* Check if we have at least KERNEL */
    if (config->kernel_path[0] == '\0') {
        dprintf(CRITICAL, "boot_sys: No KERNEL= specified in BOOT.SYS\n");
        return BOOT_SYS_ERR_PARSE_ERROR;
    }
    
    config->valid = true;
    
    dprintf(INFO, "boot_sys: Parsed BOOT.SYS successfully\n");
    dprintf(INFO, "boot_sys: KERNEL=%s\n", config->kernel_path);
    dprintf(INFO, "boot_sys: CMDLINE=%s\n", config->cmdline);
    
    return 0;
}

/*
 * Get current framebuffer info
 */
int boot_sys_get_fb_info(struct boot_sys_fb_info *fb_info)
{
    if (!fbcon_display_on()) {
        /* No framebuffer available */
        return -1;
    }
    
    fb_info->width = get_fb_width();
    fb_info->height = get_fb_height();
    fb_info->stride = get_fb_stride();
    fb_info->bpp = get_fb_bpp();
    fb_info->format = get_fb_format();
    fb_info->base = (uint64_t)get_fb_base();
    fb_info->size = get_fb_size();
    
    dprintf(INFO, "boot_sys: FB info: %dx%d stride=%d bpp=%d base=0x%llx size=%d\n",
            fb_info->width, fb_info->height, fb_info->stride,
            fb_info->bpp, fb_info->base, fb_info->size);
    
    return 0;
}

/*
 * Boot from a raw ELF file
 */
int boot_sys_boot_elf(const char *elf_path, const char *cmdline,
                      struct boot_sys_options *opts,
                      struct boot_sys_fb_info *fb_info)
{
    struct elf_boot_info kernel_info;
    int ret;
    
    if (!elf_path || !cmdline) {
        return BOOT_SYS_ERR_BOOT_FAIL;
    }
    
    /* Use default options if none provided */
    if (!opts) {
        opts = &default_opts;
    }
    
    /* Copy framebuffer info for kernel */
    if (fb_info) {
        boot_sys_get_fb_info(fb_info);
    }
    
    if (opts->verbose) {
        dprintf(INFO, "boot_sys: Loading ELF from: %s\n", elf_path);
        dprintf(INFO, "boot_sys: Cmdline: %s\n", cmdline);
    }
    
    /* Load ELF into scratch memory */
    ret = elf_load(elf_load_buf, elf_load_buf + (512 * 1024) - elf_load_buf, 0);
    if (ret != ELF_BOOT_OK) {
        dprintf(CRITICAL, "boot_sys: ELF load failed: %d\n", ret);
        return BOOT_SYS_ERR_ELF_LOAD_FAIL;
    }
    
    /* Set up boot info */
    memset(&kernel_info, 0, sizeof(kernel_info));
    kernel_info.entry = elf_get_entry(elf_load_buf);
    kernel_info.tags_addr = (uint64_t)target_get_scratch_address();
    kernel_info.machine_type = 0; /* 0 for ARM64, or actual machine type */
    
    if (opts->verbose) {
        dprintf(INFO, "boot_sys: ELF entry point: 0x%llx\n", kernel_info.entry);
    }
    
    /* Boot the kernel */
    if (opts->keep_fb && fb_info) {
        dprintf(INFO, "boot_sys: Keeping framebuffer active\n");
        /* Keep display initialized - don't call target_display_shutdown() */
    }
    
    /* TODO: Jump to kernel with ATAGS/DTB containing:
     * - Framebuffer info
     * - Command line
     * - Memory map
     */
    
    dprintf(INFO, "boot_sys: Ready to boot kernel at 0x%llx\n", kernel_info.entry);
    
    return BOOT_SYS_OK;
}

/*
 * Boot from partition using BOOT.SYS
 */
int boot_sys_boot_from_partition(const char *partition,
                                struct boot_sys_options *opts,
                                struct boot_sys_fb_info *fb_info)
{
    struct boot_sys_config config;
    int ret;
    
    if (!partition) {
        partition = BOOT_SYS_DEFAULT_PARTITION;
    }
    
    if (!opts) {
        opts = &default_opts;
    }
    
    /* Allocate scratch buffer for ELF loading */
    elf_load_buf = (unsigned char *)target_get_scratch_address();
    if (!elf_load_buf) {
        dprintf(CRITICAL, "boot_sys: Cannot get scratch address\n");
        return BOOT_SYS_ERR_NO_MEMORY;
    }
    
    /* Read BOOT.SYS config */
    ret = boot_sys_read_config(partition, &config);
    if (ret != 0) {
        return ret;
    }
    
    /* Boot ELF kernel */
    return boot_sys_boot_elf(config.kernel_path, config.cmdline, opts, fb_info);
}

/*
 * Initialize boot_sys module
 */
int boot_sys_init(void)
{
    dprintf(INFO, "boot_sys: Initializing\n");
    
    /* Get framebuffer info if available */
    if (boot_sys_get_fb_info(&current_fb) == 0) {
        dprintf(INFO, "boot_sys: Framebuffer available: %dx%d\n",
                current_fb.width, current_fb.height);
    }
    
    return 0;
}

/*
 * Print boot_sys debug info
 */
void boot_sys_dump_config(const struct boot_sys_config *config)
{
    if (!config) {
        dprintf(CRITICAL, "boot_sys: NULL config\n");
        return;
    }
    
    dprintf(INFO, "boot_sys: Configuration:\n");
    dprintf(INFO, "  valid: %s\n", config->valid ? "yes" : "no");
    dprintf(INFO, "  kernel: %s\n", config->kernel_path);
    dprintf(INFO, "  cmdline: %s\n", config->cmdline);
}
