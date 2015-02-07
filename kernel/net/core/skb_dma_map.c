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

/* skb_dma_map.c: DMA mapping helpers for socket buffers.
 *
 * Copyright (C) David S. Miller <davem@davemloft.net>
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/dma-mapping.h>
#include <linux/skbuff.h>

int skb_dma_map(struct device *dev, struct sk_buff *skb,
		enum dma_data_direction dir)
{
	struct skb_shared_info *sp = skb_shinfo(skb);
	dma_addr_t map;
	int i;

	map = dma_map_single(dev, skb->data,
			     skb_headlen(skb), dir);
	if (dma_mapping_error(dev, map))
		goto out_err;

	sp->dma_maps[0] = map;
	for (i = 0; i < sp->nr_frags; i++) {
		skb_frag_t *fp = &sp->frags[i];

		map = dma_map_page(dev, fp->page, fp->page_offset,
				   fp->size, dir);
		if (dma_mapping_error(dev, map))
			goto unwind;
		sp->dma_maps[i + 1] = map;
	}
	sp->num_dma_maps = i + 1;

	return 0;

unwind:
	while (--i >= 0) {
		skb_frag_t *fp = &sp->frags[i];

		dma_unmap_page(dev, sp->dma_maps[i + 1],
			       fp->size, dir);
	}
	dma_unmap_single(dev, sp->dma_maps[0],
			 skb_headlen(skb), dir);
out_err:
	return -ENOMEM;
}
EXPORT_SYMBOL(skb_dma_map);

void skb_dma_unmap(struct device *dev, struct sk_buff *skb,
		   enum dma_data_direction dir)
{
	struct skb_shared_info *sp = skb_shinfo(skb);
	int i;

	dma_unmap_single(dev, sp->dma_maps[0],
			 skb_headlen(skb), dir);
	for (i = 0; i < sp->nr_frags; i++) {
		skb_frag_t *fp = &sp->frags[i];

		dma_unmap_page(dev, sp->dma_maps[i + 1],
			       fp->size, dir);
	}
}
EXPORT_SYMBOL(skb_dma_unmap);
