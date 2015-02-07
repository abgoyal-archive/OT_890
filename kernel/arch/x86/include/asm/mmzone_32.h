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
 * Written by Pat Gaughen (gone@us.ibm.com) Mar 2002
 *
 */

#ifndef _ASM_X86_MMZONE_32_H
#define _ASM_X86_MMZONE_32_H

#include <asm/smp.h>

#ifdef CONFIG_NUMA
extern struct pglist_data *node_data[];
#define NODE_DATA(nid)	(node_data[nid])

#include <asm/numaq.h>
/* summit or generic arch */
#include <asm/srat.h>

extern int get_memcfg_numa_flat(void);
/*
 * This allows any one NUMA architecture to be compiled
 * for, and still fall back to the flat function if it
 * fails.
 */
static inline void get_memcfg_numa(void)
{

	if (get_memcfg_numaq())
		return;
	if (get_memcfg_from_srat())
		return;
	get_memcfg_numa_flat();
}

extern void resume_map_numa_kva(pgd_t *pgd);

#else /* !CONFIG_NUMA */

#define get_memcfg_numa get_memcfg_numa_flat

static inline void resume_map_numa_kva(pgd_t *pgd) {}

#endif /* CONFIG_NUMA */

#ifdef CONFIG_DISCONTIGMEM

/*
 * generic node memory support, the following assumptions apply:
 *
 * 1) memory comes in 64Mb contigious chunks which are either present or not
 * 2) we will not have more than 64Gb in total
 *
 * for now assume that 64Gb is max amount of RAM for whole system
 *    64Gb / 4096bytes/page = 16777216 pages
 */
#define MAX_NR_PAGES 16777216
#define MAX_ELEMENTS 1024
#define PAGES_PER_ELEMENT (MAX_NR_PAGES/MAX_ELEMENTS)

extern s8 physnode_map[];

static inline int pfn_to_nid(unsigned long pfn)
{
#ifdef CONFIG_NUMA
	return((int) physnode_map[(pfn) / PAGES_PER_ELEMENT]);
#else
	return 0;
#endif
}

/*
 * Following are macros that each numa implmentation must define.
 */

#define node_start_pfn(nid)	(NODE_DATA(nid)->node_start_pfn)
#define node_end_pfn(nid)						\
({									\
	pg_data_t *__pgdat = NODE_DATA(nid);				\
	__pgdat->node_start_pfn + __pgdat->node_spanned_pages;		\
})

static inline int pfn_valid(int pfn)
{
	int nid = pfn_to_nid(pfn);

	if (nid >= 0)
		return (pfn < node_end_pfn(nid));
	return 0;
}

#endif /* CONFIG_DISCONTIGMEM */

#ifdef CONFIG_NEED_MULTIPLE_NODES

/*
 * Following are macros that are specific to this numa platform.
 */
#define reserve_bootmem(addr, size, flags) \
	reserve_bootmem_node(NODE_DATA(0), (addr), (size), (flags))
#define alloc_bootmem(x) \
	__alloc_bootmem_node(NODE_DATA(0), (x), SMP_CACHE_BYTES, __pa(MAX_DMA_ADDRESS))
#define alloc_bootmem_nopanic(x) \
	__alloc_bootmem_node_nopanic(NODE_DATA(0), (x), SMP_CACHE_BYTES, \
				__pa(MAX_DMA_ADDRESS))
#define alloc_bootmem_low(x) \
	__alloc_bootmem_node(NODE_DATA(0), (x), SMP_CACHE_BYTES, 0)
#define alloc_bootmem_pages(x) \
	__alloc_bootmem_node(NODE_DATA(0), (x), PAGE_SIZE, __pa(MAX_DMA_ADDRESS))
#define alloc_bootmem_pages_nopanic(x) \
	__alloc_bootmem_node_nopanic(NODE_DATA(0), (x), PAGE_SIZE, \
				__pa(MAX_DMA_ADDRESS))
#define alloc_bootmem_low_pages(x) \
	__alloc_bootmem_node(NODE_DATA(0), (x), PAGE_SIZE, 0)
#define alloc_bootmem_node(pgdat, x)					\
({									\
	struct pglist_data  __maybe_unused			\
				*__alloc_bootmem_node__pgdat = (pgdat);	\
	__alloc_bootmem_node(NODE_DATA(0), (x), SMP_CACHE_BYTES,	\
						__pa(MAX_DMA_ADDRESS));	\
})
#define alloc_bootmem_pages_node(pgdat, x)				\
({									\
	struct pglist_data  __maybe_unused			\
				*__alloc_bootmem_node__pgdat = (pgdat);	\
	__alloc_bootmem_node(NODE_DATA(0), (x), PAGE_SIZE,		\
						__pa(MAX_DMA_ADDRESS));	\
})
#define alloc_bootmem_low_pages_node(pgdat, x)				\
({									\
	struct pglist_data  __maybe_unused			\
				*__alloc_bootmem_node__pgdat = (pgdat);	\
	__alloc_bootmem_node(NODE_DATA(0), (x), PAGE_SIZE, 0);		\
})
#endif /* CONFIG_NEED_MULTIPLE_NODES */

#endif /* _ASM_X86_MMZONE_32_H */
