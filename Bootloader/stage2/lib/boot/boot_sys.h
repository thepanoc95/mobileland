/*
 * Copyright (C) 2024 ravynOS Authors
 *
 * Public interface for ELF boot from /boot partition
 */

#ifndef _LIB_BOOT_SYS_H_
#define _LIB_BOOT_SYS_H_

#include <stdint.h>
#include <stdbool.h>

/* Forward declaration */
struct boot_sys_config;
struct boot_sys_options;
struct boot_sys_fb_info;

/*
 * Boot from /boot partition using BOOT.SYS configuration
 *
 * Reads BOOT.SYS from the specified partition and boots a raw ELF kernel.
 *
 * @param partition - Partition name (NULL = "boot")
 * @return 0 on success, negative on error
 */
int boot_linux_from_boot_partition(const char *partition);

/*
 * Boot ELF file directly
 *
 * @param elf_path - Path to ELF file on partition
 * @param cmdline - Kernel command line (NULL = empty)
 * @return 0 on success, negative on error
 */
int boot_elf_file(const char *elf_path, const char *cmdline);

#endif /* _LIB_BOOT_SYS_H_ */
