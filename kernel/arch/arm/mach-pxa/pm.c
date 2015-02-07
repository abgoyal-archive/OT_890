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
 * PXA250/210 Power Management Routines
 *
 * Original code for the SA11x0:
 * Copyright (c) 2001 Cliff Brake <cbrake@accelent.com>
 *
 * Modified for the PXA250 by Nicolas Pitre:
 * Copyright (c) 2002 Monta Vista Software, Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License.
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/suspend.h>
#include <linux/errno.h>
#include <linux/time.h>

#include <mach/hardware.h>
#include <asm/memory.h>
#include <asm/system.h>
#include <mach/pm.h>
#include <mach/pxa-regs.h>
#include <mach/lubbock.h>
#include <asm/mach/time.h>

struct pxa_cpu_pm_fns *pxa_cpu_pm_fns;
static unsigned long *sleep_save;

int pxa_pm_enter(suspend_state_t state)
{
	unsigned long sleep_save_checksum = 0, checksum = 0;
	int i;

#ifdef CONFIG_IWMMXT
	/* force any iWMMXt context to ram **/
	if (elf_hwcap & HWCAP_IWMMXT)
		iwmmxt_task_disable(NULL);
#endif

	/* skip registers saving for standby */
	if (state != PM_SUSPEND_STANDBY) {
		pxa_cpu_pm_fns->save(sleep_save);
		/* before sleeping, calculate and save a checksum */
		for (i = 0; i < pxa_cpu_pm_fns->save_count - 1; i++)
			sleep_save_checksum += sleep_save[i];
	}

	/* *** go zzz *** */
	pxa_cpu_pm_fns->enter(state);
	cpu_init();

	if (state != PM_SUSPEND_STANDBY) {
		/* after sleeping, validate the checksum */
		for (i = 0; i < pxa_cpu_pm_fns->save_count - 1; i++)
			checksum += sleep_save[i];

		/* if invalid, display message and wait for a hardware reset */
		if (checksum != sleep_save_checksum) {
#ifdef CONFIG_ARCH_LUBBOCK
			LUB_HEXLED = 0xbadbadc5;
#endif
			while (1)
				pxa_cpu_pm_fns->enter(state);
		}
		pxa_cpu_pm_fns->restore(sleep_save);
	}

	pr_debug("*** made it back from resume\n");

	return 0;
}

EXPORT_SYMBOL_GPL(pxa_pm_enter);

unsigned long sleep_phys_sp(void *sp)
{
	return virt_to_phys(sp);
}

static int pxa_pm_valid(suspend_state_t state)
{
	if (pxa_cpu_pm_fns)
		return pxa_cpu_pm_fns->valid(state);

	return -EINVAL;
}

static int pxa_pm_prepare(void)
{
	int ret = 0;

	if (pxa_cpu_pm_fns && pxa_cpu_pm_fns->prepare)
		ret = pxa_cpu_pm_fns->prepare();

	return ret;
}

static void pxa_pm_finish(void)
{
	if (pxa_cpu_pm_fns && pxa_cpu_pm_fns->finish)
		pxa_cpu_pm_fns->finish();
}

static struct platform_suspend_ops pxa_pm_ops = {
	.valid		= pxa_pm_valid,
	.enter		= pxa_pm_enter,
	.prepare	= pxa_pm_prepare,
	.finish		= pxa_pm_finish,
};

static int __init pxa_pm_init(void)
{
	if (!pxa_cpu_pm_fns) {
		printk(KERN_ERR "no valid pxa_cpu_pm_fns defined\n");
		return -EINVAL;
	}

	sleep_save = kmalloc(pxa_cpu_pm_fns->save_count * sizeof(unsigned long),
			     GFP_KERNEL);
	if (!sleep_save) {
		printk(KERN_ERR "failed to alloc memory for pm save\n");
		return -ENOMEM;
	}

	suspend_set_ops(&pxa_pm_ops);
	return 0;
}

device_initcall(pxa_pm_init);
