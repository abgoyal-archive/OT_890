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

#ifndef _ASM_MMZONE_H_
#define _ASM_MMZONE_H_

#include <asm/smp.h>

#ifdef CONFIG_DISCONTIGMEM

extern struct pglist_data *node_data[];
#define NODE_DATA(nid)		(node_data[nid])

#define node_localnr(pfn, nid)	((pfn) - NODE_DATA(nid)->node_start_pfn)
#define node_start_pfn(nid)	(NODE_DATA(nid)->node_start_pfn)
#define node_end_pfn(nid)						\
({									\
	pg_data_t *__pgdat = NODE_DATA(nid);				\
	__pgdat->node_start_pfn + __pgdat->node_spanned_pages - 1;	\
})

#define pmd_page(pmd)		(pfn_to_page(pmd_val(pmd) >> PAGE_SHIFT))
/*
 * pfn_valid should be made as fast as possible, and the current definition
 * is valid for machines that are NUMA, but still contiguous, which is what
 * is currently supported. A more generalised, but slower definition would
 * be something like this - mbligh:
 * ( pfn_to_pgdat(pfn) && ((pfn) < node_end_pfn(pfn_to_nid(pfn))) )
 */
#if 1	/* M32R_FIXME */
#define pfn_valid(pfn)	(1)
#else
#define pfn_valid(pfn)	((pfn) < num_physpages)
#endif

/*
 * generic node memory support, the following assumptions apply:
 */

static __inline__ int pfn_to_nid(unsigned long pfn)
{
	int node;

	for (node = 0 ; node < MAX_NUMNODES ; node++)
		if (pfn >= node_start_pfn(node) && pfn <= node_end_pfn(node))
			break;

	return node;
}

static __inline__ struct pglist_data *pfn_to_pgdat(unsigned long pfn)
{
	return(NODE_DATA(pfn_to_nid(pfn)));
}

#endif /* CONFIG_DISCONTIGMEM */
#endif /* _ASM_MMZONE_H_ */
