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

/* iommu_common.h: UltraSparc SBUS/PCI common iommu declarations.
 *
 * Copyright (C) 1999, 2008 David S. Miller (davem@davemloft.net)
 */

#ifndef _IOMMU_COMMON_H
#define _IOMMU_COMMON_H

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/scatterlist.h>
#include <linux/device.h>
#include <linux/iommu-helper.h>

#include <asm/iommu.h>
#include <asm/scatterlist.h>

/*
 * These give mapping size of each iommu pte/tlb.
 */
#define IO_PAGE_SHIFT			13
#define IO_PAGE_SIZE			(1UL << IO_PAGE_SHIFT)
#define IO_PAGE_MASK			(~(IO_PAGE_SIZE-1))
#define IO_PAGE_ALIGN(addr)		ALIGN(addr, IO_PAGE_SIZE)

#define IO_TSB_ENTRIES			(128*1024)
#define IO_TSB_SIZE			(IO_TSB_ENTRIES * 8)

/*
 * This is the hardwired shift in the iotlb tag/data parts.
 */
#define IOMMU_PAGE_SHIFT		13

#define SG_ENT_PHYS_ADDRESS(SG)	(__pa(sg_virt((SG))))

static inline int is_span_boundary(unsigned long entry,
				   unsigned long shift,
				   unsigned long boundary_size,
				   struct scatterlist *outs,
				   struct scatterlist *sg)
{
	unsigned long paddr = SG_ENT_PHYS_ADDRESS(outs);
	int nr = iommu_num_pages(paddr, outs->dma_length + sg->length,
				 IO_PAGE_SIZE);

	return iommu_is_span_boundary(entry, nr, shift, boundary_size);
}

extern unsigned long iommu_range_alloc(struct device *dev,
				       struct iommu *iommu,
				       unsigned long npages,
				       unsigned long *handle);
extern void iommu_range_free(struct iommu *iommu,
			     dma_addr_t dma_addr,
			     unsigned long npages);

#endif /* _IOMMU_COMMON_H */
