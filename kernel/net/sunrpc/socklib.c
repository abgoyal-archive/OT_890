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
 * linux/net/sunrpc/socklib.c
 *
 * Common socket helper routines for RPC client and server
 *
 * Copyright (C) 1995, 1996 Olaf Kirch <okir@monad.swb.de>
 */

#include <linux/compiler.h>
#include <linux/netdevice.h>
#include <linux/skbuff.h>
#include <linux/types.h>
#include <linux/pagemap.h>
#include <linux/udp.h>
#include <linux/sunrpc/xdr.h>


/**
 * xdr_skb_read_bits - copy some data bits from skb to internal buffer
 * @desc: sk_buff copy helper
 * @to: copy destination
 * @len: number of bytes to copy
 *
 * Possibly called several times to iterate over an sk_buff and copy
 * data out of it.
 */
size_t xdr_skb_read_bits(struct xdr_skb_reader *desc, void *to, size_t len)
{
	if (len > desc->count)
		len = desc->count;
	if (unlikely(skb_copy_bits(desc->skb, desc->offset, to, len)))
		return 0;
	desc->count -= len;
	desc->offset += len;
	return len;
}
EXPORT_SYMBOL_GPL(xdr_skb_read_bits);

/**
 * xdr_skb_read_and_csum_bits - copy and checksum from skb to buffer
 * @desc: sk_buff copy helper
 * @to: copy destination
 * @len: number of bytes to copy
 *
 * Same as skb_read_bits, but calculate a checksum at the same time.
 */
static size_t xdr_skb_read_and_csum_bits(struct xdr_skb_reader *desc, void *to, size_t len)
{
	unsigned int pos;
	__wsum csum2;

	if (len > desc->count)
		len = desc->count;
	pos = desc->offset;
	csum2 = skb_copy_and_csum_bits(desc->skb, pos, to, len, 0);
	desc->csum = csum_block_add(desc->csum, csum2, pos);
	desc->count -= len;
	desc->offset += len;
	return len;
}

/**
 * xdr_partial_copy_from_skb - copy data out of an skb
 * @xdr: target XDR buffer
 * @base: starting offset
 * @desc: sk_buff copy helper
 * @copy_actor: virtual method for copying data
 *
 */
ssize_t xdr_partial_copy_from_skb(struct xdr_buf *xdr, unsigned int base, struct xdr_skb_reader *desc, xdr_skb_read_actor copy_actor)
{
	struct page	**ppage = xdr->pages;
	unsigned int	len, pglen = xdr->page_len;
	ssize_t		copied = 0;
	size_t		ret;

	len = xdr->head[0].iov_len;
	if (base < len) {
		len -= base;
		ret = copy_actor(desc, (char *)xdr->head[0].iov_base + base, len);
		copied += ret;
		if (ret != len || !desc->count)
			goto out;
		base = 0;
	} else
		base -= len;

	if (unlikely(pglen == 0))
		goto copy_tail;
	if (unlikely(base >= pglen)) {
		base -= pglen;
		goto copy_tail;
	}
	if (base || xdr->page_base) {
		pglen -= base;
		base += xdr->page_base;
		ppage += base >> PAGE_CACHE_SHIFT;
		base &= ~PAGE_CACHE_MASK;
	}
	do {
		char *kaddr;

		/* ACL likes to be lazy in allocating pages - ACLs
		 * are small by default but can get huge. */
		if (unlikely(*ppage == NULL)) {
			*ppage = alloc_page(GFP_ATOMIC);
			if (unlikely(*ppage == NULL)) {
				if (copied == 0)
					copied = -ENOMEM;
				goto out;
			}
		}

		len = PAGE_CACHE_SIZE;
		kaddr = kmap_atomic(*ppage, KM_SKB_SUNRPC_DATA);
		if (base) {
			len -= base;
			if (pglen < len)
				len = pglen;
			ret = copy_actor(desc, kaddr + base, len);
			base = 0;
		} else {
			if (pglen < len)
				len = pglen;
			ret = copy_actor(desc, kaddr, len);
		}
		flush_dcache_page(*ppage);
		kunmap_atomic(kaddr, KM_SKB_SUNRPC_DATA);
		copied += ret;
		if (ret != len || !desc->count)
			goto out;
		ppage++;
	} while ((pglen -= len) != 0);
copy_tail:
	len = xdr->tail[0].iov_len;
	if (base < len)
		copied += copy_actor(desc, (char *)xdr->tail[0].iov_base + base, len - base);
out:
	return copied;
}
EXPORT_SYMBOL_GPL(xdr_partial_copy_from_skb);

/**
 * csum_partial_copy_to_xdr - checksum and copy data
 * @xdr: target XDR buffer
 * @skb: source skb
 *
 * We have set things up such that we perform the checksum of the UDP
 * packet in parallel with the copies into the RPC client iovec.  -DaveM
 */
int csum_partial_copy_to_xdr(struct xdr_buf *xdr, struct sk_buff *skb)
{
	struct xdr_skb_reader	desc;

	desc.skb = skb;
	desc.offset = sizeof(struct udphdr);
	desc.count = skb->len - desc.offset;

	if (skb_csum_unnecessary(skb))
		goto no_checksum;

	desc.csum = csum_partial(skb->data, desc.offset, skb->csum);
	if (xdr_partial_copy_from_skb(xdr, 0, &desc, xdr_skb_read_and_csum_bits) < 0)
		return -1;
	if (desc.offset != skb->len) {
		__wsum csum2;
		csum2 = skb_checksum(skb, desc.offset, skb->len - desc.offset, 0);
		desc.csum = csum_block_add(desc.csum, csum2, desc.offset);
	}
	if (desc.count)
		return -1;
	if (csum_fold(desc.csum))
		return -1;
	if (unlikely(skb->ip_summed == CHECKSUM_COMPLETE))
		netdev_rx_csum_fault(skb->dev);
	return 0;
no_checksum:
	if (xdr_partial_copy_from_skb(xdr, 0, &desc, xdr_skb_read_bits) < 0)
		return -1;
	if (desc.count)
		return -1;
	return 0;
}
EXPORT_SYMBOL_GPL(csum_partial_copy_to_xdr);
