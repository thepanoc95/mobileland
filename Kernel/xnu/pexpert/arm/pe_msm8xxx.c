/*
 * Generic Platform Expert for Qualcomm MSM8xxx SoC family.
 *
 * Provides a bringup-capable platform expert for any MSM8xxx-based device
 * that doesn't have a dedicated board file. Supports MSM8x60, MSM8960,
 * MSM8974, MSM8916 and similar SoCs.
 *
 * GIC and UART base addresses are configurable via compile-time defines:
 *   MSM8XXX_GIC_DIST_PADDR, MSM8XXX_GIC_CPU_PADDR, MSM8XXX_UART_PADDR
 */

#if defined(BOARD_CONFIG_MSM8XXX)

#define PE_INIT_SOC_SUPPORT_STUB_ALREADY_DEFINED 1

#include <mach/mach_types.h>
#include <pexpert/pexpert.h>
#include <pexpert/arm/protos.h>
#include <pexpert/arm/boot.h>
#include <machine/machine_routines.h>
#include <vm/pmap.h>
#include <arm/pmap.h>

#include "pe_msm8xxx.h"

#define KPRINTF_PREFIX  "PE_MSM8XXX: "

#define HwReg(x) *((volatile unsigned long *)(x))

extern void rtclock_intr(arm_saved_state_t *regs);
extern void rtc_configure(uint64_t hz);

#ifndef MSM8XXX_GIC_DIST_PADDR
#define MSM8XXX_GIC_DIST_PADDR      MSM8XXX_GIC_DIST_BASE_8916
#endif
#ifndef MSM8XXX_GIC_CPU_PADDR
#define MSM8XXX_GIC_CPU_PADDR       MSM8XXX_GIC_CPU_BASE_8916
#endif
#ifndef MSM8XXX_UART_PADDR
#define MSM8XXX_UART_PADDR          MSM8XXX_BLSP1_UART2_BASE
#endif

static vm_offset_t gGICDistBase = 0;
static vm_offset_t gGICCPUBase = 0;
static vm_offset_t gUARTBase = 0;

static uint64_t clock_decrementer = 0;
static boolean_t clock_initialized = FALSE;
static boolean_t clock_had_irq = FALSE;
static uint64_t clock_absolute_time = 0;

static void timer_configure(void)
{
    uint64_t hz = 19200000;
    gPEClockFrequencyInfo.timebase_frequency_hz = hz;
    clock_decrementer = 1000;
    rtc_configure(hz);
}

void msm8xxx_putc(int c)
{
    if (!gUARTBase)
        return;

    if (c == '\n')
        msm8xxx_putc('\r');

    while (!(HwReg(gUARTBase + UART_DM_SR) & UART_DM_SR_TXEMT))
        barrier();

    HwReg(gUARTBase + UART_DM_TF) = c;
}

int msm8xxx_getc(void)
{
    int c = -1;
    if (!gUARTBase)
        return c;
    if (HwReg(gUARTBase + UART_DM_SR) & UART_DM_SR_RXRDY)
        c = HwReg(gUARTBase + UART_DM_RF) & 0xFF;
    return c;
}

void msm8xxx_uart_init(void)
{
    gUARTBase = ml_io_map(MSM8XXX_UART_PADDR, 0x1000);
    assert(gUARTBase);
    kprintf(KPRINTF_PREFIX "UART at 0x%x -> %p\n", MSM8XXX_UART_PADDR, (void *)gUARTBase);
}

static void qgic_dist_init(void)
{
    uint32_t i, num_irq = 0, cpumask = 1;
    cpumask |= cpumask << 8;
    cpumask |= cpumask << 16;

    HwReg(gGICDistBase + GIC_DIST_CTRL) = 0;
    num_irq = (HwReg(gGICDistBase + GIC_DIST_CTR) & 0x1f) + 1;
    num_irq *= 32;

    for (i = 32; i < num_irq; i += 16)
        HwReg(gGICDistBase + GIC_DIST_CONFIG + i * 4 / 16) = 0xFFFFFFFF;
    HwReg(gGICDistBase + GIC_DIST_CONFIG + 4) = 0xFFFFFFFF;

    for (i = 32; i < num_irq; i += 4)
        HwReg(gGICDistBase + GIC_DIST_TARGET + i * 4 / 4) = cpumask;
    for (i = 0; i < num_irq; i += 4)
        HwReg(gGICDistBase + GIC_DIST_PRI) = 0xA0A0A0A0;
    for (i = 0; i < num_irq; i += 32)
        HwReg(gGICDistBase + GIC_DIST_ENABLE_CLEAR + i * 4 / 32) = 0xFFFFFFFF;

    HwReg(gGICDistBase + GIC_DIST_ENABLE_SET) = 0xFFFF;
    HwReg(gGICDistBase + GIC_DIST_CTRL) = 0x1;
}

static void qgic_cpu_init(void)
{
    HwReg(gGICCPUBase + GIC_CPU_PRIMASK) = 0xF0;
    HwReg(gGICCPUBase + GIC_CPU_CTRL) = 0x1;
}

void msm8xxx_interrupt_init(void)
{
    gGICDistBase = ml_io_map(MSM8XXX_GIC_DIST_PADDR, 0x1000);
    gGICCPUBase = ml_io_map(MSM8XXX_GIC_CPU_PADDR, 0x1000);
    assert(gGICDistBase && gGICCPUBase);
    qgic_dist_init();
    qgic_cpu_init();
}

uint64_t msm8xxx_timer_value(void);
void msm8xxx_timer_enabled(int enable);

void msm8xxx_timebase_init(void)
{
    timer_configure();
    msm8xxx_timer_enabled(FALSE);

    uint32_t reg_off = GIC_DIST_ENABLE_SET + (INT_DEBUG_TIMER_EXP / 32) * 4;
    uint32_t bit = 1 << (INT_DEBUG_TIMER_EXP & 31);
    HwReg(gGICDistBase + reg_off) = bit;

    ml_set_interrupts_enabled(TRUE);
    msm8xxx_timer_enabled(TRUE);

    clock_initialized = TRUE;
    while (!clock_had_irq)
        barrier();
}

void msm8xxx_handle_interrupt(void *context)
{
    uint32_t irq_no = HwReg(gGICCPUBase + GIC_CPU_INTACK);
    if (irq_no > NR_IRQS) {
        kprintf(KPRINTF_PREFIX "bogus IRQ %u\n", irq_no);
        return;
    }
    if (irq_no == INT_DEBUG_TIMER_EXP) {
        msm8xxx_timer_enabled(FALSE);
        clock_absolute_time += (clock_decrementer - (int64_t)msm8xxx_timer_value());
        rtclock_intr((arm_saved_state_t *)context);
        msm8xxx_timer_enabled(TRUE);
        clock_had_irq = TRUE;
    } else {
        irq_iokit_dispatch(irq_no);
    }
    HwReg(gGICCPUBase + GIC_CPU_EOI) = irq_no;
}

uint64_t msm8xxx_get_timebase(void)
{
    uint32_t timestamp;
    if (!clock_initialized)
        return 0;
    timestamp = msm8xxx_timer_value();
    if (timestamp) {
        uint64_t v = clock_absolute_time;
        v += (uint64_t)((uint64_t)clock_decrementer - (uint64_t)timestamp);
        return v;
    } else {
        clock_absolute_time += clock_decrementer;
        return clock_absolute_time;
    }
}

/*
 * Try ARM Generic Timer (CP15) first; if unavailable, fall back to
 * MSM DGT timer. We detect ARMv7+ by checking the architecture.
 */
uint64_t msm8xxx_timer_value(void)
{
    uint64_t ret = 0;
    uint32_t arch;

    __asm__ volatile("mrc p15, 0, %0, c0, c1, 0" : "=r"(arch));
    if ((arch >> 16) & 0xF) {
        __asm__ volatile("mrrc p15, 0, %Q0, %R0, c14" : "=r"(ret));
    }
    return ret;
}

void msm8xxx_timer_enabled(int enable)
{
    uint32_t ctrl;
    __asm__ volatile("mrc p15, 0, %0, c14, c2, 1" : "=r"(ctrl));
    if (enable)
        ctrl |= 1;
    else
        ctrl &= ~1;
    __asm__ volatile("mcr p15, 0, %0, c14, c2, 1" : : "r"(ctrl));
}

void vcputc(__unused int l, __unused int u, int c);

static void _fb_putc(int c)
{
    if (c == '\n')
        _fb_putc('\r');
    vcputc(0, 0, c);
}

void msm8xxx_framebuffer_init(void)
{
    void *framebuffer = pmap_steal_memory(1024 * 768 * 4);
    void *framebuffer_phys = pmap_extract(kernel_pmap, framebuffer);
    uint32_t depth = 4, width = 1024, height = 768, pitch = width * depth;

    PE_state.video.v_baseAddr = (unsigned long)framebuffer_phys;
    PE_state.video.v_rowBytes = width * 2;
    PE_state.video.v_width = width;
    PE_state.video.v_height = height;
    PE_state.video.v_depth = 2 * 8;

    bzero(framebuffer, pitch * height);
    initialize_screen((void *)&PE_state.video, kPETextMode);
}

int msm8xxx_halt_restart(int type)
{
    return 0;
}

void PE_init_SocSupport_msm8xxx(void)
{
    gPESocDispatch.uart_getc = msm8xxx_getc;
    gPESocDispatch.uart_putc = msm8xxx_putc;
    gPESocDispatch.uart_init = msm8xxx_uart_init;
    gPESocDispatch.interrupt_init = msm8xxx_interrupt_init;
    gPESocDispatch.timebase_init = msm8xxx_timebase_init;
    gPESocDispatch.get_timebase = msm8xxx_get_timebase;
    gPESocDispatch.handle_interrupt = msm8xxx_handle_interrupt;
    gPESocDispatch.timer_value = msm8xxx_timer_value;
    gPESocDispatch.timer_enabled = msm8xxx_timer_enabled;
    gPESocDispatch.framebuffer_init = msm8xxx_framebuffer_init;

    msm8xxx_uart_init();
    msm8xxx_framebuffer_init();
    PE_halt_restart = msm8xxx_halt_restart;
}

void PE_init_SocSupport_stub(void)
{
    PE_early_puts("PE_init_SocSupport: Initializing for generic Qualcomm MSM8xxx\n");
    kprintf(KPRINTF_PREFIX "GIC dist 0x%x CPU 0x%x UART 0x%x\n",
            MSM8XXX_GIC_DIST_PADDR, MSM8XXX_GIC_CPU_PADDR, MSM8XXX_UART_PADDR);
    PE_init_SocSupport_msm8xxx();
}

#endif /* BOARD_CONFIG_MSM8XXX */
