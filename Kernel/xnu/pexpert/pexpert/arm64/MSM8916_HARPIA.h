/*
 * Copyright 2026, The PocketDarwin Project.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *   Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 *   Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * Qualcomm MSM8916 (Snapdragon 410) Platform Expert definitions for ARM64.
 * Supports the Motorola Harpia (Moto G4 Play) and similar MSM8916 devices.
 *
 * MSM8916 is a 64-bit capable SoC based on ARM Cortex-A53 cores.
 */

#ifndef _PEXPERT_ARM64_MSM8916_HARPIA_H
#define _PEXPERT_ARM64_MSM8916_HARPIA_H

#ifdef ARM64_BOARD_CONFIG_MSM8916_HARPIA

#include <pexpert/arm64/board_config.h>

/* Enable ARM Generic Timer for ARM64 */
#define ARM_ARCH_TIMER

/* MSM8916 uses Cortex-A53 cores (ARMv8-A) */
#define ARM_BOARD_CLASS_MSM8916_HARPIA

/* MSM8916 specific settings */
#define MSM8916_HARPIA

/* No monitor/no secure monitor for initial bringup */
#define NO_MONITOR 1
#define NO_ECORE 1

#ifndef ASSEMBLER

/*
 * SoC base physical address
 * MSM8916 memory map:
 *   0x00000000 - 0x3FFFFFFF: DRAM (up to 1GB)
 *   0x0E000000 - 0x0EFFFFFF: SRAM
 *   0x0F000000 - 0x0FFFFFFF: Peripheral space
 */
#define MSM8916_SOC_PHYS_BASE      0x0F000000

/*
 * GIC v2 Configuration (MSM8916 uses ARM GIC-400)
 */
#define MSM8916_GIC_DIST_PHYS      0x0B000000
#define MSM8916_GIC_CPU_PHYS       0x0B002000

/*
 * UART Configuration (BLSP1 UART2 on MSM8916)
 * This is the console UART typically used for early boot debugging
 */
#define MSM8916_UART_PHYS          0x078B0000

/*
 * ARM Generic Timer configuration
 * CNTFRQ is typically 19.2 MHz (from PMIC)
 * CNTPCT_EL0 provides the physical count
 */

/*
 * Platform panic log region (optional, for development)
 */
#define PLATFORM_PANIC_LOG_PADDR   0x3C0FC000
#define PLATFORM_PANIC_LOG_SIZE     16384    /* 16KB */

/*
 * I/O register access macros for MSM8916
 */
#define MSM8916_READ32(addr)       (*((volatile uint32_t *)(addr)))
#define MSM8916_WRITE32(addr, val) do { *((volatile uint32_t *)(addr)) = (val); } while(0)
#define MSM8916_READ64(addr)       (*((volatile uint64_t *)(addr)))
#define MSM8916_WRITE64(addr, val) do { *((volatile uint64_t *)(addr)) = (val); } while(0)

/*
 * Clock configuration for MSM8916
 * These values are typical for MSM8916/Snapdragon 410
 */
#define MSM8916_CPU_FREQ_HZ        1200000000ULL   /* 1.2 GHz typical */
#define MSM8916_BUS_FREQ_HZ        400000000ULL    /* 400 MHz */
#define MSM8916_GPU_FREQ_HZ        294000000ULL    /* 294 MHz */
#define MSM8916_MEM_FREQ_HZ        533000000ULL    /* 533 MHz LPDDR3 */

/*
 * Peripheral clock frequencies
 */
#define MSM8916_UART_FREQ_HZ       7372800ULL      /* 7.3728 MHz */
#define MSM8916_SPI_FREQ_HZ        48000000ULL     /* 48 MHz */

/*
 * Interrupt Configuration
 * GIC PPI_BASE = 16, SPI_BASE = 32
 */
#define GIC_PPI_START              16
#define GIC_SPI_START              32

/*
 * Platform-specific CPU count (MSM8916 is quad-core)
 */
#define CPU_COUNT                  4
#define CORE_NCTRS                4

/*
 * L2 cache configuration for Cortex-A53
 * Typically 512KB shared L2 cache
 */
#define __ARM_L2CACHE_SIZE_LOG__   19  /* 2^19 = 512KB */

/*
 * WFE timeout for spinlocks (in nanoseconds)
 */
#define ARM_BOARD_WFE_TIMEOUT_NS   10000

#endif /* !ASSEMBLER */

#endif /* ARM64_BOARD_CONFIG_MSM8916_HARPIA */

#endif /* !_PEXPERT_ARM64_MSM8916_HARPIA_H */
