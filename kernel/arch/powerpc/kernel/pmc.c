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
 *  arch/powerpc/kernel/pmc.c
 *
 *  Copyright (C) 2004 David Gibson, IBM Corporation.
 *  Includes code formerly from arch/ppc/kernel/perfmon.c:
 *    Author: Andy Fleming
 *    Copyright (c) 2004 Freescale Semiconductor, Inc
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version
 *  2 of the License, or (at your option) any later version.
 */

#include <linux/errno.h>
#include <linux/spinlock.h>
#include <linux/module.h>

#include <asm/processor.h>
#include <asm/cputable.h>
#include <asm/pmc.h>

#ifndef MMCR0_PMAO
#define MMCR0_PMAO	0
#endif

static void dummy_perf(struct pt_regs *regs)
{
#if defined(CONFIG_FSL_EMB_PERFMON)
	mtpmr(PMRN_PMGC0, mfpmr(PMRN_PMGC0) & ~PMGC0_PMIE);
#elif defined(CONFIG_PPC64) || defined(CONFIG_6xx)
	if (cur_cpu_spec->pmc_type == PPC_PMC_IBM)
		mtspr(SPRN_MMCR0, mfspr(SPRN_MMCR0) & ~(MMCR0_PMXE|MMCR0_PMAO));
#else
	mtspr(SPRN_MMCR0, mfspr(SPRN_MMCR0) & ~MMCR0_PMXE);
#endif
}


static DEFINE_SPINLOCK(pmc_owner_lock);
static void *pmc_owner_caller; /* mostly for debugging */
perf_irq_t perf_irq = dummy_perf;

int reserve_pmc_hardware(perf_irq_t new_perf_irq)
{
	int err = 0;

	spin_lock(&pmc_owner_lock);

	if (pmc_owner_caller) {
		printk(KERN_WARNING "reserve_pmc_hardware: "
		       "PMC hardware busy (reserved by caller %p)\n",
		       pmc_owner_caller);
		err = -EBUSY;
		goto out;
	}

	pmc_owner_caller = __builtin_return_address(0);
	perf_irq = new_perf_irq ? new_perf_irq : dummy_perf;

 out:
	spin_unlock(&pmc_owner_lock);
	return err;
}
EXPORT_SYMBOL_GPL(reserve_pmc_hardware);

void release_pmc_hardware(void)
{
	spin_lock(&pmc_owner_lock);

	WARN_ON(! pmc_owner_caller);

	pmc_owner_caller = NULL;
	perf_irq = dummy_perf;

	spin_unlock(&pmc_owner_lock);
}
EXPORT_SYMBOL_GPL(release_pmc_hardware);

#ifdef CONFIG_PPC64
void power4_enable_pmcs(void)
{
	unsigned long hid0;

	hid0 = mfspr(SPRN_HID0);
	hid0 |= 1UL << (63 - 20);

	/* POWER4 requires the following sequence */
	asm volatile(
		"sync\n"
		"mtspr     %1, %0\n"
		"mfspr     %0, %1\n"
		"mfspr     %0, %1\n"
		"mfspr     %0, %1\n"
		"mfspr     %0, %1\n"
		"mfspr     %0, %1\n"
		"mfspr     %0, %1\n"
		"isync" : "=&r" (hid0) : "i" (SPRN_HID0), "0" (hid0):
		"memory");
}
#endif /* CONFIG_PPC64 */
