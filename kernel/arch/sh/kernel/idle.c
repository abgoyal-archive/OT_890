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
 * The idle loop for all SuperH platforms.
 *
 *  Copyright (C) 2002 - 2008  Paul Mundt
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */
#include <linux/module.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/pm.h>
#include <linux/tick.h>
#include <linux/preempt.h>
#include <linux/thread_info.h>
#include <linux/irqflags.h>
#include <asm/pgalloc.h>
#include <asm/system.h>
#include <asm/atomic.h>

static int hlt_counter;
void (*pm_idle)(void);
void (*pm_power_off)(void);
EXPORT_SYMBOL(pm_power_off);

static int __init nohlt_setup(char *__unused)
{
	hlt_counter = 1;
	return 1;
}
__setup("nohlt", nohlt_setup);

static int __init hlt_setup(char *__unused)
{
	hlt_counter = 0;
	return 1;
}
__setup("hlt", hlt_setup);

static void default_idle(void)
{
	if (!hlt_counter) {
		clear_thread_flag(TIF_POLLING_NRFLAG);
		smp_mb__after_clear_bit();
		set_bl_bit();
		stop_critical_timings();

		while (!need_resched())
			cpu_sleep();

		start_critical_timings();
		clear_bl_bit();
		set_thread_flag(TIF_POLLING_NRFLAG);
	} else
		while (!need_resched())
			cpu_relax();
}

void cpu_idle(void)
{
	set_thread_flag(TIF_POLLING_NRFLAG);

	/* endless idle loop with no priority at all */
	while (1) {
		void (*idle)(void) = pm_idle;

		if (!idle)
			idle = default_idle;

		tick_nohz_stop_sched_tick(1);
		while (!need_resched())
			idle();
		tick_nohz_restart_sched_tick();

		preempt_enable_no_resched();
		schedule();
		preempt_disable();
		check_pgt_cache();
	}
}
