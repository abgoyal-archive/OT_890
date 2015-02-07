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
 *  linux/arch/arm/mach-realview/hotplug.c
 *
 *  Copyright (C) 2002 ARM Ltd.
 *  All Rights Reserved
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/smp.h>
#include <linux/completion.h>

#include <asm/cacheflush.h>

extern volatile int pen_release;

static DECLARE_COMPLETION(cpu_killed);

static inline void cpu_enter_lowpower(void)
{
	unsigned int v;

	flush_cache_all();
	asm volatile(
	"	mcr	p15, 0, %1, c7, c5, 0\n"
	"	mcr	p15, 0, %1, c7, c10, 4\n"
	/*
	 * Turn off coherency
	 */
	"	mrc	p15, 0, %0, c1, c0, 1\n"
	"	bic	%0, %0, #0x20\n"
	"	mcr	p15, 0, %0, c1, c0, 1\n"
	"	mrc	p15, 0, %0, c1, c0, 0\n"
	"	bic	%0, %0, #0x04\n"
	"	mcr	p15, 0, %0, c1, c0, 0\n"
	  : "=&r" (v)
	  : "r" (0)
	  : "cc");
}

static inline void cpu_leave_lowpower(void)
{
	unsigned int v;

	asm volatile(	"mrc	p15, 0, %0, c1, c0, 0\n"
	"	orr	%0, %0, #0x04\n"
	"	mcr	p15, 0, %0, c1, c0, 0\n"
	"	mrc	p15, 0, %0, c1, c0, 1\n"
	"	orr	%0, %0, #0x20\n"
	"	mcr	p15, 0, %0, c1, c0, 1\n"
	  : "=&r" (v)
	  :
	  : "cc");
}

static inline void platform_do_lowpower(unsigned int cpu)
{
	/*
	 * there is no power-control hardware on this platform, so all
	 * we can do is put the core into WFI; this is safe as the calling
	 * code will have already disabled interrupts
	 */
	for (;;) {
		/*
		 * here's the WFI
		 */
		asm(".word	0xe320f003\n"
		    :
		    :
		    : "memory", "cc");

		if (pen_release == cpu) {
			/*
			 * OK, proper wakeup, we're done
			 */
			break;
		}

		/*
		 * getting here, means that we have come out of WFI without
		 * having been woken up - this shouldn't happen
		 *
		 * The trouble is, letting people know about this is not really
		 * possible, since we are currently running incoherently, and
		 * therefore cannot safely call printk() or anything else
		 */
#ifdef DEBUG
		printk("CPU%u: spurious wakeup call\n", cpu);
#endif
	}
}

int platform_cpu_kill(unsigned int cpu)
{
	return wait_for_completion_timeout(&cpu_killed, 5000);
}

/*
 * platform-specific code to shutdown a CPU
 *
 * Called with IRQs disabled
 */
void platform_cpu_die(unsigned int cpu)
{
#ifdef DEBUG
	unsigned int this_cpu = hard_smp_processor_id();

	if (cpu != this_cpu) {
		printk(KERN_CRIT "Eek! platform_cpu_die running on %u, should be %u\n",
			   this_cpu, cpu);
		BUG();
	}
#endif

	printk(KERN_NOTICE "CPU%u: shutdown\n", cpu);
	complete(&cpu_killed);

	/*
	 * we're ready for shutdown now, so do it
	 */
	cpu_enter_lowpower();
	platform_do_lowpower(cpu);

	/*
	 * bring this CPU back into the world of cache
	 * coherency, and then restore interrupts
	 */
	cpu_leave_lowpower();
}

int mach_cpu_disable(unsigned int cpu)
{
	/*
	 * we don't allow CPU 0 to be shutdown (it is still too special
	 * e.g. clock tick interrupts)
	 */
	return cpu == 0 ? -EPERM : 0;
}
