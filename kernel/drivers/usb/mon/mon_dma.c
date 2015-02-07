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
 * The USB Monitor, inspired by Dave Harding's USBMon.
 *
 * mon_dma.c: Library which snoops on DMA areas.
 *
 * Copyright (C) 2005 Pete Zaitcev (zaitcev@redhat.com)
 */
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/highmem.h>
#include <asm/page.h>

#include <linux/usb.h>	/* Only needed for declarations in usb_mon.h */
#include "usb_mon.h"

/*
 * PC-compatibles, are, fortunately, sufficiently cache-coherent for this.
 */
#if defined(__i386__) || defined(__x86_64__) /* CONFIG_ARCH_I386 doesn't exit */
#define MON_HAS_UNMAP 1

#define phys_to_page(phys)	pfn_to_page((phys) >> PAGE_SHIFT)

char mon_dmapeek(unsigned char *dst, dma_addr_t dma_addr, int len)
{
	struct page *pg;
	unsigned long flags;
	unsigned char *map;
	unsigned char *ptr;

	/*
	 * On i386, a DMA handle is the "physical" address of a page.
	 * In other words, the bus address is equal to physical address.
	 * There is no IOMMU.
	 */
	pg = phys_to_page(dma_addr);

	/*
	 * We are called from hardware IRQs in case of callbacks.
	 * But we can be called from softirq or process context in case
	 * of submissions. In such case, we need to protect KM_IRQ0.
	 */
	local_irq_save(flags);
	map = kmap_atomic(pg, KM_IRQ0);
	ptr = map + (dma_addr & (PAGE_SIZE-1));
	memcpy(dst, ptr, len);
	kunmap_atomic(map, KM_IRQ0);
	local_irq_restore(flags);
	return 0;
}

void mon_dmapeek_vec(const struct mon_reader_bin *rp,
    unsigned int offset, dma_addr_t dma_addr, unsigned int length)
{
	unsigned long flags;
	unsigned int step_len;
	struct page *pg;
	unsigned char *map;
	unsigned long page_off, page_len;

	local_irq_save(flags);
	while (length) {
		/* compute number of bytes we are going to copy in this page */
		step_len = length;
		page_off = dma_addr & (PAGE_SIZE-1);
		page_len = PAGE_SIZE - page_off;
		if (page_len < step_len)
			step_len = page_len;

		/* copy data and advance pointers */
		pg = phys_to_page(dma_addr);
		map = kmap_atomic(pg, KM_IRQ0);
		offset = mon_copy_to_buff(rp, offset, map + page_off, step_len);
		kunmap_atomic(map, KM_IRQ0);
		dma_addr += step_len;
		length -= step_len;
	}
	local_irq_restore(flags);
}

#endif /* __i386__ */

#ifndef MON_HAS_UNMAP
char mon_dmapeek(unsigned char *dst, dma_addr_t dma_addr, int len)
{
	return 'D';
}

void mon_dmapeek_vec(const struct mon_reader_bin *rp,
    unsigned int offset, dma_addr_t dma_addr, unsigned int length)
{
	;
}

#endif /* MON_HAS_UNMAP */
