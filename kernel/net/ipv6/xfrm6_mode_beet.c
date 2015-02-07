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
 * xfrm6_mode_beet.c - BEET mode encapsulation for IPv6.
 *
 * Copyright (c) 2006 Diego Beltrami <diego.beltrami@gmail.com>
 *                    Miika Komu     <miika@iki.fi>
 *                    Herbert Xu     <herbert@gondor.apana.org.au>
 *                    Abhinav Pathak <abhinav.pathak@hiit.fi>
 *                    Jeff Ahrenholz <ahrenholz@gmail.com>
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/stringify.h>
#include <net/dsfield.h>
#include <net/dst.h>
#include <net/inet_ecn.h>
#include <net/ipv6.h>
#include <net/xfrm.h>

static void xfrm6_beet_make_header(struct sk_buff *skb)
{
	struct ipv6hdr *iph = ipv6_hdr(skb);

	iph->version = 6;

	memcpy(iph->flow_lbl, XFRM_MODE_SKB_CB(skb)->flow_lbl,
	       sizeof(iph->flow_lbl));
	iph->nexthdr = XFRM_MODE_SKB_CB(skb)->protocol;

	ipv6_change_dsfield(iph, 0, XFRM_MODE_SKB_CB(skb)->tos);
	iph->hop_limit = XFRM_MODE_SKB_CB(skb)->ttl;
}

/* Add encapsulation header.
 *
 * The top IP header will be constructed per draft-nikander-esp-beet-mode-06.txt.
 */
static int xfrm6_beet_output(struct xfrm_state *x, struct sk_buff *skb)
{
	struct ipv6hdr *top_iph;
	struct ip_beet_phdr *ph;
	struct iphdr *iphv4;
	int optlen, hdr_len;

	iphv4 = ip_hdr(skb);
	hdr_len = 0;
	optlen = XFRM_MODE_SKB_CB(skb)->optlen;
	if (unlikely(optlen))
		hdr_len += IPV4_BEET_PHMAXLEN - (optlen & 4);

	skb_set_network_header(skb, -x->props.header_len - hdr_len);
	if (x->sel.family != AF_INET6)
		skb->network_header += IPV4_BEET_PHMAXLEN;
	skb->mac_header = skb->network_header +
			  offsetof(struct ipv6hdr, nexthdr);
	skb->transport_header = skb->network_header + sizeof(*top_iph);
	ph = (struct ip_beet_phdr *)__skb_pull(skb, XFRM_MODE_SKB_CB(skb)->ihl-hdr_len);

	xfrm6_beet_make_header(skb);

	top_iph = ipv6_hdr(skb);
	if (unlikely(optlen)) {

		BUG_ON(optlen < 0);

		ph->padlen = 4 - (optlen & 4);
		ph->hdrlen = optlen / 8;
		ph->nexthdr = top_iph->nexthdr;
		if (ph->padlen)
			memset(ph + 1, IPOPT_NOP, ph->padlen);

		top_iph->nexthdr = IPPROTO_BEETPH;
	}

	ipv6_addr_copy(&top_iph->saddr, (struct in6_addr *)&x->props.saddr);
	ipv6_addr_copy(&top_iph->daddr, (struct in6_addr *)&x->id.daddr);
	return 0;
}

static int xfrm6_beet_input(struct xfrm_state *x, struct sk_buff *skb)
{
	struct ipv6hdr *ip6h;
	const unsigned char *old_mac;
	int size = sizeof(struct ipv6hdr);
	int err;

	err = skb_cow_head(skb, size + skb->mac_len);
	if (err)
		goto out;

	__skb_push(skb, size);
	skb_reset_network_header(skb);

	old_mac = skb_mac_header(skb);
	skb_set_mac_header(skb, -skb->mac_len);
	memmove(skb_mac_header(skb), old_mac, skb->mac_len);

	xfrm6_beet_make_header(skb);

	ip6h = ipv6_hdr(skb);
	ip6h->payload_len = htons(skb->len - size);
	ipv6_addr_copy(&ip6h->daddr, (struct in6_addr *) &x->sel.daddr.a6);
	ipv6_addr_copy(&ip6h->saddr, (struct in6_addr *) &x->sel.saddr.a6);
	err = 0;
out:
	return err;
}

static struct xfrm_mode xfrm6_beet_mode = {
	.input2 = xfrm6_beet_input,
	.input = xfrm_prepare_input,
	.output2 = xfrm6_beet_output,
	.output = xfrm6_prepare_output,
	.owner = THIS_MODULE,
	.encap = XFRM_MODE_BEET,
	.flags = XFRM_MODE_FLAG_TUNNEL,
};

static int __init xfrm6_beet_init(void)
{
	return xfrm_register_mode(&xfrm6_beet_mode, AF_INET6);
}

static void __exit xfrm6_beet_exit(void)
{
	int err;

	err = xfrm_unregister_mode(&xfrm6_beet_mode, AF_INET6);
	BUG_ON(err);
}

module_init(xfrm6_beet_init);
module_exit(xfrm6_beet_exit);
MODULE_LICENSE("GPL");
MODULE_ALIAS_XFRM_MODE(AF_INET6, XFRM_MODE_BEET);
