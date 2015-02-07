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
 * Copyright(c) 2004 - 2006 Intel Corporation. All rights reserved.
 * Portions based on net/core/datagram.c and copyrighted by their authors.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 59
 * Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * The full GNU General Public License is included in this distribution in the
 * file called COPYING.
 */

/*
 * This code allows the net stack to make use of a DMA engine for
 * skb to iovec copies.
 */

#include <linux/dmaengine.h>
#include <linux/socket.h>
#include <net/tcp.h>
#include <net/netdma.h>

#define NET_DMA_DEFAULT_COPYBREAK 4096

int sysctl_tcp_dma_copybreak = NET_DMA_DEFAULT_COPYBREAK;
EXPORT_SYMBOL(sysctl_tcp_dma_copybreak);

/**
 *	dma_skb_copy_datagram_iovec - Copy a datagram to an iovec.
 *	@skb - buffer to copy
 *	@offset - offset in the buffer to start copying from
 *	@iovec - io vector to copy to
 *	@len - amount of data to copy from buffer to iovec
 *	@pinned_list - locked iovec buffer data
 *
 *	Note: the iovec is modified during the copy.
 */
int dma_skb_copy_datagram_iovec(struct dma_chan *chan,
			struct sk_buff *skb, int offset, struct iovec *to,
			size_t len, struct dma_pinned_list *pinned_list)
{
	int start = skb_headlen(skb);
	int i, copy = start - offset;
	dma_cookie_t cookie = 0;

	/* Copy header. */
	if (copy > 0) {
		if (copy > len)
			copy = len;
		cookie = dma_memcpy_to_iovec(chan, to, pinned_list,
					    skb->data + offset, copy);
		if (cookie < 0)
			goto fault;
		len -= copy;
		if (len == 0)
			goto end;
		offset += copy;
	}

	/* Copy paged appendix. Hmm... why does this look so complicated? */
	for (i = 0; i < skb_shinfo(skb)->nr_frags; i++) {
		int end;

		WARN_ON(start > offset + len);

		end = start + skb_shinfo(skb)->frags[i].size;
		copy = end - offset;
		if (copy > 0) {
			skb_frag_t *frag = &skb_shinfo(skb)->frags[i];
			struct page *page = frag->page;

			if (copy > len)
				copy = len;

			cookie = dma_memcpy_pg_to_iovec(chan, to, pinned_list, page,
					frag->page_offset + offset - start, copy);
			if (cookie < 0)
				goto fault;
			len -= copy;
			if (len == 0)
				goto end;
			offset += copy;
		}
		start = end;
	}

	if (skb_shinfo(skb)->frag_list) {
		struct sk_buff *list = skb_shinfo(skb)->frag_list;

		for (; list; list = list->next) {
			int end;

			WARN_ON(start > offset + len);

			end = start + list->len;
			copy = end - offset;
			if (copy > 0) {
				if (copy > len)
					copy = len;
				cookie = dma_skb_copy_datagram_iovec(chan, list,
						offset - start, to, copy,
						pinned_list);
				if (cookie < 0)
					goto fault;
				len -= copy;
				if (len == 0)
					goto end;
				offset += copy;
			}
			start = end;
		}
	}

end:
	if (!len) {
		skb->dma_cookie = cookie;
		return cookie;
	}

fault:
	return -EFAULT;
}
