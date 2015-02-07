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
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * ia64 kernel NUMA specific stuff
 *
 * Copyright (C) 2002 Erich Focht <efocht@ess.nec.de>
 * Copyright (C) 2004 Silicon Graphics, Inc.
 *   Jesse Barnes <jbarnes@sgi.com>
 */
#include <linux/topology.h>
#include <linux/module.h>
#include <asm/processor.h>
#include <asm/smp.h>

u16 cpu_to_node_map[NR_CPUS] __cacheline_aligned;
EXPORT_SYMBOL(cpu_to_node_map);

cpumask_t node_to_cpu_mask[MAX_NUMNODES] __cacheline_aligned;
EXPORT_SYMBOL(node_to_cpu_mask);

void __cpuinit map_cpu_to_node(int cpu, int nid)
{
	int oldnid;
	if (nid < 0) { /* just initialize by zero */
		cpu_to_node_map[cpu] = 0;
		return;
	}
	/* sanity check first */
	oldnid = cpu_to_node_map[cpu];
	if (cpu_isset(cpu, node_to_cpu_mask[oldnid])) {
		return; /* nothing to do */
	}
	/* we don't have cpu-driven node hot add yet...
	   In usual case, node is created from SRAT at boot time. */
	if (!node_online(nid))
		nid = first_online_node;
	cpu_to_node_map[cpu] = nid;
	cpu_set(cpu, node_to_cpu_mask[nid]);
	return;
}

void __cpuinit unmap_cpu_from_node(int cpu, int nid)
{
	WARN_ON(!cpu_isset(cpu, node_to_cpu_mask[nid]));
	WARN_ON(cpu_to_node_map[cpu] != nid);
	cpu_to_node_map[cpu] = 0;
	cpu_clear(cpu, node_to_cpu_mask[nid]);
}


/**
 * build_cpu_to_node_map - setup cpu to node and node to cpumask arrays
 *
 * Build cpu to node mapping and initialize the per node cpu masks using
 * info from the node_cpuid array handed to us by ACPI.
 */
void __init build_cpu_to_node_map(void)
{
	int cpu, i, node;

	for(node=0; node < MAX_NUMNODES; node++)
		cpus_clear(node_to_cpu_mask[node]);

	for_each_possible_early_cpu(cpu) {
		node = -1;
		for (i = 0; i < NR_CPUS; ++i)
			if (cpu_physical_id(cpu) == node_cpuid[i].phys_id) {
				node = node_cpuid[i].nid;
				break;
			}
		map_cpu_to_node(cpu, node);
	}
}
