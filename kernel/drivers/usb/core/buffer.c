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
 * DMA memory management for framework level HCD code (hc_driver)
 *
 * This implementation plugs in through generic "usb_bus" level methods,
 * and should work with all USB controllers, regardles of bus type.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/mm.h>
#include <asm/io.h>
#include <linux/dma-mapping.h>
#include <linux/dmapool.h>
#include <linux/usb.h>
#include "hcd.h"


/*
 * DMA-Coherent Buffers
 */

/* FIXME tune these based on pool statistics ... */
static const size_t	pool_max [HCD_BUFFER_POOLS] = {
	/* platforms without dma-friendly caches might need to
	 * prevent cacheline sharing...
	 */
	32,
	128,
	512,
	PAGE_SIZE / 2
	/* bigger --> allocate pages */
};


/* SETUP primitives */

/**
 * hcd_buffer_create - initialize buffer pools
 * @hcd: the bus whose buffer pools are to be initialized
 * Context: !in_interrupt()
 *
 * Call this as part of initializing a host controller that uses the dma
 * memory allocators.  It initializes some pools of dma-coherent memory that
 * will be shared by all drivers using that controller, or returns a negative
 * errno value on error.
 *
 * Call hcd_buffer_destroy() to clean up after using those pools.
 */
int hcd_buffer_create(struct usb_hcd *hcd)
{
	char		name[16];
	int 		i, size;

	if (!hcd->self.controller->dma_mask &&
	    !(hcd->driver->flags & HCD_LOCAL_MEM))
		return 0;

	for (i = 0; i < HCD_BUFFER_POOLS; i++) {
		size = pool_max[i];
		if (!size)
			continue;
		snprintf(name, sizeof name, "buffer-%d", size);
		hcd->pool[i] = dma_pool_create(name, hcd->self.controller,
				size, size, 0);
		if (!hcd->pool [i]) {
			hcd_buffer_destroy(hcd);
			return -ENOMEM;
		}
	}
	return 0;
}


/**
 * hcd_buffer_destroy - deallocate buffer pools
 * @hcd: the bus whose buffer pools are to be destroyed
 * Context: !in_interrupt()
 *
 * This frees the buffer pools created by hcd_buffer_create().
 */
void hcd_buffer_destroy(struct usb_hcd *hcd)
{
	int i;

	for (i = 0; i < HCD_BUFFER_POOLS; i++) {
		struct dma_pool *pool = hcd->pool[i];
		if (pool) {
			dma_pool_destroy(pool);
			hcd->pool[i] = NULL;
		}
	}
}


/* sometimes alloc/free could use kmalloc with GFP_DMA, for
 * better sharing and to leverage mm/slab.c intelligence.
 */

void *hcd_buffer_alloc(
	struct usb_bus 	*bus,
	size_t			size,
	gfp_t			mem_flags,
	dma_addr_t		*dma
)
{
	struct usb_hcd		*hcd = bus_to_hcd(bus);
	int 			i;

	/* some USB hosts just use PIO */
	if (!bus->controller->dma_mask &&
	    !(hcd->driver->flags & HCD_LOCAL_MEM)) {
		*dma = ~(dma_addr_t) 0;
		return kmalloc(size, mem_flags);
	}

	for (i = 0; i < HCD_BUFFER_POOLS; i++) {
		if (size <= pool_max [i])
			return dma_pool_alloc(hcd->pool [i], mem_flags, dma);
	}
	return dma_alloc_coherent(hcd->self.controller, size, dma, 0);
}

void hcd_buffer_free(
	struct usb_bus 	*bus,
	size_t			size,
	void 			*addr,
	dma_addr_t		dma
)
{
	struct usb_hcd		*hcd = bus_to_hcd(bus);
	int 			i;

	if (!addr)
		return;

	if (!bus->controller->dma_mask &&
	    !(hcd->driver->flags & HCD_LOCAL_MEM)) {
		kfree(addr);
		return;
	}

	for (i = 0; i < HCD_BUFFER_POOLS; i++) {
		if (size <= pool_max [i]) {
			dma_pool_free(hcd->pool [i], addr, dma);
			return;
		}
	}
	dma_free_coherent(hcd->self.controller, size, addr, dma);
}
