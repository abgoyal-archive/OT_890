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
 * mce.c - x86 Machine Check Exception Reporting
 * (c) 2002 Alan Cox <alan@lxorguk.ukuu.org.uk>, Dave Jones <davej@redhat.com>
 */

#include <linux/init.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/smp.h>
#include <linux/thread_info.h>

#include <asm/processor.h>
#include <asm/system.h>
#include <asm/mce.h>

#include "mce.h"

int mce_disabled;
int nr_mce_banks;

EXPORT_SYMBOL_GPL(nr_mce_banks);	/* non-fatal.o */

/* Handle unconfigured int18 (should never happen) */
static void unexpected_machine_check(struct pt_regs *regs, long error_code)
{
	printk(KERN_ERR "CPU#%d: Unexpected int18 (Machine Check).\n", smp_processor_id());
}

/* Call the installed machine check handler for this CPU setup. */
void (*machine_check_vector)(struct pt_regs *, long error_code) = unexpected_machine_check;

/* This has to be run for each processor */
void mcheck_init(struct cpuinfo_x86 *c)
{
	if (mce_disabled == 1)
		return;

	switch (c->x86_vendor) {
	case X86_VENDOR_AMD:
		amd_mcheck_init(c);
		break;

	case X86_VENDOR_INTEL:
		if (c->x86 == 5)
			intel_p5_mcheck_init(c);
		if (c->x86 == 6)
			intel_p6_mcheck_init(c);
		if (c->x86 == 15)
			intel_p4_mcheck_init(c);
		break;

	case X86_VENDOR_CENTAUR:
		if (c->x86 == 5)
			winchip_mcheck_init(c);
		break;

	default:
		break;
	}
}

static unsigned long old_cr4 __initdata;

void __init stop_mce(void)
{
	old_cr4 = read_cr4();
	clear_in_cr4(X86_CR4_MCE);
}

void __init restart_mce(void)
{
	if (old_cr4 & X86_CR4_MCE)
		set_in_cr4(X86_CR4_MCE);
}

static int __init mcheck_disable(char *str)
{
	mce_disabled = 1;
	return 1;
}

static int __init mcheck_enable(char *str)
{
	mce_disabled = -1;
	return 1;
}

__setup("nomce", mcheck_disable);
__setup("mce", mcheck_enable);
