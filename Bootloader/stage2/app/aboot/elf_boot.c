/*
 * Copyright (C) 2024 ravynOS Authors
 *
 * ELF Boot Support for lk2nd
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 */

#include <app.h>
#include <debug.h>
#include <string.h>
#include <stdlib.h>
#include <kernel/thread.h>
#include <arch/ops.h>
#include <arch/arm.h>

#include "elf_boot.h"

/* Memory bounds of loaded ELF */
static uint64_t elf_min_addr = 0xFFFFFFFFFFFFFFFFULL;
static uint64_t elf_max_addr = 0;

/*
 * Check if data is a valid ELF file
 */
bool elf_is_valid(const void *data, size_t size)
{
    const unsigned char *e_ident = (const unsigned char *)data;
    
    if (!data || size < 16)
        return false;
    
    /* Check ELF magic */
    if (e_ident[0] != 0x7F || e_ident[1] != 'E' || 
        e_ident[2] != 'L' || e_ident[3] != 'F') {
        return false;
    }
    
    return true;
}

/*
 * Get ELF class (32 or 64 bit)
 */
int elf_get_class(const void *data)
{
    const unsigned char *e_ident = (const unsigned char *)data;
    
    if (!elf_is_valid(data, 16))
        return 0;
    
    return e_ident[4]; /* EI_CLASS */
}

/*
 * Get ELF machine type
 */
int elf_get_machine(const void *data)
{
    if (!elf_is_valid(data, 20))
        return -1;
    
    if (elf_get_class(data) == 1) {
        /* 32-bit ELF */
        const struct elf32_hdr *hdr = (const struct elf32_hdr *)data;
        return hdr->e_machine;
    } else if (elf_get_class(data) == 2) {
        /* 64-bit ELF */
        const struct elf64_hdr *hdr = (const struct elf64_hdr *)data;
        return hdr->e_machine;
    }
    
    return -1;
}

/*
 * Get entry point address
 */
uint64_t elf_get_entry(const void *data)
{
    if (!elf_is_valid(data, 24))
        return 0;
    
    if (elf_get_class(data) == 1) {
        /* 32-bit ELF */
        const struct elf32_hdr *hdr = (const struct elf32_hdr *)data;
        return hdr->e_entry;
    } else if (elf_get_class(data) == 2) {
        /* 64-bit ELF */
        const struct elf64_hdr *hdr = (const struct elf64_hdr *)data;
        return hdr->e_entry;
    }
    
    return 0;
}

/*
 * Get number of program headers
 */
uint16_t elf_get_phnum(const void *data)
{
    if (!elf_is_valid(data, 40))
        return 0;
    
    if (elf_get_class(data) == 1) {
        /* 32-bit ELF */
        const struct elf32_hdr *hdr = (const struct elf32_hdr *)data;
        return hdr->e_phnum;
    } else if (elf_get_class(data) == 2) {
        /* 64-bit ELF */
        const struct elf64_hdr *hdr = (const struct elf64_hdr *)data;
        return hdr->e_phnum;
    }
    
    return 0;
}

/*
 * Get program header
 */
int elf_get_phdr(const void *data, uint16_t index, void *phdr_out)
{
    uint16_t phnum;
    uint64_t phoff;
    size_t phdr_size;
    
    if (!elf_is_valid(data, 64))
        return -1;
    
    if (!phdr_out)
        return -1;
    
    phnum = elf_get_phnum(data);
    if (index >= phnum)
        return -1;
    
    if (elf_get_class(data) == 1) {
        /* 32-bit ELF */
        const struct elf32_hdr *hdr = (const struct elf32_hdr *)data;
        const struct elf32_phdr *phdr_array;
        
        phoff = hdr->e_phoff;
        phdr_size = sizeof(struct elf32_phdr);
        
        if (phoff + (index + 1) * phdr_size > (size_t)hdr->e_shoff)
            return -1;
        
        phdr_array = (const struct elf32_phdr *)((const char *)data + phoff);
        memcpy(phdr_out, &phdr_array[index], phdr_size);
        
    } else if (elf_get_class(data) == 2) {
        /* 64-bit ELF */
        const struct elf64_hdr *hdr = (const struct elf64_hdr *)data;
        const struct elf64_phdr *phdr_array;
        
        phoff = hdr->e_phoff;
        phdr_size = sizeof(struct elf64_phdr);
        
        if (phoff + (index + 1) * phdr_size > (size_t)hdr->e_shoff)
            return -1;
        
        phdr_array = (const struct elf64_phdr *)((const char *)data + phoff);
        memcpy(phdr_out, &phdr_array[index], phdr_size);
    }
    
    return 0;
}

/*
 * Load ELF segments into memory
 */
int elf_load(const void *data, size_t size, uint64_t load_addr)
{
    uint16_t phnum;
    int i;
    int ret;
    uint64_t min_addr = 0xFFFFFFFFFFFFFFFFULL;
    uint64_t max_addr = 0;
    
    if (!elf_is_valid(data, size)) {
        dprintf(CRITICAL, "elf_boot: Invalid ELF file\n");
        return ELF_BOOT_ERR_INVALID;
    }
    
    /* Check architecture */
    int machine = elf_get_machine(data);
    if (machine != ELF_EM_ARM && machine != ELF_EM_AARCH64) {
        dprintf(CRITICAL, "elf_boot: Unsupported machine type: %d\n", machine);
        return ELF_BOOT_ERR_WRONG_ARCH;
    }
    
    /* Get program headers */
    phnum = elf_get_phnum(data);
    if (phnum == 0) {
        dprintf(CRITICAL, "elf_boot: No program headers found\n");
        return ELF_BOOT_ERR_PARSE_FAIL;
    }
    
    dprintf(INFO, "elf_boot: Loading ELF with %d program headers\n", phnum);
    
    /* Reset memory bounds */
    elf_min_addr = 0xFFFFFFFFFFFFFFFFULL;
    elf_max_addr = 0;
    
    /* Load each PT_LOAD segment */
    for (i = 0; i < phnum; i++) {
        uint32_t p_type, p_flags;
        uint64_t p_offset, p_vaddr, p_paddr, p_filesz, p_memsz;
        
        if (elf_get_class(data) == 1) {
            /* 32-bit ELF */
            struct elf32_phdr phdr;
            elf_get_phdr(data, i, &phdr);
            p_type = phdr.p_type;
            p_flags = phdr.p_flags;
            p_offset = phdr.p_offset;
            p_vaddr = phdr.p_vaddr;
            p_paddr = phdr.p_paddr;
            p_filesz = phdr.p_filesz;
            p_memsz = phdr.p_memsz;
        } else {
            /* 64-bit ELF */
            struct elf64_phdr phdr;
            elf_get_phdr(data, i, &phdr);
            p_type = phdr.p_type;
            p_flags = phdr.p_flags;
            p_offset = phdr.p_offset;
            p_vaddr = phdr.p_vaddr;
            p_paddr = phdr.p_paddr;
            p_filesz = phdr.p_filesz;
            p_memsz = phdr.p_memsz;
        }
        
        /* Only process PT_LOAD segments */
        if (p_type != ELF_PT_LOAD)
            continue;
        
        /* Validate segment */
        if (p_offset + p_filesz > size) {
            dprintf(CRITICAL, "elf_boot: Segment %d extends beyond file\n", i);
            return ELF_BOOT_ERR_PARSE_FAIL;
        }
        
        /* Calculate actual load addresses */
        uint64_t dest_addr;
        if (load_addr != 0) {
            /* Relocate based on load_addr */
            dest_addr = load_addr + p_vaddr;
        } else {
            /* Use physical address from ELF */
            dest_addr = p_paddr ? p_paddr : p_vaddr;
        }
        
        dprintf(INFO, "elf_boot: Loading segment %d: file @0x%llx, mem @0x%llx (%llu bytes)\n",
                i, p_offset, dest_addr, p_filesz);
        
        /* Copy segment data to load address */
        if (p_filesz > 0) {
            void *dest = (void *)(addr_t)dest_addr;
            const void *src = (const char *)data + p_offset;
            memcpy(dest, src, p_filesz);
        }
        
        /* Zero BSS section (memory > file size) */
        if (p_memsz > p_filesz) {
            void *bss_start = (void *)(addr_t)(dest_addr + p_filesz);
            size_t bss_size = p_memsz - p_filesz;
            dprintf(INFO, "elf_boot: Zeroing BSS: @0x%llx (%llu bytes)\n",
                    (uint64_t)(addr_t)bss_start, bss_size);
            memset(bss_start, 0, bss_size);
        }
        
        /* Update memory bounds */
        if (dest_addr < min_addr)
            min_addr = dest_addr;
        if (dest_addr + p_memsz > max_addr)
            max_addr = dest_addr + p_memsz;
    }
    
    /* Store memory bounds */
    elf_min_addr = min_addr;
    elf_max_addr = max_addr;
    
    dprintf(INFO, "elf_boot: ELF loaded to memory range 0x%llx - 0x%llx\n",
            min_addr, max_addr);
    
    return ELF_BOOT_OK;
}

/*
 * Get memory bounds of loaded ELF
 */
void elf_get_memory_bounds(uint64_t *min_addr_out, uint64_t *max_addr_out)
{
    if (min_addr_out)
        *min_addr_out = elf_min_addr;
    if (max_addr_out)
        *max_addr_out = elf_max_addr;
}

/*
 * Boot ELF kernel
 */
int elf_boot(const void *data, size_t size, const char *cmdline,
             struct elf_boot_info *info)
{
    uint64_t entry;
    uint64_t min_addr, max_addr;
    void (*kernel_entry)(unsigned, unsigned, unsigned*) = NULL;
    
    if (!elf_is_valid(data, size)) {
        return ELF_BOOT_ERR_INVALID;
    }
    
    /* Load ELF */
    int ret = elf_load(data, size, 0);
    if (ret != ELF_BOOT_OK) {
        return ret;
    }
    
    /* Get entry point */
    entry = elf_get_entry(data);
    if (entry == 0) {
        dprintf(CRITICAL, "elf_boot: No entry point found\n");
        return ELF_BOOT_ERR_LOAD_FAIL;
    }
    
    /* Get memory bounds */
    elf_get_memory_bounds(&min_addr, &max_addr);
    
    dprintf(INFO, "elf_boot: Boot entry point: 0x%llx\n", entry);
    dprintf(INFO, "elf_boot: Memory range: 0x%llx - 0x%llx\n", min_addr, max_addr);
    
    /* Fill boot info */
    if (info) {
        info->entry = entry;
        info->kernel_size = max_addr - min_addr;
        info->kernel_addr = min_addr;
        info->machine_type = (elf_get_machine(data) == ELF_EM_AARCH64) ? 0 : ELF_EM_ARM;
    }
    
    /* Flush and disable caches before jumping to kernel */
    arch_disable_mmu();
    
    /* Jump to kernel */
    dprintf(INFO, "elf_boot: Jumping to kernel...\n");
    
    /* For ARM64: entry is already virtual address, call directly */
    kernel_entry = (void (*)(unsigned, unsigned, unsigned*))(addr_t)entry;
    
    /* Boot with ATAGS pointer in r2, machine type in r1, 0 in r0 */
    /* For ARM64, r0-r2 are used differently */
    kernel_entry(0, 0, (unsigned*)info);
    
    /* Should not return */
    dprintf(CRITICAL, "elf_boot: Kernel returned!\n");
    
    return ELF_BOOT_ERR_BOOT_FAIL;
}
