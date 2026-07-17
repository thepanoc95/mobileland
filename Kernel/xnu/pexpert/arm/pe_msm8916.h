/*
 * Copyright 2024, The PocketDarwin Team.
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

#ifndef _PEXPERT_MSM8916_H_
#define _PEXPERT_MSM8916_H_

/*
 * Qualcomm MSM8916 (Snapdragon 410) memory map and register definitions.
 */

/* GIC v2 */
#define MSM8916_GIC_DIST_BASE        0x0B000000
#define MSM8916_GIC_CPU_BASE         0x0B002000

#define GIC_CPU_CTRL                0x00
#define GIC_CPU_PRIMASK             0x04
#define GIC_CPU_BINPOINT            0x08
#define GIC_CPU_INTACK              0x0C
#define GIC_CPU_EOI                 0x10
#define GIC_CPU_RUNNINGPRI          0x14
#define GIC_CPU_HIGHPRI             0x18

#define GIC_DIST_CTRL               0x000
#define GIC_DIST_CTR                0x004
#define GIC_DIST_ENABLE_SET         0x100
#define GIC_DIST_ENABLE_CLEAR       0x180
#define GIC_DIST_PENDING_SET        0x200
#define GIC_DIST_PENDING_CLEAR      0x280
#define GIC_DIST_ACTIVE_BIT         0x300
#define GIC_DIST_PRI                0x400
#define GIC_DIST_TARGET             0x800
#define GIC_DIST_CONFIG             0xC00
#define GIC_DIST_SOFTINT            0xF00

/* BLSP1 UART2 (console) */
#define MSM8916_BLSP1_UART2_BASE    0x078B0000

/* UART DM (Data Mover) registers */
#define UART_DM_SR                  0x0A0
#define UART_DM_CR                  0x0A4
#define UART_DM_TF                  0x0A8
#define UART_DM_RF                  0x0AC
#define UART_DM_MR1                 0x0B0
#define UART_DM_MR2                 0x0B4
#define UART_DM_IPR                 0x0B8
#define UART_DM_TFWR                0x0BC
#define UART_DM_RX_TOTAL_SNAP       0x0C8
#define UART_DM_DMRX                0x0D0
#define UART_DM_IRQ_STAT            0x0F0

/* UART DM status register bits */
#define UART_DM_SR_TXEMT            (1 << 3)
#define UART_DM_SR_TXRDY            (1 << 2)
#define UART_DM_SR_RXRDY            (1 << 0)

/* UART DM command register bits */
#define UART_DM_CR_RESET_TX         0x0C
#define UART_DM_CR_RESET_RX         0x10
#define UART_DM_CR_RESET_ERROR      0x14
#define UART_DM_CR_ENA_TX           0x24
#define UART_DM_CR_ENA_RX           0x28
#define UART_DM_CR_TX_NEW           (1 << 4)

/* Interrupts */
#define GIC_PPI_START               16
#define GIC_SPI_START               32

#define INT_DEBUG_TIMER_EXP         (GIC_PPI_START + 1) /* PPI 1 = physical timer */

#define NR_MSM8916_IRQS             256
#define NR_IRQS                     NR_MSM8916_IRQS

#endif /* !_PEXPERT_MSM8916_H_ */
