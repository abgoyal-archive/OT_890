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
 * xfrm4_output.c - Common IPsec encapsulation code for IPv4.
 * Copyright (c) 2004 Herbert Xu <herbert@gondor.apana.org.au>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#include <linux/if_ether.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/netfilter_ipv4.h>
#include <net/dst.h>
#include <net/ip.h>
#include <net/xfrm.h>
#include <net/icmp.h>

static int xfrm4_tunnel_check_size(struct sk_buff *skb)
{
	int mtu, ret = 0;
	struct dst_entry *dst;

	if (IPCB(skb)->flags & IPSKB_XFRM_TUNNEL_SIZE)
		goto out;

	if (!(ip_hdr(skb)->frag_off & htons(IP_DF)) || skb->local_df)
		goto out;

	dst = skb->dst;
	mtu = dst_mtu(dst);
	if (skb->len > mtu) {
		icmp_send(skb, ICMP_DEST_UNREACH, ICMP_FRAG_NEEDED, htonl(mtu));
		ret = -EMSGSIZE;
	}
out:
	return ret;
}

int xfrm4_extract_output(struct xfrm_state *x, struct sk_buff *skb)
{
	int err;

	err = xfrm4_tunnel_check_size(skb);
	if (err)
		return err;

	XFRM_MODE_SKB_CB(skb)->protocol = ip_hdr(skb)->protocol;

	return xfrm4_extract_header(skb);
}

int xfrm4_prepare_output(struct xfrm_state *x, struct sk_buff *skb)
{
	int err;

	err = xfrm_inner_extract_output(x, skb);
	if (err)
		return err;

	memset(IPCB(skb), 0, sizeof(*IPCB(skb)));
	IPCB(skb)->flags |= IPSKB_XFRM_TUNNEL_SIZE | IPSKB_XFRM_TRANSFORMED;

	skb->protocol = htons(ETH_P_IP);

	return x->outer_mode->output2(x, skb);
}
EXPORT_SYMBOL(xfrm4_prepare_output);

static int xfrm4_output_finish(struct sk_buff *skb)
{
#ifdef CONFIG_NETFILTER
	if (!skb->dst->xfrm) {
		IPCB(skb)->flags |= IPSKB_REROUTED;
		return dst_output(skb);
	}

	IPCB(skb)->flags |= IPSKB_XFRM_TRANSFORMED;
#endif

	skb->protocol = htons(ETH_P_IP);
	return xfrm_output(skb);
}

int xfrm4_output(struct sk_buff *skb)
{
	return NF_HOOK_COND(PF_INET, NF_INET_POST_ROUTING, skb,
			    NULL, skb->dst->dev, xfrm4_output_finish,
			    !(IPCB(skb)->flags & IPSKB_REROUTED));
}
