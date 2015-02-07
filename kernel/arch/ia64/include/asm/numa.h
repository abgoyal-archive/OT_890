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
 * This file contains NUMA specific prototypes and definitions.
 *
 * 2002/08/05 Erich Focht <efocht@ess.nec.de>
 *
 */
#ifndef _ASM_IA64_NUMA_H
#define _ASM_IA64_NUMA_H


#ifdef CONFIG_NUMA

#include <linux/cache.h>
#include <linux/cpumask.h>
#include <linux/numa.h>
#include <linux/smp.h>
#include <linux/threads.h>

#include <asm/mmzone.h>

#define NUMA_NO_NODE	-1

extern u16 cpu_to_node_map[NR_CPUS] __cacheline_aligned;
extern cpumask_t node_to_cpu_mask[MAX_NUMNODES] __cacheline_aligned;
extern pg_data_t *pgdat_list[MAX_NUMNODES];

/* Stuff below this line could be architecture independent */

extern int num_node_memblks;		/* total number of memory chunks */

/*
 * List of node memory chunks. Filled when parsing SRAT table to
 * obtain information about memory nodes.
*/

struct node_memblk_s {
	unsigned long start_paddr;
	unsigned long size;
	int nid;		/* which logical node contains this chunk? */
	int bank;		/* which mem bank on this node */
};

struct node_cpuid_s {
	u16	phys_id;	/* id << 8 | eid */
	int	nid;		/* logical node containing this CPU */
};

extern struct node_memblk_s node_memblk[NR_NODE_MEMBLKS];
extern struct node_cpuid_s node_cpuid[NR_CPUS];

/*
 * ACPI 2.0 SLIT (System Locality Information Table)
 * http://devresource.hp.com/devresource/Docs/TechPapers/IA64/slit.pdf
 *
 * This is a matrix with "distances" between nodes, they should be
 * proportional to the memory access latency ratios.
 */

extern u8 numa_slit[MAX_NUMNODES * MAX_NUMNODES];
#define node_distance(from,to) (numa_slit[(from) * num_online_nodes() + (to)])

extern int paddr_to_nid(unsigned long paddr);

#define local_nodeid (cpu_to_node_map[smp_processor_id()])

extern void map_cpu_to_node(int cpu, int nid);
extern void unmap_cpu_from_node(int cpu, int nid);


#else /* !CONFIG_NUMA */
#define map_cpu_to_node(cpu, nid)	do{}while(0)
#define unmap_cpu_from_node(cpu, nid)	do{}while(0)

#define paddr_to_nid(addr)	0

#endif /* CONFIG_NUMA */

#endif /* _ASM_IA64_NUMA_H */
