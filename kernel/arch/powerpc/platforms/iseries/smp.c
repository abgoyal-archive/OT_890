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
 * SMP support for iSeries machines.
 *
 * Dave Engebretsen, Peter Bergner, and
 * Mike Corrigan {engebret|bergner|mikec}@us.ibm.com
 *
 * Plus various changes from other IBM teams...
 *
 *      This program is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU General Public License
 *      as published by the Free Software Foundation; either version
 *      2 of the License, or (at your option) any later version.
 */

#undef DEBUG

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/smp.h>
#include <linux/interrupt.h>
#include <linux/kernel_stat.h>
#include <linux/delay.h>
#include <linux/init.h>
#include <linux/spinlock.h>
#include <linux/cache.h>
#include <linux/err.h>
#include <linux/sysdev.h>
#include <linux/cpu.h>

#include <asm/ptrace.h>
#include <asm/atomic.h>
#include <asm/irq.h>
#include <asm/page.h>
#include <asm/pgtable.h>
#include <asm/io.h>
#include <asm/smp.h>
#include <asm/paca.h>
#include <asm/iseries/hv_call.h>
#include <asm/time.h>
#include <asm/machdep.h>
#include <asm/cputable.h>
#include <asm/system.h>

#include "smp.h"

static unsigned long iSeries_smp_message[NR_CPUS];

void iSeries_smp_message_recv(void)
{
	int cpu = smp_processor_id();
	int msg;

	if (num_online_cpus() < 2)
		return;

	for (msg = 0; msg < 4; msg++)
		if (test_and_clear_bit(msg, &iSeries_smp_message[cpu]))
			smp_message_recv(msg);
}

static inline void smp_iSeries_do_message(int cpu, int msg)
{
	set_bit(msg, &iSeries_smp_message[cpu]);
	HvCall_sendIPI(&(paca[cpu]));
}

static void smp_iSeries_message_pass(int target, int msg)
{
	int i;

	if (target < NR_CPUS)
		smp_iSeries_do_message(target, msg);
	else {
		for_each_online_cpu(i) {
			if ((target == MSG_ALL_BUT_SELF) &&
					(i == smp_processor_id()))
				continue;
			smp_iSeries_do_message(i, msg);
		}
	}
}

static int smp_iSeries_probe(void)
{
	return cpus_weight(cpu_possible_map);
}

static void smp_iSeries_kick_cpu(int nr)
{
	BUG_ON((nr < 0) || (nr >= NR_CPUS));

	/* Verify that our partition has a processor nr */
	if (lppaca[nr].dyn_proc_status >= 2)
		return;

	/* The processor is currently spinning, waiting
	 * for the cpu_start field to become non-zero
	 * After we set cpu_start, the processor will
	 * continue on to secondary_start in iSeries_head.S
	 */
	paca[nr].cpu_start = 1;
}

static void __devinit smp_iSeries_setup_cpu(int nr)
{
}

static struct smp_ops_t iSeries_smp_ops = {
	.message_pass = smp_iSeries_message_pass,
	.probe        = smp_iSeries_probe,
	.kick_cpu     = smp_iSeries_kick_cpu,
	.setup_cpu    = smp_iSeries_setup_cpu,
};

/* This is called very early. */
void __init smp_init_iSeries(void)
{
	smp_ops = &iSeries_smp_ops;
}
