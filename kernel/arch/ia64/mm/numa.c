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
 * This file contains NUMA specific variables and functions which can
 * be split away from DISCONTIGMEM and are used on NUMA machines with
 * contiguous memory.
 * 
 *                         2002/08/07 Erich Focht <efocht@ess.nec.de>
 */

#include <linux/cpu.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/node.h>
#include <linux/init.h>
#include <linux/bootmem.h>
#include <linux/module.h>
#include <asm/mmzone.h>
#include <asm/numa.h>


/*
 * The following structures are usually initialized by ACPI or
 * similar mechanisms and describe the NUMA characteristics of the machine.
 */
int num_node_memblks;
struct node_memblk_s node_memblk[NR_NODE_MEMBLKS];
struct node_cpuid_s node_cpuid[NR_CPUS] =
	{ [0 ... NR_CPUS-1] = { .phys_id = 0, .nid = NUMA_NO_NODE } };

/*
 * This is a matrix with "distances" between nodes, they should be
 * proportional to the memory access latency ratios.
 */
u8 numa_slit[MAX_NUMNODES * MAX_NUMNODES];

/* Identify which cnode a physical address resides on */
int
paddr_to_nid(unsigned long paddr)
{
	int	i;

	for (i = 0; i < num_node_memblks; i++)
		if (paddr >= node_memblk[i].start_paddr &&
		    paddr < node_memblk[i].start_paddr + node_memblk[i].size)
			break;

	return (i < num_node_memblks) ? node_memblk[i].nid : (num_node_memblks ? -1 : 0);
}

#if defined(CONFIG_SPARSEMEM) && defined(CONFIG_NUMA)
/*
 * Because of holes evaluate on section limits.
 * If the section of memory exists, then return the node where the section
 * resides.  Otherwise return node 0 as the default.  This is used by
 * SPARSEMEM to allocate the SPARSEMEM sectionmap on the NUMA node where
 * the section resides.
 */
int __meminit __early_pfn_to_nid(unsigned long pfn)
{
	int i, section = pfn >> PFN_SECTION_SHIFT, ssec, esec;

	for (i = 0; i < num_node_memblks; i++) {
		ssec = node_memblk[i].start_paddr >> PA_SECTION_SHIFT;
		esec = (node_memblk[i].start_paddr + node_memblk[i].size +
			((1L << PA_SECTION_SHIFT) - 1)) >> PA_SECTION_SHIFT;
		if (section >= ssec && section < esec)
			return node_memblk[i].nid;
	}

	return -1;
}

#ifdef CONFIG_MEMORY_HOTPLUG
/*
 *  SRAT information is stored in node_memblk[], then we can use SRAT
 *  information at memory-hot-add if necessary.
 */

int memory_add_physaddr_to_nid(u64 addr)
{
	int nid = paddr_to_nid(addr);
	if (nid < 0)
		return 0;
	return nid;
}

EXPORT_SYMBOL_GPL(memory_add_physaddr_to_nid);
#endif
#endif
