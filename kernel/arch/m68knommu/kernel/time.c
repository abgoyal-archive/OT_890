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
 *  linux/arch/m68knommu/kernel/time.c
 *
 *  Copyright (C) 1991, 1992, 1995  Linus Torvalds
 *
 * This file contains the m68k-specific time handling details.
 * Most of the stuff is located in the machine specific files.
 *
 * 1997-09-10	Updated NTP code according to technical memorandum Jan '96
 *		"A Kernel Model for Precision Timekeeping" by Dave Mills
 */

#include <linux/errno.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/param.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/profile.h>
#include <linux/time.h>
#include <linux/timex.h>

#include <asm/machdep.h>
#include <asm/irq_regs.h>

#define	TICK_SIZE (tick_nsec / 1000)

static inline int set_rtc_mmss(unsigned long nowtime)
{
	if (mach_set_clock_mmss)
		return mach_set_clock_mmss (nowtime);
	return -1;
}

#ifndef CONFIG_GENERIC_CLOCKEVENTS
/*
 * timer_interrupt() needs to keep up the real-time clock,
 * as well as call the "do_timer()" routine every clocktick
 */
irqreturn_t arch_timer_interrupt(int irq, void *dummy)
{

	if (current->pid)
		profile_tick(CPU_PROFILING);

	write_seqlock(&xtime_lock);

	do_timer(1);

	write_sequnlock(&xtime_lock);

#ifndef CONFIG_SMP
	update_process_times(user_mode(get_irq_regs()));
#endif
	return(IRQ_HANDLED);
}
#endif

static unsigned long read_rtc_mmss(void)
{
	unsigned int year, mon, day, hour, min, sec;

	if (mach_gettod)
		mach_gettod(&year, &mon, &day, &hour, &min, &sec);
	else
		year = mon = day = hour = min = sec = 0;

	if ((year += 1900) < 1970)
		year += 100;

	return  mktime(year, mon, day, hour, min, sec);;
}

unsigned long read_persistent_clock(void)
{
	return read_rtc_mmss();
}

int update_persistent_clock(struct timespec now)
{
	return set_rtc_mmss(now.tv_sec);
}

void time_init(void)
{
	hw_timer_init();
}
