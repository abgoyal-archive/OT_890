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

#ifndef __LINUX_SWIOTLB_H
#define __LINUX_SWIOTLB_H

#include <linux/types.h>

struct device;
struct dma_attrs;
struct scatterlist;

/*
 * Maximum allowable number of contiguous slabs to map,
 * must be a power of 2.  What is the appropriate value ?
 * The complexity of {map,unmap}_single is linearly dependent on this value.
 */
#define IO_TLB_SEGSIZE	128


/*
 * log of the size of each IO TLB slab.  The number of slabs is command line
 * controllable.
 */
#define IO_TLB_SHIFT 11

extern void
swiotlb_init(void);

extern void *swiotlb_alloc_boot(size_t bytes, unsigned long nslabs);
extern void *swiotlb_alloc(unsigned order, unsigned long nslabs);

extern dma_addr_t swiotlb_phys_to_bus(struct device *hwdev,
				      phys_addr_t address);
extern phys_addr_t swiotlb_bus_to_phys(dma_addr_t address);

extern int swiotlb_arch_range_needs_mapping(void *ptr, size_t size);

extern void
*swiotlb_alloc_coherent(struct device *hwdev, size_t size,
			dma_addr_t *dma_handle, gfp_t flags);

extern void
swiotlb_free_coherent(struct device *hwdev, size_t size,
		      void *vaddr, dma_addr_t dma_handle);

extern dma_addr_t
swiotlb_map_single(struct device *hwdev, void *ptr, size_t size, int dir);

extern void
swiotlb_unmap_single(struct device *hwdev, dma_addr_t dev_addr,
		     size_t size, int dir);

extern dma_addr_t
swiotlb_map_single_attrs(struct device *hwdev, void *ptr, size_t size,
			 int dir, struct dma_attrs *attrs);

extern void
swiotlb_unmap_single_attrs(struct device *hwdev, dma_addr_t dev_addr,
			   size_t size, int dir, struct dma_attrs *attrs);

extern int
swiotlb_map_sg(struct device *hwdev, struct scatterlist *sg, int nents,
	       int direction);

extern void
swiotlb_unmap_sg(struct device *hwdev, struct scatterlist *sg, int nents,
		 int direction);

extern int
swiotlb_map_sg_attrs(struct device *hwdev, struct scatterlist *sgl, int nelems,
		     int dir, struct dma_attrs *attrs);

extern void
swiotlb_unmap_sg_attrs(struct device *hwdev, struct scatterlist *sgl,
		       int nelems, int dir, struct dma_attrs *attrs);

extern void
swiotlb_sync_single_for_cpu(struct device *hwdev, dma_addr_t dev_addr,
			    size_t size, int dir);

extern void
swiotlb_sync_sg_for_cpu(struct device *hwdev, struct scatterlist *sg,
			int nelems, int dir);

extern void
swiotlb_sync_single_for_device(struct device *hwdev, dma_addr_t dev_addr,
			       size_t size, int dir);

extern void
swiotlb_sync_sg_for_device(struct device *hwdev, struct scatterlist *sg,
			   int nelems, int dir);

extern void
swiotlb_sync_single_range_for_cpu(struct device *hwdev, dma_addr_t dev_addr,
				  unsigned long offset, size_t size, int dir);

extern void
swiotlb_sync_single_range_for_device(struct device *hwdev, dma_addr_t dev_addr,
				     unsigned long offset, size_t size,
				     int dir);

extern int
swiotlb_dma_mapping_error(struct device *hwdev, dma_addr_t dma_addr);

extern int
swiotlb_dma_supported(struct device *hwdev, u64 mask);

#endif /* __LINUX_SWIOTLB_H */
