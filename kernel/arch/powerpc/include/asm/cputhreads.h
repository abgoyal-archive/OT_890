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

#ifndef _ASM_POWERPC_CPUTHREADS_H
#define _ASM_POWERPC_CPUTHREADS_H

#include <linux/cpumask.h>

/*
 * Mapping of threads to cores
 */

#ifdef CONFIG_SMP
extern int threads_per_core;
extern int threads_shift;
extern cpumask_t threads_core_mask;
#else
#define threads_per_core	1
#define threads_shift		0
#define threads_core_mask	(CPU_MASK_CPU0)
#endif

/* cpu_thread_mask_to_cores - Return a cpumask of one per cores
 *                            hit by the argument
 *
 * @threads:	a cpumask of threads
 *
 * This function returns a cpumask which will have one "cpu" (or thread)
 * bit set for each core that has at least one thread set in the argument.
 *
 * This can typically be used for things like IPI for tlb invalidations
 * since those need to be done only once per core/TLB
 */
static inline cpumask_t cpu_thread_mask_to_cores(cpumask_t threads)
{
	cpumask_t	tmp, res;
	int		i;

	res = CPU_MASK_NONE;
	for (i = 0; i < NR_CPUS; i += threads_per_core) {
		cpus_shift_left(tmp, threads_core_mask, i);
		if (cpus_intersects(threads, tmp))
			cpu_set(i, res);
	}
	return res;
}

static inline int cpu_nr_cores(void)
{
	return NR_CPUS >> threads_shift;
}

static inline cpumask_t cpu_online_cores_map(void)
{
	return cpu_thread_mask_to_cores(cpu_online_map);
}

static inline int cpu_thread_to_core(int cpu)
{
	return cpu >> threads_shift;
}

static inline int cpu_thread_in_core(int cpu)
{
	return cpu & (threads_per_core - 1);
}

static inline int cpu_first_thread_in_core(int cpu)
{
	return cpu & ~(threads_per_core - 1);
}

#endif /* _ASM_POWERPC_CPUTHREADS_H */

