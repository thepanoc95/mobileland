/*
 * Copyright (C) 2024 ravynOS Authors
 *
 * ELF Boot Support for lk2nd
 * 
 * This module provides basic ELF loading and booting capabilities
 * for raw ELF kernel files.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 */

#ifndef _ELF_BOOT_H_
#define _ELF_BOOT_H_

#include <stdint.h>
#include <stdbool.h>

/* ELF magic number */
#define ELF_MAGIC 0x464C457F  /* "\x7FELF" in little endian */

/* ELF machine types */
#define ELF_EM_ARM        40  /* ARM */
#define ELF_EM_AARCH64   183 /* ARM64 */

/* ELF program header types */
#define ELF_PT_LOAD 1

/* ELF header (32-bit) */
struct elf32_hdr {
    unsigned char e_ident[16];  /* Magic and other info */
    uint16_t e_type;            /* Object file type */
    uint16_t e_machine;         /* Architecture */
    uint32_t e_version;         /* Object file version */
    uint32_t e_entry;           /* Entry point virtual address */
    uint32_t e_phoff;           /* Program header table file offset */
    uint32_t e_shoff;           /* Section header table file offset */
    uint32_t e_flags;           /* Processor-specific flags */
    uint16_t e_ehsize;         /* ELF header size */
    uint16_t e_phentsize;       /* Program header table entry size */
    uint16_t e_phnum;           /* Program header table entry count */
    uint16_t e_shentsize;       /* Section header table entry size */
    uint16_t e_shnum;           /* Section header table entry count */
    uint16_t e_shstrndx;        /* Section header string table index */
} __attribute__((packed));

/* ELF header (64-bit) */
struct elf64_hdr {
    unsigned char e_ident[16];  /* Magic and other info */
    uint16_t e_type;            /* Object file type */
    uint16_t e_machine;         /* Architecture */
    uint32_t e_version;         /* Object file version */
    uint64_t e_entry;           /* Entry point virtual address */
    uint64_t e_phoff;           /* Program header table file offset */
    uint64_t e_shoff;           /* Section header table file offset */
    uint32_t e_flags;           /* Processor-specific flags */
    uint16_t e_ehsize;          /* ELF header size */
    uint16_t e_phentsize;       /* Program header table entry size */
    uint16_t e_phnum;           /* Program header table entry count */
    uint16_t e_shentsize;       /* Section header table entry size */
    uint16_t e_shnum;           /* Section header table entry count */
    uint16_t e_shstrndx;        /* Section header string table index */
} __attribute__((packed));

/* ELF program header (32-bit) */
struct elf32_phdr {
    uint32_t p_type;     /* Segment type */
    uint32_t p_offset;   /* Segment file offset */
    uint32_t p_vaddr;    /* Segment virtual address */
    uint32_t p_paddr;    /* Segment physical address */
    uint32_t p_filesz;   /* Segment size in file */
    uint32_t p_memsz;    /* Segment size in memory */
    uint32_t p_flags;    /* Segment flags */
    uint32_t p_align;   /* Segment alignment */
} __attribute__((packed));

/* ELF program header (64-bit) */
struct elf64_phdr {
    uint32_t p_type;     /* Segment type */
    uint32_t p_flags;    /* Segment flags */
    uint64_t p_offset;   /* Segment file offset */
    uint64_t p_vaddr;    /* Segment virtual address */
    uint64_t p_paddr;    /* Segment physical address */
    uint64_t p_filesz;   /* Segment size in file */
    uint64_t p_memsz;    /* Segment size in memory */
    uint64_t p_align;     /* Segment alignment */
} __attribute__((packed));

/* Boot info structure passed to kernel via registers or DTB */
struct elf_boot_info {
    uint64_t entry;           /* Kernel entry point */
    uint64_t tags_addr;       /* ATAGS/DTB physical address */
    uint64_t kernel_size;     /* Size of loaded kernel */
    uint64_t kernel_addr;     /* Load address of kernel */
    uint32_t machine_type;    /* ARM machine type (0 for ARM64) */
    uint32_t flags;            /* Boot flags */
};

/* ELF loading result */
enum elf_boot_result {
    ELF_BOOT_OK = 0,
    ELF_BOOT_ERR_INVALID = -1,       /* Invalid ELF file */
    ELF_BOOT_ERR_WRONG_ARCH = -2,     /* Unsupported architecture */
    ELF_BOOT_ERR_PARSE_FAIL = -3,     /* Parse error */
    ELF_BOOT_ERR_LOAD_FAIL = -4,      /* Load segment failed */
    ELF_BOOT_ERR_NO_MEMORY = -5,      /* Out of memory */
};

/*
 * Check if data is a valid ELF file
 * Returns true if valid ELF
 */
bool elf_is_valid(const void *data, size_t size);

/*
 * Get ELF class (32 or 64 bit)
 * Returns: 32, 64, or 0 if invalid
 */
int elf_get_class(const void *data);

/*
 * Get ELF machine type
 * Returns: ELF_EM_ARM, ELF_EM_AARCH64, or -1 if invalid
 */
int elf_get_machine(const void *data);

/*
 * Get entry point address
 */
uint64_t elf_get_entry(const void *data);

/*
 * Get number of program headers
 */
uint16_t elf_get_phnum(const void *data);

/*
 * Get program header
 * index: Program header index
 * phdr: Output structure
 * Returns: 0 on success
 */
int elf_get_phdr(const void *data, uint16_t index, void *phdr);

/*
 * Load ELF into memory
 * data: ELF file data
 * size: Size of ELF data
 * load_addr: Base address to load at (0 = use file addresses)
 * Returns: BOOT_SYS_OK on success
 */
int elf_load(const void *data, size_t size, uint64_t load_addr);

/*
 * Load and boot ELF kernel
 * data: ELF file data
 * size: Size of ELF data
 * cmdline: Kernel command line
 * info: Boot info structure (can be NULL)
 * Returns: BOOT_SYS_OK on success, kernel booted
 */
int elf_boot(const void *data, size_t size, const char *cmdline,
             struct elf_boot_info *info);

/*
 * Get memory bounds of loaded ELF
 * min_addr: Lowest used address (output)
 * max_addr: Highest used address (output)
 */
void elf_get_memory_bounds(uint64_t *min_addr, uint64_t *max_addr);

/*
 * Parse program header for 32-bit ELF
 */
static inline void elf32_parse_phdr(const struct elf32_phdr *src, 
                                     uint32_t *type, uint32_t *offset,
                                     uint32_t *vaddr, uint32_t *paddr,
                                     uint32_t *filesz, uint32_t *memsz,
                                     uint32_t *flags, uint32_t *align)
{
    *type = src->p_type;
    *offset = src->p_offset;
    *vaddr = src->p_vaddr;
    *paddr = src->p_paddr;
    *filesz = src->p_filesz;
    *memsz = src->p_memsz;
    *flags = src->p_flags;
    *align = src->p_align;
}

/*
 * Parse program header for 64-bit ELF
 */
static inline void elf64_parse_phdr(const struct elf64_phdr *src,
                                    uint32_t *type, uint32_t *flags,
                                    uint64_t *offset, uint64_t *vaddr,
                                    uint64_t *paddr, uint64_t *filesz,
                                    uint64_t *memsz, uint64_t *align)
{
    *type = src->p_type;
    *flags = src->p_flags;
    *offset = src->p_offset;
    *vaddr = src->p_vaddr;
    *paddr = src->p_paddr;
    *filesz = src->p_filesz;
    *memsz = src->p_memsz;
    *align = src->p_align;
}

#endif /* _ELF_BOOT_H_ */
