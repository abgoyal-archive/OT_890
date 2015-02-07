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
 * Copyright (C) 2006 Benjamin Herrenschmidt, IBM Corporation
 *
 * Provide default implementations of the DMA mapping callbacks for
 * busses using the iommu infrastructure
 */

#include <asm/iommu.h>

/*
 * Generic iommu implementation
 */

/* Allocates a contiguous real buffer and creates mappings over it.
 * Returns the virtual address of the buffer and sets dma_handle
 * to the dma address (mapping) of the first page.
 */
static void *dma_iommu_alloc_coherent(struct device *dev, size_t size,
				      dma_addr_t *dma_handle, gfp_t flag)
{
	return iommu_alloc_coherent(dev, dev->archdata.dma_data, size,
				    dma_handle, device_to_mask(dev), flag,
				    dev_to_node(dev));
}

static void dma_iommu_free_coherent(struct device *dev, size_t size,
				    void *vaddr, dma_addr_t dma_handle)
{
	iommu_free_coherent(dev->archdata.dma_data, size, vaddr, dma_handle);
}

/* Creates TCEs for a user provided buffer.  The user buffer must be
 * contiguous real kernel storage (not vmalloc).  The address passed here
 * comprises a page address and offset into that page. The dma_addr_t
 * returned will point to the same byte within the page as was passed in.
 */
static dma_addr_t dma_iommu_map_page(struct device *dev, struct page *page,
				     unsigned long offset, size_t size,
				     enum dma_data_direction direction,
				     struct dma_attrs *attrs)
{
	return iommu_map_page(dev, dev->archdata.dma_data, page, offset, size,
			      device_to_mask(dev), direction, attrs);
}


static void dma_iommu_unmap_page(struct device *dev, dma_addr_t dma_handle,
				 size_t size, enum dma_data_direction direction,
				 struct dma_attrs *attrs)
{
	iommu_unmap_page(dev->archdata.dma_data, dma_handle, size, direction,
			 attrs);
}


static int dma_iommu_map_sg(struct device *dev, struct scatterlist *sglist,
			    int nelems, enum dma_data_direction direction,
			    struct dma_attrs *attrs)
{
	return iommu_map_sg(dev, dev->archdata.dma_data, sglist, nelems,
			    device_to_mask(dev), direction, attrs);
}

static void dma_iommu_unmap_sg(struct device *dev, struct scatterlist *sglist,
		int nelems, enum dma_data_direction direction,
		struct dma_attrs *attrs)
{
	iommu_unmap_sg(dev->archdata.dma_data, sglist, nelems, direction,
		       attrs);
}

/* We support DMA to/from any memory page via the iommu */
static int dma_iommu_dma_supported(struct device *dev, u64 mask)
{
	struct iommu_table *tbl = dev->archdata.dma_data;

	if (!tbl || tbl->it_offset > mask) {
		printk(KERN_INFO
		       "Warning: IOMMU offset too big for device mask\n");
		if (tbl)
			printk(KERN_INFO
			       "mask: 0x%08llx, table offset: 0x%08lx\n",
				mask, tbl->it_offset);
		else
			printk(KERN_INFO "mask: 0x%08llx, table unavailable\n",
				mask);
		return 0;
	} else
		return 1;
}

struct dma_mapping_ops dma_iommu_ops = {
	.alloc_coherent	= dma_iommu_alloc_coherent,
	.free_coherent	= dma_iommu_free_coherent,
	.map_sg		= dma_iommu_map_sg,
	.unmap_sg	= dma_iommu_unmap_sg,
	.dma_supported	= dma_iommu_dma_supported,
	.map_page	= dma_iommu_map_page,
	.unmap_page	= dma_iommu_unmap_page,
};
EXPORT_SYMBOL(dma_iommu_ops);
