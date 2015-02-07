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

/* K8 NUMA support */
/* Copyright 2002,2003 by Andi Kleen, SuSE Labs */
/* 2.5 Version loosely based on the NUMAQ Code by Pat Gaughen. */
#ifndef _ASM_X86_MMZONE_64_H
#define _ASM_X86_MMZONE_64_H


#ifdef CONFIG_NUMA

#include <linux/mmdebug.h>

#include <asm/smp.h>

/* Simple perfect hash to map physical addresses to node numbers */
struct memnode {
	int shift;
	unsigned int mapsize;
	s16 *map;
	s16 embedded_map[64 - 8];
} ____cacheline_aligned; /* total size = 128 bytes */
extern struct memnode memnode;
#define memnode_shift memnode.shift
#define memnodemap memnode.map
#define memnodemapsize memnode.mapsize

extern struct pglist_data *node_data[];

static inline __attribute__((pure)) int phys_to_nid(unsigned long addr)
{
	unsigned nid;
	VIRTUAL_BUG_ON(!memnodemap);
	nid = memnodemap[addr >> memnode_shift];
	VIRTUAL_BUG_ON(nid >= MAX_NUMNODES || !node_data[nid]);
	return nid;
}

#define NODE_DATA(nid)		(node_data[nid])

#define node_start_pfn(nid)	(NODE_DATA(nid)->node_start_pfn)
#define node_end_pfn(nid)       (NODE_DATA(nid)->node_start_pfn +	\
				 NODE_DATA(nid)->node_spanned_pages)

#ifdef CONFIG_NUMA_EMU
#define FAKE_NODE_MIN_SIZE	(64 * 1024 * 1024)
#define FAKE_NODE_MIN_HASH_MASK	(~(FAKE_NODE_MIN_SIZE - 1UL))
#endif

#endif
#endif /* _ASM_X86_MMZONE_64_H */
