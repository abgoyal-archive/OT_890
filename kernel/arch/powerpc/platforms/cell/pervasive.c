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
 * CBE Pervasive Monitor and Debug
 *
 * (C) Copyright IBM Corporation 2005
 *
 * Authors: Maximino Aguilar (maguilar@us.ibm.com)
 *          Michael N. Day (mnday@us.ibm.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#undef DEBUG

#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/percpu.h>
#include <linux/types.h>
#include <linux/kallsyms.h>

#include <asm/io.h>
#include <asm/machdep.h>
#include <asm/prom.h>
#include <asm/pgtable.h>
#include <asm/reg.h>
#include <asm/cell-regs.h>

#include "pervasive.h"

static void cbe_power_save(void)
{
	unsigned long ctrl, thread_switch_control;

	/*
	 * We need to hard disable interrupts, the local_irq_enable() done by
	 * our caller upon return will hard re-enable.
	 */
	hard_irq_disable();

	ctrl = mfspr(SPRN_CTRLF);

	/* Enable DEC and EE interrupt request */
	thread_switch_control  = mfspr(SPRN_TSC_CELL);
	thread_switch_control |= TSC_CELL_EE_ENABLE | TSC_CELL_EE_BOOST;

	switch (ctrl & CTRL_CT) {
	case CTRL_CT0:
		thread_switch_control |= TSC_CELL_DEC_ENABLE_0;
		break;
	case CTRL_CT1:
		thread_switch_control |= TSC_CELL_DEC_ENABLE_1;
		break;
	default:
		printk(KERN_WARNING "%s: unknown configuration\n",
			__func__);
		break;
	}
	mtspr(SPRN_TSC_CELL, thread_switch_control);

	/*
	 * go into low thread priority, medium priority will be
	 * restored for us after wake-up.
	 */
	HMT_low();

	/*
	 * atomically disable thread execution and runlatch.
	 * External and Decrementer exceptions are still handled when the
	 * thread is disabled but now enter in cbe_system_reset_exception()
	 */
	ctrl &= ~(CTRL_RUNLATCH | CTRL_TE);
	mtspr(SPRN_CTRLT, ctrl);
}

static int cbe_system_reset_exception(struct pt_regs *regs)
{
	switch (regs->msr & SRR1_WAKEMASK) {
	case SRR1_WAKEEE:
		do_IRQ(regs);
		break;
	case SRR1_WAKEDEC:
		timer_interrupt(regs);
		break;
	case SRR1_WAKEMT:
		return cbe_sysreset_hack();
#ifdef CONFIG_CBE_RAS
	case SRR1_WAKESYSERR:
		cbe_system_error_exception(regs);
		break;
	case SRR1_WAKETHERM:
		cbe_thermal_exception(regs);
		break;
#endif /* CONFIG_CBE_RAS */
	default:
		/* do system reset */
		return 0;
	}
	/* everything handled */
	return 1;
}

void __init cbe_pervasive_init(void)
{
	int cpu;

	if (!cpu_has_feature(CPU_FTR_PAUSE_ZERO))
		return;

	for_each_possible_cpu(cpu) {
		struct cbe_pmd_regs __iomem *regs = cbe_get_cpu_pmd_regs(cpu);
		if (!regs)
			continue;

		 /* Enable Pause(0) control bit */
		out_be64(&regs->pmcr, in_be64(&regs->pmcr) |
					    CBE_PMD_PAUSE_ZERO_CONTROL);
	}

	ppc_md.power_save = cbe_power_save;
	ppc_md.system_reset_exception = cbe_system_reset_exception;
}
