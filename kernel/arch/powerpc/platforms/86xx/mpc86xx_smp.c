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
 * Author: Xianghua Xiao <x.xiao@freescale.com>
 *         Zhang Wei <wei.zhang@freescale.com>
 *
 * Copyright 2006 Freescale Semiconductor Inc.
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */

#include <linux/stddef.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/delay.h>

#include <asm/code-patching.h>
#include <asm/page.h>
#include <asm/pgtable.h>
#include <asm/pci-bridge.h>
#include <asm/mpic.h>
#include <asm/mpc86xx.h>
#include <asm/cacheflush.h>

#include <sysdev/fsl_soc.h>

#include "mpc86xx.h"

extern void __secondary_start_mpc86xx(void);
extern unsigned long __secondary_hold_acknowledge;


static void __init
smp_86xx_release_core(int nr)
{
	__be32 __iomem *mcm_vaddr;
	unsigned long pcr;

	if (nr < 0 || nr >= NR_CPUS)
		return;

	/*
	 * Startup Core #nr.
	 */
	mcm_vaddr = ioremap(get_immrbase() + MPC86xx_MCM_OFFSET,
			    MPC86xx_MCM_SIZE);
	pcr = in_be32(mcm_vaddr + (MCM_PORT_CONFIG_OFFSET >> 2));
	pcr |= 1 << (nr + 24);
	out_be32(mcm_vaddr + (MCM_PORT_CONFIG_OFFSET >> 2), pcr);
}


static void __init
smp_86xx_kick_cpu(int nr)
{
	unsigned int save_vector;
	unsigned long target, flags;
	int n = 0;
	unsigned int *vector = (unsigned int *)(KERNELBASE + 0x100);

	if (nr < 0 || nr >= NR_CPUS)
		return;

	pr_debug("smp_86xx_kick_cpu: kick CPU #%d\n", nr);

	local_irq_save(flags);

	/* Save reset vector */
	save_vector = *vector;

	/* Setup fake reset vector to call __secondary_start_mpc86xx. */
	target = (unsigned long) __secondary_start_mpc86xx;
	patch_branch(vector, target, BRANCH_SET_LINK);

	/* Kick that CPU */
	smp_86xx_release_core(nr);

	/* Wait a bit for the CPU to take the exception. */
	while ((__secondary_hold_acknowledge != nr) && (n++, n < 1000))
		mdelay(1);

	/* Restore the exception vector */
	*vector = save_vector;
	flush_icache_range((unsigned long) vector, (unsigned long) vector + 4);

	local_irq_restore(flags);

	pr_debug("wait CPU #%d for %d msecs.\n", nr, n);
}


static void __init
smp_86xx_setup_cpu(int cpu_nr)
{
	mpic_setup_this_cpu();
}


struct smp_ops_t smp_86xx_ops = {
	.message_pass = smp_mpic_message_pass,
	.probe = smp_mpic_probe,
	.kick_cpu = smp_86xx_kick_cpu,
	.setup_cpu = smp_86xx_setup_cpu,
	.take_timebase = smp_generic_take_timebase,
	.give_timebase = smp_generic_give_timebase,
};


void __init
mpc86xx_smp_init(void)
{
	smp_ops = &smp_86xx_ops;
}
