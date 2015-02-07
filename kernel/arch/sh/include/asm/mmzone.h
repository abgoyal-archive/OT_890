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

#ifndef __ASM_SH_MMZONE_H
#define __ASM_SH_MMZONE_H

#ifdef __KERNEL__

#ifdef CONFIG_NEED_MULTIPLE_NODES
#include <linux/numa.h>

extern struct pglist_data *node_data[];
#define NODE_DATA(nid)		(node_data[nid])

#define node_start_pfn(nid)	(NODE_DATA(nid)->node_start_pfn)
#define node_end_pfn(nid)	(NODE_DATA(nid)->node_start_pfn + \
				 NODE_DATA(nid)->node_spanned_pages)

static inline int pfn_to_nid(unsigned long pfn)
{
	int nid;

	for (nid = 0; nid < MAX_NUMNODES; nid++)
		if (pfn >= node_start_pfn(nid) && pfn <= node_end_pfn(nid))
			break;

	return nid;
}

static inline struct pglist_data *pfn_to_pgdat(unsigned long pfn)
{
	return NODE_DATA(pfn_to_nid(pfn));
}

/* arch/sh/mm/numa.c */
void __init setup_bootmem_node(int nid, unsigned long start, unsigned long end);
#else
static inline void
setup_bootmem_node(int nid, unsigned long start, unsigned long end)
{
}
#endif /* CONFIG_NEED_MULTIPLE_NODES */

/* Platform specific mem init */
void __init plat_mem_setup(void);

/* arch/sh/kernel/setup.c */
void __init setup_bootmem_allocator(unsigned long start_pfn);
void __init __add_active_range(unsigned int nid, unsigned long start_pfn,
			       unsigned long end_pfn);

#endif /* __KERNEL__ */
#endif /* __ASM_SH_MMZONE_H */
