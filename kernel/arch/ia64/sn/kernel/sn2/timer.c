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
 * linux/arch/ia64/sn/kernel/sn2/timer.c
 *
 * Copyright (C) 2003 Silicon Graphics, Inc.
 * Copyright (C) 2003 Hewlett-Packard Co
 *	David Mosberger <davidm@hpl.hp.com>: updated for new timer-interpolation infrastructure
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/time.h>
#include <linux/interrupt.h>
#include <linux/clocksource.h>

#include <asm/hw_irq.h>
#include <asm/system.h>
#include <asm/timex.h>

#include <asm/sn/leds.h>
#include <asm/sn/shub_mmr.h>
#include <asm/sn/clksupport.h>

extern unsigned long sn_rtc_cycles_per_second;

static cycle_t read_sn2(void)
{
	return (cycle_t)readq(RTC_COUNTER_ADDR);
}

static struct clocksource clocksource_sn2 = {
        .name           = "sn2_rtc",
        .rating         = 450,
        .read           = read_sn2,
        .mask           = (1LL << 55) - 1,
        .mult           = 0,
        .shift          = 10,
        .flags          = CLOCK_SOURCE_IS_CONTINUOUS,
};

/*
 * sn udelay uses the RTC instead of the ITC because the ITC is not
 * synchronized across all CPUs, and the thread may migrate to another CPU
 * if preemption is enabled.
 */
static void
ia64_sn_udelay (unsigned long usecs)
{
	unsigned long start = rtc_time();
	unsigned long end = start +
			usecs * sn_rtc_cycles_per_second / 1000000;

	while (time_before((unsigned long)rtc_time(), end))
		cpu_relax();
}

void __init sn_timer_init(void)
{
	clocksource_sn2.fsys_mmio = RTC_COUNTER_ADDR;
	clocksource_sn2.mult = clocksource_hz2mult(sn_rtc_cycles_per_second,
							clocksource_sn2.shift);
	clocksource_register(&clocksource_sn2);

	ia64_udelay = &ia64_sn_udelay;
}
