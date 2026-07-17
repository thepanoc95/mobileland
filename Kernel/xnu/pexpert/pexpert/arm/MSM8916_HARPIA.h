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
 * Qualcomm MSM8916 (Snapdragon 410) Platform Expert definitions for ARM32.
 * Supports the Motorola Harpia (Moto G4 Play) and similar MSM8916 devices.
 */

#ifndef _PEXPERT_ARM_MSM8916_HARPIA_H
#define _PEXPERT_ARM_MSM8916_HARPIA_H

#ifdef ARM_BOARD_CONFIG_MSM8916_HARPIA

#include <pexpert/arm/board_config.h>

#define ARM_ARCH_TIMER         /* Use ARM Generic Timer */

/* ARMv7 Cortex-A7 (MSM8916 uses A7 cores) */
#define ARMA7

/* MSM8916 is UP (single core) for initial port */
#define __XNU_UP__

/* L2 cache size for Cortex-A7: 512KB = 2^19 */
#define __ARM_L2CACHE_SIZE_LOG__ 19

/* WFE timeout for spinlocks */
#define ARM_BOARD_WFE_TIMEOUT_NS 10000

/* Board class identifier */
#define ARM_BOARD_CLASS_MSM8916_HARPIA

/* MSM8916 specific settings */
#define MSM8916_HARPIA

/*
 * SoC base address - MSM8916 uses 0x00000000 for ROM and 0x0E000000 for SRAM
 * The peripheral base is typically at 0x0F000000
 */
#ifndef ASSEMBLER
extern vm_offset_t gSocPhys;
#endif

#define MSM8916_SOC_PHYS_BASE      0x0F000000

/*
 * GIC v2 Configuration (MSM8916 uses ARM GIC-400)
 */
#define MSM8916_GIC_DIST_PHYS      0x0B000000
#define MSM8916_GIC_CPU_PHYS       0x0B002000

/*
 * UART Configuration (BLSP1 UART2 on MSM8916)
 */
#define MSM8916_UART_PHYS          0x078B0000

/*
 * Timer Configuration
 * MSM8916 uses ARM Generic Timer (CP15 based)
 */

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

#endif /* ARM_BOARD_CONFIG_MSM8916_HARPIA */

#endif /* !_PEXPERT_ARM_MSM8916_HARPIA_H */
