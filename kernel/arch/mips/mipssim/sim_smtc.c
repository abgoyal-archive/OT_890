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
 * Copyright (C) 2005 MIPS Technologies, Inc.  All rights reserved.
 *
 *  This program is free software; you can distribute it and/or modify it
 *  under the terms of the GNU General Public License (Version 2) as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place - Suite 330, Boston MA 02111-1307, USA.
 *
 */
/*
 * Simulator Platform-specific hooks for SMTC operation
 */
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/cpumask.h>
#include <linux/interrupt.h>
#include <linux/smp.h>

#include <asm/atomic.h>
#include <asm/cpu.h>
#include <asm/processor.h>
#include <asm/system.h>
#include <asm/mmu_context.h>
#include <asm/smtc_ipi.h>

/* VPE/SMP Prototype implements platform interfaces directly */

/*
 * Cause the specified action to be performed on a targeted "CPU"
 */

static void ssmtc_send_ipi_single(int cpu, unsigned int action)
{
	smtc_send_ipi(cpu, LINUX_SMP_IPI, action);
	/* "CPU" may be TC of same VPE, VPE of same CPU, or different CPU */
}

static inline void ssmtc_send_ipi_mask(cpumask_t mask, unsigned int action)
{
	unsigned int i;

	for_each_cpu_mask(i, mask)
		ssmtc_send_ipi_single(i, action);
}

/*
 * Post-config but pre-boot cleanup entry point
 */
static void __cpuinit ssmtc_init_secondary(void)
{
	void smtc_init_secondary(void);

	smtc_init_secondary();
}

/*
 * SMP initialization finalization entry point
 */
static void __cpuinit ssmtc_smp_finish(void)
{
	smtc_smp_finish();
}

/*
 * Hook for after all CPUs are online
 */
static void ssmtc_cpus_done(void)
{
}

/*
 * Platform "CPU" startup hook
 */
static void __cpuinit ssmtc_boot_secondary(int cpu, struct task_struct *idle)
{
	smtc_boot_secondary(cpu, idle);
}

static void __init ssmtc_smp_setup(void)
{
	if (read_c0_config3() & (1 << 2))
		mipsmt_build_cpu_map(0);
}

/*
 * Platform SMP pre-initialization
 */
static void ssmtc_prepare_cpus(unsigned int max_cpus)
{
	/*
	 * As noted above, we can assume a single CPU for now
	 * but it may be multithreaded.
	 */

	if (read_c0_config3() & (1 << 2)) {
		mipsmt_prepare_cpus();
	}
}

struct plat_smp_ops ssmtc_smp_ops = {
	.send_ipi_single	= ssmtc_send_ipi_single,
	.send_ipi_mask		= ssmtc_send_ipi_mask,
	.init_secondary		= ssmtc_init_secondary,
	.smp_finish		= ssmtc_smp_finish,
	.cpus_done		= ssmtc_cpus_done,
	.boot_secondary		= ssmtc_boot_secondary,
	.smp_setup		= ssmtc_smp_setup,
	.prepare_cpus		= ssmtc_prepare_cpus,
};
