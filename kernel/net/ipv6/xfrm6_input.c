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
 * xfrm6_input.c: based on net/ipv4/xfrm4_input.c
 *
 * Authors:
 *	Mitsuru KANDA @USAGI
 * 	Kazunori MIYAZAWA @USAGI
 * 	Kunihiro Ishiguro <kunihiro@ipinfusion.com>
 *	YOSHIFUJI Hideaki @USAGI
 *		IPv6 support
 */

#include <linux/module.h>
#include <linux/string.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv6.h>
#include <net/ipv6.h>
#include <net/xfrm.h>

int xfrm6_extract_input(struct xfrm_state *x, struct sk_buff *skb)
{
	return xfrm6_extract_header(skb);
}

int xfrm6_rcv_spi(struct sk_buff *skb, int nexthdr, __be32 spi)
{
	XFRM_SPI_SKB_CB(skb)->family = AF_INET6;
	XFRM_SPI_SKB_CB(skb)->daddroff = offsetof(struct ipv6hdr, daddr);
	return xfrm_input(skb, nexthdr, spi, 0);
}
EXPORT_SYMBOL(xfrm6_rcv_spi);

int xfrm6_transport_finish(struct sk_buff *skb, int async)
{
	skb_network_header(skb)[IP6CB(skb)->nhoff] =
		XFRM_MODE_SKB_CB(skb)->protocol;

#ifndef CONFIG_NETFILTER
	if (!async)
		return 1;
#endif

	ipv6_hdr(skb)->payload_len = htons(skb->len);
	__skb_push(skb, skb->data - skb_network_header(skb));

	NF_HOOK(PF_INET6, NF_INET_PRE_ROUTING, skb, skb->dev, NULL,
		ip6_rcv_finish);
	return -1;
}

int xfrm6_rcv(struct sk_buff *skb)
{
	return xfrm6_rcv_spi(skb, skb_network_header(skb)[IP6CB(skb)->nhoff],
			     0);
}

EXPORT_SYMBOL(xfrm6_rcv);

int xfrm6_input_addr(struct sk_buff *skb, xfrm_address_t *daddr,
		     xfrm_address_t *saddr, u8 proto)
{
	struct net *net = dev_net(skb->dev);
	struct xfrm_state *x = NULL;
	int i = 0;

	/* Allocate new secpath or COW existing one. */
	if (!skb->sp || atomic_read(&skb->sp->refcnt) != 1) {
		struct sec_path *sp;

		sp = secpath_dup(skb->sp);
		if (!sp) {
			XFRM_INC_STATS(net, LINUX_MIB_XFRMINERROR);
			goto drop;
		}
		if (skb->sp)
			secpath_put(skb->sp);
		skb->sp = sp;
	}

	if (1 + skb->sp->len == XFRM_MAX_DEPTH) {
		XFRM_INC_STATS(net, LINUX_MIB_XFRMINBUFFERERROR);
		goto drop;
	}

	for (i = 0; i < 3; i++) {
		xfrm_address_t *dst, *src;

		switch (i) {
		case 0:
			dst = daddr;
			src = saddr;
			break;
		case 1:
			/* lookup state with wild-card source address */
			dst = daddr;
			src = (xfrm_address_t *)&in6addr_any;
			break;
		default:
			/* lookup state with wild-card addresses */
			dst = (xfrm_address_t *)&in6addr_any;
			src = (xfrm_address_t *)&in6addr_any;
			break;
		}

		x = xfrm_state_lookup_byaddr(net, dst, src, proto, AF_INET6);
		if (!x)
			continue;

		spin_lock(&x->lock);

		if ((!i || (x->props.flags & XFRM_STATE_WILDRECV)) &&
		    likely(x->km.state == XFRM_STATE_VALID) &&
		    !xfrm_state_check_expire(x)) {
			spin_unlock(&x->lock);
			if (x->type->input(x, skb) > 0) {
				/* found a valid state */
				break;
			}
		} else
			spin_unlock(&x->lock);

		xfrm_state_put(x);
		x = NULL;
	}

	if (!x) {
		XFRM_INC_STATS(net, LINUX_MIB_XFRMINNOSTATES);
		xfrm_audit_state_notfound_simple(skb, AF_INET6);
		goto drop;
	}

	skb->sp->xvec[skb->sp->len++] = x;

	spin_lock(&x->lock);

	x->curlft.bytes += skb->len;
	x->curlft.packets++;

	spin_unlock(&x->lock);

	return 1;

drop:
	return -1;
}

EXPORT_SYMBOL(xfrm6_input_addr);
