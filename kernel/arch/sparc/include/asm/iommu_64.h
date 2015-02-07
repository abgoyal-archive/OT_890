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

/* iommu.h: Definitions for the sun5 IOMMU.
 *
 * Copyright (C) 1996, 1999, 2007 David S. Miller (davem@davemloft.net)
 */
#ifndef _SPARC64_IOMMU_H
#define _SPARC64_IOMMU_H

/* The format of an iopte in the page tables. */
#define IOPTE_VALID   0x8000000000000000UL
#define IOPTE_64K     0x2000000000000000UL
#define IOPTE_STBUF   0x1000000000000000UL
#define IOPTE_INTRA   0x0800000000000000UL
#define IOPTE_CONTEXT 0x07ff800000000000UL
#define IOPTE_PAGE    0x00007fffffffe000UL
#define IOPTE_CACHE   0x0000000000000010UL
#define IOPTE_WRITE   0x0000000000000002UL

#define IOMMU_NUM_CTXS	4096

struct iommu_arena {
	unsigned long	*map;
	unsigned int	hint;
	unsigned int	limit;
};

struct iommu {
	spinlock_t		lock;
	struct iommu_arena	arena;
	void			(*flush_all)(struct iommu *);
	iopte_t			*page_table;
	u32			page_table_map_base;
	unsigned long		iommu_control;
	unsigned long		iommu_tsbbase;
	unsigned long		iommu_flush;
	unsigned long		iommu_flushinv;
	unsigned long		iommu_tags;
	unsigned long		iommu_ctxflush;
	unsigned long		write_complete_reg;
	unsigned long		dummy_page;
	unsigned long		dummy_page_pa;
	unsigned long		ctx_lowest_free;
	DECLARE_BITMAP(ctx_bitmap, IOMMU_NUM_CTXS);
	u32			dma_addr_mask;
};

struct strbuf {
	int			strbuf_enabled;
	unsigned long		strbuf_control;
	unsigned long		strbuf_pflush;
	unsigned long		strbuf_fsync;
	unsigned long		strbuf_err_stat;
	unsigned long		strbuf_tag_diag;
	unsigned long		strbuf_line_diag;
	unsigned long		strbuf_ctxflush;
	unsigned long		strbuf_ctxmatch_base;
	unsigned long		strbuf_flushflag_pa;
	volatile unsigned long *strbuf_flushflag;
	volatile unsigned long	__flushflag_buf[(64+(64-1)) / sizeof(long)];
};

extern int iommu_table_init(struct iommu *iommu, int tsbsize,
			    u32 dma_offset, u32 dma_addr_mask,
			    int numa_node);

#endif /* !(_SPARC64_IOMMU_H) */
