/*
 * Generic Qualcomm MSM8xxx SoC family register definitions.
 *
 * Covers MSM8x25, MSM8x27, MSM8x30, MSM8x60, MSM8960, MSM8974, MSM8916
 * and related SoCs from the Snapdragon family.
 */

#ifndef _PEXPERT_MSM8XXX_H_
#define _PEXPERT_MSM8XXX_H_

/*
 * GIC v2 distributor and CPU interface registers.
 * Offsets are the same across all MSM GIC implementations.
 */
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

/*
 * MSM SoC-specific GIC bases.
 * These vary by SoC generation; used as defaults.
 */
#define MSM8XXX_GIC_DIST_BASE_8X60  0x02080000
#define MSM8XXX_GIC_CPU_BASE_8X60   0x02081000

#define MSM8XXX_GIC_DIST_BASE_8974  0xF9000000
#define MSM8XXX_GIC_CPU_BASE_8974   0xF9002000

#define MSM8XXX_GIC_DIST_BASE_8916  0x0B000000
#define MSM8XXX_GIC_CPU_BASE_8916   0x0B002000

/*
 * BLSP UART DM (Data Mover) registers.
 * Used across all modern MSM SoCs (8x60, 8960, 8974, 8916+).
 */
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

#define UART_DM_SR_TXEMT            (1 << 3)
#define UART_DM_SR_TXRDY            (1 << 2)
#define UART_DM_SR_RXRDY            (1 << 0)

#define UART_DM_CR_RESET_TX         0x0C
#define UART_DM_CR_RESET_RX         0x10
#define UART_DM_CR_RESET_ERROR      0x14
#define UART_DM_CR_ENA_TX           0x24
#define UART_DM_CR_ENA_RX           0x28
#define UART_DM_CR_TX_NEW           (1 << 4)

/* Default BLSP1 UART2 base (common debug UART on most MSM boards) */
#define MSM8XXX_BLSP1_UART2_BASE    0x078B0000

/*
 * MSM timer registers (pre-ARMv8 SoCs like 8x60, 8960, 8974).
 */
#define MSM_TMR_REG(off)            (off)

#define GPT_MATCH_VAL               MSM_TMR_REG(0x0000)
#define GPT_COUNT_VAL               MSM_TMR_REG(0x0004)
#define GPT_ENABLE                  MSM_TMR_REG(0x0008)
#define GPT_CLEAR                   MSM_TMR_REG(0x000C)

#define DGT_MATCH_VAL               MSM_TMR_REG(0x0024)
#define DGT_COUNT_VAL               MSM_TMR_REG(0x0028)
#define DGT_ENABLE                  MSM_TMR_REG(0x002C)
#define DGT_CLEAR                   MSM_TMR_REG(0x0030)

#define GPT_ENABLE_CLR_ON_MATCH_EN  2
#define GPT_ENABLE_EN               1
#define DGT_ENABLE_CLR_ON_MATCH_EN  2
#define DGT_ENABLE_EN               1

/* Interrupt number bases */
#define GIC_PPI_START               16
#define GIC_SPI_START               32

#define INT_DEBUG_TIMER_EXP         (GIC_PPI_START + 1)

#define NR_MSM_IRQS                 256
#define NR_GPIO_IRQS                150
#define NR_BOARD_IRQS               0
#define NR_IRQS                     (NR_MSM_IRQS + NR_GPIO_IRQS + NR_BOARD_IRQS)

#endif /* !_PEXPERT_MSM8XXX_H_ */
