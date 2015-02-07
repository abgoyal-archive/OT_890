/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 */

/*
 * linux/arch/arm/kernel/xscale-cp0.c
 *
 * XScale DSP and iWMMXt coprocessor context switching and handling
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/signal.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/io.h>
#include <asm/thread_notify.h>

static inline void dsp_save_state(u32 *state)
{
	__asm__ __volatile__ (
		"mrrc	p0, 0, %0, %1, c0\n"
		: "=r" (state[0]), "=r" (state[1]));
}

static inline void dsp_load_state(u32 *state)
{
	__asm__ __volatile__ (
		"mcrr	p0, 0, %0, %1, c0\n"
		: : "r" (state[0]), "r" (state[1]));
}

static int dsp_do(struct notifier_block *self, unsigned long cmd, void *t)
{
	struct thread_info *thread = t;

	switch (cmd) {
	case THREAD_NOTIFY_FLUSH:
		thread->cpu_context.extra[0] = 0;
		thread->cpu_context.extra[1] = 0;
		break;

	case THREAD_NOTIFY_SWITCH:
		dsp_save_state(current_thread_info()->cpu_context.extra);
		dsp_load_state(thread->cpu_context.extra);
		break;
	}

	return NOTIFY_DONE;
}

static struct notifier_block dsp_notifier_block = {
	.notifier_call	= dsp_do,
};


#ifdef CONFIG_IWMMXT
static int iwmmxt_do(struct notifier_block *self, unsigned long cmd, void *t)
{
	struct thread_info *thread = t;

	switch (cmd) {
	case THREAD_NOTIFY_FLUSH:
		/*
		 * flush_thread() zeroes thread->fpstate, so no need
		 * to do anything here.
		 *
		 * FALLTHROUGH: Ensure we don't try to overwrite our newly
		 * initialised state information on the first fault.
		 */

	case THREAD_NOTIFY_RELEASE:
		iwmmxt_task_release(thread);
		break;

	case THREAD_NOTIFY_SWITCH:
		iwmmxt_task_switch(thread);
		break;
	}

	return NOTIFY_DONE;
}

static struct notifier_block iwmmxt_notifier_block = {
	.notifier_call	= iwmmxt_do,
};
#endif


static u32 __init xscale_cp_access_read(void)
{
	u32 value;

	__asm__ __volatile__ (
		"mrc	p15, 0, %0, c15, c1, 0\n\t"
		: "=r" (value));

	return value;
}

static void __init xscale_cp_access_write(u32 value)
{
	u32 temp;

	__asm__ __volatile__ (
		"mcr	p15, 0, %1, c15, c1, 0\n\t"
		"mrc	p15, 0, %0, c15, c1, 0\n\t"
		"mov	%0, %0\n\t"
		"sub	pc, pc, #4\n\t"
		: "=r" (temp) : "r" (value));
}

/*
 * Detect whether we have a MAC coprocessor (40 bit register) or an
 * iWMMXt coprocessor (64 bit registers) by loading 00000100:00000000
 * into a coprocessor register and reading it back, and checking
 * whether the upper word survived intact.
 */
static int __init cpu_has_iwmmxt(void)
{
	u32 lo;
	u32 hi;

	/*
	 * This sequence is interpreted by the DSP coprocessor as:
	 *	mar	acc0, %2, %3
	 *	mra	%0, %1, acc0
	 *
	 * And by the iWMMXt coprocessor as:
	 *	tmcrr	wR0, %2, %3
	 *	tmrrc	%0, %1, wR0
	 */
	__asm__ __volatile__ (
		"mcrr	p0, 0, %2, %3, c0\n"
		"mrrc	p0, 0, %0, %1, c0\n"
		: "=r" (lo), "=r" (hi)
		: "r" (0), "r" (0x100));

	return !!hi;
}


/*
 * If we detect that the CPU has iWMMXt (and CONFIG_IWMMXT=y), we
 * disable CP0/CP1 on boot, and let call_fpe() and the iWMMXt lazy
 * switch code handle iWMMXt context switching.  If on the other
 * hand the CPU has a DSP coprocessor, we keep access to CP0 enabled
 * all the time, and save/restore acc0 on context switch in non-lazy
 * fashion.
 */
static int __init xscale_cp0_init(void)
{
	u32 cp_access;

	cp_access = xscale_cp_access_read() & ~3;
	xscale_cp_access_write(cp_access | 1);

	if (cpu_has_iwmmxt()) {
#ifndef CONFIG_IWMMXT
		printk(KERN_WARNING "CAUTION: XScale iWMMXt coprocessor "
			"detected, but kernel support is missing.\n");
#else
		printk(KERN_INFO "XScale iWMMXt coprocessor detected.\n");
		elf_hwcap |= HWCAP_IWMMXT;
		thread_register_notifier(&iwmmxt_notifier_block);
#endif
	} else {
		printk(KERN_INFO "XScale DSP coprocessor detected.\n");
		thread_register_notifier(&dsp_notifier_block);
		cp_access |= 1;
	}

	xscale_cp_access_write(cp_access);

	return 0;
}

late_initcall(xscale_cp0_init);
