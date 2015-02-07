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

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/intel-iommu.h>

void *
vtd_alloc_coherent(struct device *dev, size_t size, dma_addr_t *dma_handle,
		 gfp_t flags)
{
	return intel_alloc_coherent(dev, size, dma_handle, flags);
}
EXPORT_SYMBOL_GPL(vtd_alloc_coherent);

void
vtd_free_coherent(struct device *dev, size_t size, void *vaddr,
		 dma_addr_t dma_handle)
{
	intel_free_coherent(dev, size, vaddr, dma_handle);
}
EXPORT_SYMBOL_GPL(vtd_free_coherent);

dma_addr_t
vtd_map_single_attrs(struct device *dev, void *addr, size_t size,
		     int dir, struct dma_attrs *attrs)
{
	return intel_map_single(dev, (phys_addr_t)addr, size, dir);
}
EXPORT_SYMBOL_GPL(vtd_map_single_attrs);

void
vtd_unmap_single_attrs(struct device *dev, dma_addr_t iova, size_t size,
		       int dir, struct dma_attrs *attrs)
{
	intel_unmap_single(dev, iova, size, dir);
}
EXPORT_SYMBOL_GPL(vtd_unmap_single_attrs);

int
vtd_map_sg_attrs(struct device *dev, struct scatterlist *sglist, int nents,
		 int dir, struct dma_attrs *attrs)
{
	return intel_map_sg(dev, sglist, nents, dir);
}
EXPORT_SYMBOL_GPL(vtd_map_sg_attrs);

void
vtd_unmap_sg_attrs(struct device *dev, struct scatterlist *sglist,
		   int nents, int dir, struct dma_attrs *attrs)
{
	intel_unmap_sg(dev, sglist, nents, dir);
}
EXPORT_SYMBOL_GPL(vtd_unmap_sg_attrs);

int
vtd_dma_mapping_error(struct device *dev, dma_addr_t dma_addr)
{
	return 0;
}
EXPORT_SYMBOL_GPL(vtd_dma_mapping_error);
