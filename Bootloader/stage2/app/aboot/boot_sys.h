/*
 * Copyright (C) 2024 ravynOS Authors
 *
 * Boot from RAW ELF files from /boot partition
 * 
 * This module allows lk2nd to boot raw ELF kernels from a /boot partition
 * by reading a BOOT.SYS configuration file.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Author may be used to endorse or promote
 *    products derived from this software without specific prior written
 *    permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
 * TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _BOOT_SYS_H_
#define _BOOT_SYS_H_

#include <stdint.h>
#include <stdbool.h>

/* BOOT.SYS file format (plain text):
 *
 * KERNEL=/boot/kernel.elf
 * CMDLINE=root=/dev/mmcblk0p15 quiet
 *
 * Lines starting with # are comments
 * Empty lines are ignored
 */

/* Maximum length of BOOT.SYS file line */
#define BOOT_SYS_LINE_LEN 256

/* Maximum length of kernel path */
#define BOOT_SYS_KERNEL_PATH_LEN 128

/* Maximum length of kernel command line */
#define BOOT_SYS_CMDLINE_LEN 512

/* Maximum number of lines to parse */
#define BOOT_SYS_MAX_LINES 32

/* Default partition name to search for BOOT.SYS */
#define BOOT_SYS_DEFAULT_PARTITION "boot"

/* Maximum size of BOOT.SYS file to read */
#define BOOT_SYS_MAX_FILE_SIZE 4096

/* Boot configuration parsed from BOOT.SYS */
struct boot_sys_config {
    char kernel_path[BOOT_SYS_KERNEL_PATH_LEN];
    char cmdline[BOOT_SYS_CMDLINE_LEN];
    bool valid;
};

/* Boot result codes */
enum boot_sys_result {
    BOOT_SYS_OK = 0,
    BOOT_SYS_ERR_NO_PARTITION = -1,
    BOOT_SYS_ERR_NO_FILE = -2,
    BOOT_SYS_ERR_PARSE_ERROR = -3,
    BOOT_SYS_ERR_ELF_INVALID = -4,
    BOOT_SYS_ERR_ELF_LOAD_FAIL = -5,
    BOOT_SYS_ERR_BOOT_FAIL = -6,
};

/* Framebuffer info passed to kernel */
struct boot_sys_fb_info {
    uint32_t width;
    uint32_t height;
    uint32_t stride;
    uint32_t bpp;
    uint32_t format;
    uint64_t base;
    uint32_t size;
};

/* Boot options */
struct boot_sys_options {
    bool keep_fb;          /* Keep framebuffer active */
    bool verbose;         /* Verbose output */
    bool use_tags;        /* Use ATAGS/DTB */
};

/*
 * Initialize boot_sys module
 * Returns 0 on success
 */
int boot_sys_init(void);

/*
 * Attempt to boot from /boot partition using BOOT.SYS config
 * 
 * This function:
 * 1. Reads BOOT.SYS from the specified partition
 * 2. Parses KERNEL= and CMDLINE= directives
 * 3. Loads the ELF kernel from the specified path
 * 4. Boot the kernel with the given command line
 * 5. Optionally keeps framebuffer active
 *
 * Parameters:
 *   partition - Partition name to search (e.g., "boot", "system")
 *   opts      - Boot options (can be NULL for defaults)
 *   fb_info   - Framebuffer info to pass to kernel (can be NULL)
 *
 * Returns:
 *   BOOT_SYS_OK on success
 *   Negative error code on failure
 */
int boot_sys_boot_from_partition(const char *partition, 
                                 struct boot_sys_options *opts,
                                 struct boot_sys_fb_info *fb_info);

/*
 * Read and parse BOOT.SYS from a partition
 * 
 * Parameters:
 *   partition - Partition name
 *   config    - Output config structure
 *
 * Returns:
 *   0 on success
 *   Negative on error
 */
int boot_sys_read_config(const char *partition, struct boot_sys_config *config);

/*
 * Get the current framebuffer info
 * Fills fb_info structure with current display parameters
 *
 * Returns:
 *   0 on success
 *   Negative on error (no framebuffer available)
 */
int boot_sys_get_fb_info(struct boot_sys_fb_info *fb_info);

/*
 * Boot from a raw ELF file directly
 *
 * Parameters:
 *   elf_path  - Path to ELF file on partition
 *   cmdline   - Kernel command line
 *   opts      - Boot options
 *   fb_info   - Framebuffer info
 *
 * Returns:
 *   BOOT_SYS_OK on success
 *   Negative error code on failure
 */
int boot_sys_boot_elf(const char *elf_path, const char *cmdline,
                       struct boot_sys_options *opts,
                       struct boot_sys_fb_info *fb_info);

/*
 * Print boot_sys debug info
 */
void boot_sys_dump_config(const struct boot_sys_config *config);

#endif /* _BOOT_SYS_H_ */
