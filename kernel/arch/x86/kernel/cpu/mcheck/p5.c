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
 * P5 specific Machine Check Exception Reporting
 * (C) Copyright 2002 Alan Cox <alan@lxorguk.ukuu.org.uk>
 */

#include <linux/init.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/smp.h>

#include <asm/processor.h>
#include <asm/system.h>
#include <asm/msr.h>

#include "mce.h"

/* Machine check handler for Pentium class Intel */
static void pentium_machine_check(struct pt_regs *regs, long error_code)
{
	u32 loaddr, hi, lotype;
	rdmsr(MSR_IA32_P5_MC_ADDR, loaddr, hi);
	rdmsr(MSR_IA32_P5_MC_TYPE, lotype, hi);
	printk(KERN_EMERG "CPU#%d: Machine Check Exception:  0x%8X (type 0x%8X).\n", smp_processor_id(), loaddr, lotype);
	if (lotype&(1<<5))
		printk(KERN_EMERG "CPU#%d: Possible thermal failure (CPU on fire ?).\n", smp_processor_id());
	add_taint(TAINT_MACHINE_CHECK);
}

/* Set up machine check reporting for processors with Intel style MCE */
void intel_p5_mcheck_init(struct cpuinfo_x86 *c)
{
	u32 l, h;

	/*Check for MCE support */
	if (!cpu_has(c, X86_FEATURE_MCE))
		return;

	/* Default P5 to off as its often misconnected */
	if (mce_disabled != -1)
		return;
	machine_check_vector = pentium_machine_check;
	wmb();

	/* Read registers before enabling */
	rdmsr(MSR_IA32_P5_MC_ADDR, l, h);
	rdmsr(MSR_IA32_P5_MC_TYPE, l, h);
	printk(KERN_INFO "Intel old style machine check architecture supported.\n");

	/* Enable MCE */
	set_in_cr4(X86_CR4_MCE);
	printk(KERN_INFO "Intel old style machine check reporting enabled on CPU#%d.\n", smp_processor_id());
}
