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
 * DMA region bookkeeping routines
 *
 * Copyright (C) 2002 Maas Digital LLC
 *
 * This code is licensed under the GPL.  See the file COPYING in the root
 * directory of the kernel sources for details.
 */

#ifndef IEEE1394_DMA_H
#define IEEE1394_DMA_H

#include <asm/types.h>

struct file;
struct pci_dev;
struct scatterlist;
struct vm_area_struct;

/**
 * struct dma_prog_region - small contiguous DMA buffer
 * @kvirt:    kernel virtual address
 * @dev:      PCI device
 * @n_pages:  number of kernel pages
 * @bus_addr: base bus address
 *
 * a small, physically contiguous DMA buffer with random-access, synchronous
 * usage characteristics
 */
struct dma_prog_region {
	unsigned char *kvirt;
	struct pci_dev *dev;
	unsigned int n_pages;
	dma_addr_t bus_addr;
};

/* clear out all fields but do not allocate any memory */
void dma_prog_region_init(struct dma_prog_region *prog);
int dma_prog_region_alloc(struct dma_prog_region *prog, unsigned long n_bytes,
			  struct pci_dev *dev);
void dma_prog_region_free(struct dma_prog_region *prog);

static inline dma_addr_t dma_prog_region_offset_to_bus(
		struct dma_prog_region *prog, unsigned long offset)
{
	return prog->bus_addr + offset;
}

/**
 * struct dma_region - large non-contiguous DMA buffer
 * @virt:        kernel virtual address
 * @dev:         PCI device
 * @n_pages:     number of kernel pages
 * @n_dma_pages: number of IOMMU pages
 * @sglist:      IOMMU mapping
 * @direction:   PCI_DMA_TODEVICE, etc.
 *
 * a large, non-physically-contiguous DMA buffer with streaming, asynchronous
 * usage characteristics
 */
struct dma_region {
	unsigned char *kvirt;
	struct pci_dev *dev;
	unsigned int n_pages;
	unsigned int n_dma_pages;
	struct scatterlist *sglist;
	int direction;
};

void dma_region_init(struct dma_region *dma);
int dma_region_alloc(struct dma_region *dma, unsigned long n_bytes,
		     struct pci_dev *dev, int direction);
void dma_region_free(struct dma_region *dma);
void dma_region_sync_for_cpu(struct dma_region *dma, unsigned long offset,
			     unsigned long len);
void dma_region_sync_for_device(struct dma_region *dma, unsigned long offset,
				unsigned long len);
int  dma_region_mmap(struct dma_region *dma, struct file *file,
		     struct vm_area_struct *vma);
dma_addr_t dma_region_offset_to_bus(struct dma_region *dma,
				    unsigned long offset);

/**
 * dma_region_i - macro to index into a DMA region (or dma_prog_region)
 */
#define dma_region_i(_dma, _type, _index) \
	( ((_type*) ((_dma)->kvirt)) + (_index) )

#endif /* IEEE1394_DMA_H */
