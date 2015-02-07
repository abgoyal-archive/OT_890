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

/* MN10300 Dynamic DMA mapping support
 *
 * Copyright (C) 2007 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 * Derived from: arch/i386/kernel/pci-dma.c
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version
 * 2 of the Licence, or (at your option) any later version.
 */

#include <linux/types.h>
#include <linux/mm.h>
#include <linux/string.h>
#include <linux/pci.h>
#include <asm/io.h>

void *dma_alloc_coherent(struct device *dev, size_t size,
			 dma_addr_t *dma_handle, int gfp)
{
	unsigned long addr;
	void *ret;

	/* ignore region specifiers */
	gfp &= ~(__GFP_DMA | __GFP_HIGHMEM);

	if (dev == NULL || dev->coherent_dma_mask < 0xffffffff)
		gfp |= GFP_DMA;

	addr = __get_free_pages(gfp, get_order(size));
	if (!addr)
		return NULL;

	/* map the coherent memory through the uncached memory window */
	ret = (void *) (addr | 0x20000000);

	/* fill the memory with obvious rubbish */
	memset((void *) addr, 0xfb, size);

	/* write back and evict all cache lines covering this region */
	mn10300_dcache_flush_inv_range2(virt_to_phys((void *) addr), PAGE_SIZE);

	*dma_handle = virt_to_bus((void *) addr);
	return ret;
}
EXPORT_SYMBOL(dma_alloc_coherent);

void dma_free_coherent(struct device *dev, size_t size, void *vaddr,
		       dma_addr_t dma_handle)
{
	unsigned long addr = (unsigned long) vaddr & ~0x20000000;

	free_pages(addr, get_order(size));
}
EXPORT_SYMBOL(dma_free_coherent);
