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
 * Platform Expert for Qualcomm MSM8916 (Snapdragon 410).
 *
 * Supports the Motorola Harpia (Moto G4 Play) and similar MSM8916 devices.
 */

#if defined(ARM_BOARD_CONFIG_MSM8916_HARPIA)

#define PE_INIT_SOC_SUPPORT_STUB_ALREADY_DEFINED 1

#include <mach/mach_types.h>

#include <pexpert/pexpert.h>
#include <pexpert/arm/protos.h>
#include <pexpert/arm/boot.h>

#include <machine/machine_routines.h>

#include <vm/pmap.h>
#include <arm/pmap.h>

#include "pe_msm8916.h"

#define KPRINTF_PREFIX  "PE_MSM8916: "

#define HwReg(x) *((volatile unsigned long *)(x))

extern void rtclock_intr(arm_saved_state_t *regs);
extern void rtc_configure(uint64_t hz);

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
    kprintf(KPRINTF_PREFIX "decrementer frequency = %llu\n", clock_decrementer);

    rtc_configure(hz);
}

void msm8916_putc(int c)
{
    if (!gUARTBase)
        return;

    if (c == '\n')
        msm8916_putc('\r');

    while (!(HwReg(gUARTBase + UART_DM_SR) & UART_DM_SR_TXEMT))
        barrier();

    HwReg(gUARTBase + UART_DM_TF) = c;
}

int msm8916_getc(void)
{
    int c = -1;

    if (!gUARTBase)
        return c;

    if (HwReg(gUARTBase + UART_DM_SR) & UART_DM_SR_RXRDY)
        c = HwReg(gUARTBase + UART_DM_RF) & 0xFF;

    return c;
}

void msm8916_uart_init(void)
{
    gUARTBase = ml_io_map(MSM8916_BLSP1_UART2_BASE, 0x1000);
    assert(gUARTBase);

    kprintf(KPRINTF_PREFIX "UART base at %p\n", (void *)gUARTBase);
}

static void qgic_dist_init(void)
{
    uint32_t i;
    uint32_t num_irq = 0;
    uint32_t cpumask = 1;

    cpumask |= cpumask << 8;
    cpumask |= cpumask << 16;

    HwReg(gGICDistBase + GIC_DIST_CTRL) = 0;

    num_irq = HwReg(gGICDistBase + GIC_DIST_CTR) & 0x1f;
    num_irq = (num_irq + 1) * 32;

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

void msm8916_interrupt_init(void)
{
    gGICDistBase = ml_io_map(MSM8916_GIC_DIST_BASE, 0x1000);
    gGICCPUBase = ml_io_map(MSM8916_GIC_CPU_BASE, 0x1000);

    assert(gGICDistBase && gGICCPUBase);

    qgic_dist_init();
    qgic_cpu_init();
}

uint64_t msm8916_timer_value(void);
void msm8916_timer_enabled(int enable);

void msm8916_timebase_init(void)
{
    timer_configure();

    msm8916_timer_enabled(FALSE);

    uint32_t reg = GIC_DIST_ENABLE_SET + (INT_DEBUG_TIMER_EXP / 32) * 4;
    uint32_t bit = 1 << (INT_DEBUG_TIMER_EXP & 31);
    HwReg(gGICDistBase + reg) = bit;

    ml_set_interrupts_enabled(TRUE);

    msm8916_timer_enabled(TRUE);

    clock_initialized = TRUE;
    while (!clock_had_irq)
        barrier();
}

void msm8916_handle_interrupt(void *context)
{
    uint32_t irq_no = HwReg(gGICCPUBase + GIC_CPU_INTACK);

    if (irq_no > NR_IRQS) {
        kprintf(KPRINTF_PREFIX "Got a bogus IRQ?\n");
        return;
    }

    if (irq_no == INT_DEBUG_TIMER_EXP) {
        msm8916_timer_enabled(FALSE);
        clock_absolute_time += (clock_decrementer - (int64_t)msm8916_timer_value());
        rtclock_intr((arm_saved_state_t *)context);
        msm8916_timer_enabled(TRUE);
        clock_had_irq = TRUE;
    } else {
        irq_iokit_dispatch(irq_no);
    }

    HwReg(gGICCPUBase + GIC_CPU_EOI) = irq_no;
}

uint64_t msm8916_get_timebase(void)
{
    uint32_t timestamp;

    if (!clock_initialized)
        return 0;

    timestamp = msm8916_timer_value();

    if (timestamp) {
        uint64_t v = clock_absolute_time;
        v += (uint64_t)(((uint64_t)clock_decrementer) - (uint64_t)(timestamp));
        return v;
    } else {
        clock_absolute_time += clock_decrementer;
        return clock_absolute_time;
    }
}

uint64_t msm8916_timer_value(void)
{
    uint64_t ret;
    __asm__ volatile("mrrc p15, 0, %Q0, %R0, c14" : "=r"(ret));
    return ret;
}

void msm8916_timer_enabled(int enable)
{
    uint32_t ctrl;
    __asm__ volatile("mrc p15, 0, %0, c14, c2, 1" : "=r"(ctrl));
    if (enable)
        ctrl |= (1 << 0);
    else
        ctrl &= ~(1 << 0);
    __asm__ volatile("mcr p15, 0, %0, c14, c2, 1" : : "r"(ctrl));
}

void vcputc(__unused int l, __unused int u, int c);

static void _fb_putc(int c)
{
    if (c == '\n')
        _fb_putc('\r');
    vcputc(0, 0, c);
}

void msm8916_framebuffer_init(void)
{
    /* Framebuffer initialization is deferred until after VM is ready.
     * Called after pmap_bootstrap() in pe_init.c. For early boot,
     * we only use UART for console output.
     *
     * If framebuffer properties are passed in boot args by the bootloader,
     * use those. Otherwise, allocate a framebuffer from available memory.
     */
    if (PE_state.video.v_baseAddr == 0) {
        /* No framebuffer provided by bootloader, skip allocation for now */
        kprintf(KPRINTF_PREFIX "framebuffer: using UART console only\n");
        return;
    }

    /* Framebuffer already configured by bootloader */
    kprintf(KPRINTF_PREFIX "framebuffer: base=0x%lx width=%lu height=%lu depth=%lu\n",
        PE_state.video.v_baseAddr, PE_state.video.v_width,
        PE_state.video.v_height, PE_state.video.v_depth);
}

int msm8916_halt_restart(int type)
{
    return 0;
}

void PE_init_SocSupport_msm8916(void)
{
    gPESocDispatch.uart_getc = msm8916_getc;
    gPESocDispatch.uart_putc = msm8916_putc;
    gPESocDispatch.uart_init = msm8916_uart_init;

    gPESocDispatch.interrupt_init = msm8916_interrupt_init;
    gPESocDispatch.timebase_init = msm8916_timebase_init;

    gPESocDispatch.get_timebase = msm8916_get_timebase;

    gPESocDispatch.handle_interrupt = msm8916_handle_interrupt;

    gPESocDispatch.timer_value = msm8916_timer_value;
    gPESocDispatch.timer_enabled = msm8916_timer_enabled;

    gPESocDispatch.framebuffer_init = msm8916_framebuffer_init;

    msm8916_uart_init();
    msm8916_framebuffer_init();

    PE_halt_restart = msm8916_halt_restart;
}

void PE_init_SocSupport_stub(void)
{
    PE_early_puts("PE_init_SocSupport: Initializing for Qualcomm MSM8916\n");
    PE_init_SocSupport_msm8916();
}

#endif
