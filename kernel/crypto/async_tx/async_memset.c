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
 * memory fill offload engine support
 *
 * Copyright © 2006, Intel Corporation.
 *
 *      Dan Williams <dan.j.williams@intel.com>
 *
 *      with architecture considerations by:
 *      Neil Brown <neilb@suse.de>
 *      Jeff Garzik <jeff@garzik.org>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/mm.h>
#include <linux/dma-mapping.h>
#include <linux/async_tx.h>

/**
 * async_memset - attempt to fill memory with a dma engine.
 * @dest: destination page
 * @val: fill value
 * @offset: offset in pages to start transaction
 * @len: length in bytes
 * @flags: ASYNC_TX_ACK, ASYNC_TX_DEP_ACK
 * @depend_tx: memset depends on the result of this transaction
 * @cb_fn: function to call when the memcpy completes
 * @cb_param: parameter to pass to the callback routine
 */
struct dma_async_tx_descriptor *
async_memset(struct page *dest, int val, unsigned int offset,
	size_t len, enum async_tx_flags flags,
	struct dma_async_tx_descriptor *depend_tx,
	dma_async_tx_callback cb_fn, void *cb_param)
{
	struct dma_chan *chan = async_tx_find_channel(depend_tx, DMA_MEMSET,
						      &dest, 1, NULL, 0, len);
	struct dma_device *device = chan ? chan->device : NULL;
	struct dma_async_tx_descriptor *tx = NULL;

	if (device) {
		dma_addr_t dma_dest;
		unsigned long dma_prep_flags = cb_fn ? DMA_PREP_INTERRUPT : 0;

		dma_dest = dma_map_page(device->dev, dest, offset, len,
					DMA_FROM_DEVICE);

		tx = device->device_prep_dma_memset(chan, dma_dest, val, len,
						    dma_prep_flags);
	}

	if (tx) {
		pr_debug("%s: (async) len: %zu\n", __func__, len);
		async_tx_submit(chan, tx, flags, depend_tx, cb_fn, cb_param);
	} else { /* run the memset synchronously */
		void *dest_buf;
		pr_debug("%s: (sync) len: %zu\n", __func__, len);

		dest_buf = (void *) (((char *) page_address(dest)) + offset);

		/* wait for any prerequisite operations */
		async_tx_quiesce(&depend_tx);

		memset(dest_buf, val, len);

		async_tx_sync_epilog(cb_fn, cb_param);
	}

	return tx;
}
EXPORT_SYMBOL_GPL(async_memset);

static int __init async_memset_init(void)
{
	return 0;
}

static void __exit async_memset_exit(void)
{
	do { } while (0);
}

module_init(async_memset_init);
module_exit(async_memset_exit);

MODULE_AUTHOR("Intel Corporation");
MODULE_DESCRIPTION("asynchronous memset api");
MODULE_LICENSE("GPL");
