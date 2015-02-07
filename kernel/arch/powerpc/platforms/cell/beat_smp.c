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
 * SMP support for Celleb platform. (Incomplete)
 *
 * (C) Copyright 2006 TOSHIBA CORPORATION
 *
 * This code is based on arch/powerpc/platforms/cell/smp.c:
 * Dave Engebretsen, Peter Bergner, and
 * Mike Corrigan {engebret|bergner|mikec}@us.ibm.com
 * Plus various changes from other IBM teams...
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#undef DEBUG

#include <linux/kernel.h>
#include <linux/smp.h>
#include <linux/interrupt.h>
#include <linux/init.h>
#include <linux/threads.h>
#include <linux/cpu.h>

#include <asm/irq.h>
#include <asm/smp.h>
#include <asm/machdep.h>
#include <asm/udbg.h>

#include "beat_interrupt.h"

#ifdef DEBUG
#define DBG(fmt...) udbg_printf(fmt)
#else
#define DBG(fmt...)
#endif

/*
 * The primary thread of each non-boot processor is recorded here before
 * smp init.
 */
/* static cpumask_t of_spin_map; */

/**
 * smp_startup_cpu() - start the given cpu
 *
 * At boot time, there is nothing to do for primary threads which were
 * started from Open Firmware.  For anything else, call RTAS with the
 * appropriate start location.
 *
 * Returns:
 *	0	- failure
 *	1	- success
 */
static inline int __devinit smp_startup_cpu(unsigned int lcpu)
{
	return 0;
}

static void smp_beatic_message_pass(int target, int msg)
{
	unsigned int i;

	if (target < NR_CPUS) {
		beatic_cause_IPI(target, msg);
	} else {
		for_each_online_cpu(i) {
			if (target == MSG_ALL_BUT_SELF
			    && i == smp_processor_id())
				continue;
			beatic_cause_IPI(i, msg);
		}
	}
}

static int __init smp_beatic_probe(void)
{
	return cpus_weight(cpu_possible_map);
}

static void __devinit smp_beatic_setup_cpu(int cpu)
{
	beatic_setup_cpu(cpu);
}

static void __devinit smp_celleb_kick_cpu(int nr)
{
	BUG_ON(nr < 0 || nr >= NR_CPUS);

	if (!smp_startup_cpu(nr))
		return;
}

static int smp_celleb_cpu_bootable(unsigned int nr)
{
	return 1;
}
static struct smp_ops_t bpa_beatic_smp_ops = {
	.message_pass	= smp_beatic_message_pass,
	.probe		= smp_beatic_probe,
	.kick_cpu	= smp_celleb_kick_cpu,
	.setup_cpu	= smp_beatic_setup_cpu,
	.cpu_bootable	= smp_celleb_cpu_bootable,
};

/* This is called very early */
void __init smp_init_celleb(void)
{
	DBG(" -> smp_init_celleb()\n");

	smp_ops = &bpa_beatic_smp_ops;

	DBG(" <- smp_init_celleb()\n");
}
