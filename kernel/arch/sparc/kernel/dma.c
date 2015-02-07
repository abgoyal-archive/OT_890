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

/* dma.c: PCI and SBUS DMA accessors for 32-bit sparc.
 *
 * Copyright (C) 2008 David S. Miller <davem@davemloft.net>
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/dma-mapping.h>
#include <linux/scatterlist.h>
#include <linux/mm.h>

#ifdef CONFIG_PCI
#include <linux/pci.h>
#endif

#include "dma.h"

int dma_supported(struct device *dev, u64 mask)
{
#ifdef CONFIG_PCI
	if (dev->bus == &pci_bus_type)
		return pci_dma_supported(to_pci_dev(dev), mask);
#endif
	return 0;
}
EXPORT_SYMBOL(dma_supported);

int dma_set_mask(struct device *dev, u64 dma_mask)
{
#ifdef CONFIG_PCI
	if (dev->bus == &pci_bus_type)
		return pci_set_dma_mask(to_pci_dev(dev), dma_mask);
#endif
	return -EOPNOTSUPP;
}
EXPORT_SYMBOL(dma_set_mask);

void *dma_alloc_coherent(struct device *dev, size_t size,
			 dma_addr_t *dma_handle, gfp_t flag)
{
#ifdef CONFIG_PCI
	if (dev->bus == &pci_bus_type)
		return pci_alloc_consistent(to_pci_dev(dev), size, dma_handle);
#endif
	return sbus_alloc_consistent(dev, size, dma_handle);
}
EXPORT_SYMBOL(dma_alloc_coherent);

void dma_free_coherent(struct device *dev, size_t size,
		       void *cpu_addr, dma_addr_t dma_handle)
{
#ifdef CONFIG_PCI
	if (dev->bus == &pci_bus_type) {
		pci_free_consistent(to_pci_dev(dev), size,
				    cpu_addr, dma_handle);
		return;
	}
#endif
	sbus_free_consistent(dev, size, cpu_addr, dma_handle);
}
EXPORT_SYMBOL(dma_free_coherent);

dma_addr_t dma_map_single(struct device *dev, void *cpu_addr,
			  size_t size, enum dma_data_direction direction)
{
#ifdef CONFIG_PCI
	if (dev->bus == &pci_bus_type)
		return pci_map_single(to_pci_dev(dev), cpu_addr,
				      size, (int)direction);
#endif
	return sbus_map_single(dev, cpu_addr, size, (int)direction);
}
EXPORT_SYMBOL(dma_map_single);

void dma_unmap_single(struct device *dev, dma_addr_t dma_addr,
		      size_t size,
		      enum dma_data_direction direction)
{
#ifdef CONFIG_PCI
	if (dev->bus == &pci_bus_type) {
		pci_unmap_single(to_pci_dev(dev), dma_addr,
				 size, (int)direction);
		return;
	}
#endif
	sbus_unmap_single(dev, dma_addr, size, (int)direction);
}
EXPORT_SYMBOL(dma_unmap_single);

dma_addr_t dma_map_page(struct device *dev, struct page *page,
			unsigned long offset, size_t size,
			enum dma_data_direction direction)
{
#ifdef CONFIG_PCI
	if (dev->bus == &pci_bus_type)
		return pci_map_page(to_pci_dev(dev), page, offset,
				    size, (int)direction);
#endif
	return sbus_map_single(dev, page_address(page) + offset,
			       size, (int)direction);
}
EXPORT_SYMBOL(dma_map_page);

void dma_unmap_page(struct device *dev, dma_addr_t dma_address,
		    size_t size, enum dma_data_direction direction)
{
#ifdef CONFIG_PCI
	if (dev->bus == &pci_bus_type) {
		pci_unmap_page(to_pci_dev(dev), dma_address,
			       size, (int)direction);
		return;
	}
#endif
	sbus_unmap_single(dev, dma_address, size, (int)direction);
}
EXPORT_SYMBOL(dma_unmap_page);

int dma_map_sg(struct device *dev, struct scatterlist *sg,
			     int nents, enum dma_data_direction direction)
{
#ifdef CONFIG_PCI
	if (dev->bus == &pci_bus_type)
		return pci_map_sg(to_pci_dev(dev), sg, nents, (int)direction);
#endif
	return sbus_map_sg(dev, sg, nents, direction);
}
EXPORT_SYMBOL(dma_map_sg);

void dma_unmap_sg(struct device *dev, struct scatterlist *sg,
		  int nents, enum dma_data_direction direction)
{
#ifdef CONFIG_PCI
	if (dev->bus == &pci_bus_type) {
		pci_unmap_sg(to_pci_dev(dev), sg, nents, (int)direction);
		return;
	}
#endif
	sbus_unmap_sg(dev, sg, nents, (int)direction);
}
EXPORT_SYMBOL(dma_unmap_sg);

void dma_sync_single_for_cpu(struct device *dev, dma_addr_t dma_handle,
			     size_t size, enum dma_data_direction direction)
{
#ifdef CONFIG_PCI
	if (dev->bus == &pci_bus_type) {
		pci_dma_sync_single_for_cpu(to_pci_dev(dev), dma_handle,
					    size, (int)direction);
		return;
	}
#endif
	sbus_dma_sync_single_for_cpu(dev, dma_handle, size, (int) direction);
}
EXPORT_SYMBOL(dma_sync_single_for_cpu);

void dma_sync_single_for_device(struct device *dev, dma_addr_t dma_handle,
				size_t size, enum dma_data_direction direction)
{
#ifdef CONFIG_PCI
	if (dev->bus == &pci_bus_type) {
		pci_dma_sync_single_for_device(to_pci_dev(dev), dma_handle,
					       size, (int)direction);
		return;
	}
#endif
	sbus_dma_sync_single_for_device(dev, dma_handle, size, (int) direction);
}
EXPORT_SYMBOL(dma_sync_single_for_device);

void dma_sync_single_range_for_cpu(struct device *dev,
				   dma_addr_t dma_handle,
				   unsigned long offset,
				   size_t size,
				   enum dma_data_direction direction)
{
	dma_sync_single_for_cpu(dev, dma_handle+offset, size, direction);
}
EXPORT_SYMBOL(dma_sync_single_range_for_cpu);

void dma_sync_single_range_for_device(struct device *dev, dma_addr_t dma_handle,
				      unsigned long offset, size_t size,
				      enum dma_data_direction direction)
{
	dma_sync_single_for_device(dev, dma_handle+offset, size, direction);
}
EXPORT_SYMBOL(dma_sync_single_range_for_device);

void dma_sync_sg_for_cpu(struct device *dev, struct scatterlist *sg,
			 int nelems, enum dma_data_direction direction)
{
#ifdef CONFIG_PCI
	if (dev->bus == &pci_bus_type) {
		pci_dma_sync_sg_for_cpu(to_pci_dev(dev), sg,
					nelems, (int)direction);
		return;
	}
#endif
	BUG();
}
EXPORT_SYMBOL(dma_sync_sg_for_cpu);

void dma_sync_sg_for_device(struct device *dev,
			    struct scatterlist *sg, int nelems,
			    enum dma_data_direction direction)
{
#ifdef CONFIG_PCI
	if (dev->bus == &pci_bus_type) {
		pci_dma_sync_sg_for_device(to_pci_dev(dev), sg,
					   nelems, (int)direction);
		return;
	}
#endif
	BUG();
}
EXPORT_SYMBOL(dma_sync_sg_for_device);

int dma_mapping_error(struct device *dev, dma_addr_t dma_addr)
{
	return (dma_addr == DMA_ERROR_CODE);
}
EXPORT_SYMBOL(dma_mapping_error);

int dma_get_cache_alignment(void)
{
	return 32;
}
EXPORT_SYMBOL(dma_get_cache_alignment);
