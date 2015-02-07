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
 * Copyright (C) 2006-2007 PA Semi, Inc
 *
 * Maintained by: Olof Johansson <olof@lixom.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 */

#undef DEBUG

#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/irq.h>

#include <asm/machdep.h>
#include <asm/reg.h>
#include <asm/smp.h>

#include "pasemi.h"

struct sleep_mode {
	char *name;
	void (*entry)(void);
};

static struct sleep_mode modes[] = {
	{ .name = "spin", .entry = &idle_spin },
	{ .name = "doze", .entry = &idle_doze },
};

static int current_mode = 0;

static int pasemi_system_reset_exception(struct pt_regs *regs)
{
	/* If we were woken up from power savings, we need to return
	 * to the calling function, since nip is not saved across
	 * all modes.
	 */

	if (regs->msr & SRR1_WAKEMASK)
		regs->nip = regs->link;

	switch (regs->msr & SRR1_WAKEMASK) {
	case SRR1_WAKEEE:
		do_IRQ(regs);
		break;
	case SRR1_WAKEDEC:
		timer_interrupt(regs);
		break;
	default:
		/* do system reset */
		return 0;
	}

	/* Set higher astate since we come out of power savings at 0 */
	restore_astate(hard_smp_processor_id());

	/* everything handled */
	regs->msr |= MSR_RI;
	return 1;
}

static int __init pasemi_idle_init(void)
{
#ifndef CONFIG_PPC_PASEMI_CPUFREQ
	printk(KERN_WARNING "No cpufreq driver, powersavings modes disabled\n");
	current_mode = 0;
#endif

	ppc_md.system_reset_exception = pasemi_system_reset_exception;
	ppc_md.power_save = modes[current_mode].entry;
	printk(KERN_INFO "Using PA6T idle loop (%s)\n", modes[current_mode].name);

	return 0;
}
machine_late_initcall(pasemi, pasemi_idle_init);

static int __init idle_param(char *p)
{
	int i;
	for (i = 0; i < sizeof(modes)/sizeof(struct sleep_mode); i++) {
		if (!strcmp(modes[i].name, p)) {
			current_mode = i;
			break;
		}
	}
	return 0;
}

early_param("idle", idle_param);
