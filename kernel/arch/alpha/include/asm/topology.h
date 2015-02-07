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

#ifndef _ASM_ALPHA_TOPOLOGY_H
#define _ASM_ALPHA_TOPOLOGY_H

#include <linux/smp.h>
#include <linux/threads.h>
#include <asm/machvec.h>

#ifdef CONFIG_NUMA
static inline int cpu_to_node(int cpu)
{
	int node;
	
	if (!alpha_mv.cpuid_to_nid)
		return 0;

	node = alpha_mv.cpuid_to_nid(cpu);

#ifdef DEBUG_NUMA
	BUG_ON(node < 0);
#endif

	return node;
}

static inline cpumask_t node_to_cpumask(int node)
{
	cpumask_t node_cpu_mask = CPU_MASK_NONE;
	int cpu;

	for_each_online_cpu(cpu) {
		if (cpu_to_node(cpu) == node)
			cpu_set(cpu, node_cpu_mask);
	}

#ifdef DEBUG_NUMA
	printk("node %d: cpu_mask: %016lx\n", node, node_cpu_mask);
#endif

	return node_cpu_mask;
}

extern struct cpumask node_to_cpumask_map[];
/* FIXME: This is dumb, recalculating every time.  But simple. */
static const struct cpumask *cpumask_of_node(int node)
{
	int cpu;

	cpumask_clear(&node_to_cpumask_map[node]);

	for_each_online_cpu(cpu) {
		if (cpu_to_node(cpu) == node)
			cpumask_set_cpu(cpu, node_to_cpumask_map[node]);
	}

	return &node_to_cpumask_map[node];
}

#define pcibus_to_cpumask(bus)	(cpu_online_map)
#define cpumask_of_pcibus(bus)	(cpu_online_mask)

#endif /* !CONFIG_NUMA */
# include <asm-generic/topology.h>

#endif /* _ASM_ALPHA_TOPOLOGY_H */
