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
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2006, 07 by Ralf Baechle (ralf@linux-mips.org)
 *
 * Symmetric Uniprocessor (TM) Support
 */
#include <linux/kernel.h>
#include <linux/sched.h>

/*
 * Send inter-processor interrupt
 */
void up_send_ipi_single(int cpu, unsigned int action)
{
	panic(KERN_ERR "%s called", __func__);
}

static inline void up_send_ipi_mask(cpumask_t mask, unsigned int action)
{
	panic(KERN_ERR "%s called", __func__);
}

/*
 *  After we've done initial boot, this function is called to allow the
 *  board code to clean up state, if needed
 */
void __cpuinit up_init_secondary(void)
{
}

void __cpuinit up_smp_finish(void)
{
}

/* Hook for after all CPUs are online */
void up_cpus_done(void)
{
}

/*
 * Firmware CPU startup hook
 */
void __cpuinit up_boot_secondary(int cpu, struct task_struct *idle)
{
}

void __init up_smp_setup(void)
{
}

void __init up_prepare_cpus(unsigned int max_cpus)
{
}

struct plat_smp_ops up_smp_ops = {
	.send_ipi_single	= up_send_ipi_single,
	.send_ipi_mask		= up_send_ipi_mask,
	.init_secondary		= up_init_secondary,
	.smp_finish		= up_smp_finish,
	.cpus_done		= up_cpus_done,
	.boot_secondary		= up_boot_secondary,
	.smp_setup		= up_smp_setup,
	.prepare_cpus		= up_prepare_cpus,
};
