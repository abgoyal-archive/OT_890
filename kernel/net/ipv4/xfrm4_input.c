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
 * xfrm4_input.c
 *
 * Changes:
 *	YOSHIFUJI Hideaki @USAGI
 *		Split up af-specific portion
 *	Derek Atkins <derek@ihtfp.com>
 *		Add Encapsulation support
 *
 */

#include <linux/module.h>
#include <linux/string.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <net/ip.h>
#include <net/xfrm.h>

int xfrm4_extract_input(struct xfrm_state *x, struct sk_buff *skb)
{
	return xfrm4_extract_header(skb);
}

static inline int xfrm4_rcv_encap_finish(struct sk_buff *skb)
{
	if (skb->dst == NULL) {
		const struct iphdr *iph = ip_hdr(skb);

		if (ip_route_input(skb, iph->daddr, iph->saddr, iph->tos,
				   skb->dev))
			goto drop;
	}
	return dst_input(skb);
drop:
	kfree_skb(skb);
	return NET_RX_DROP;
}

int xfrm4_rcv_encap(struct sk_buff *skb, int nexthdr, __be32 spi,
		    int encap_type)
{
	XFRM_SPI_SKB_CB(skb)->family = AF_INET;
	XFRM_SPI_SKB_CB(skb)->daddroff = offsetof(struct iphdr, daddr);
	return xfrm_input(skb, nexthdr, spi, encap_type);
}
EXPORT_SYMBOL(xfrm4_rcv_encap);

int xfrm4_transport_finish(struct sk_buff *skb, int async)
{
	struct iphdr *iph = ip_hdr(skb);

	iph->protocol = XFRM_MODE_SKB_CB(skb)->protocol;

#ifndef CONFIG_NETFILTER
	if (!async)
		return -iph->protocol;
#endif

	__skb_push(skb, skb->data - skb_network_header(skb));
	iph->tot_len = htons(skb->len);
	ip_send_check(iph);

	NF_HOOK(PF_INET, NF_INET_PRE_ROUTING, skb, skb->dev, NULL,
		xfrm4_rcv_encap_finish);
	return 0;
}

/* If it's a keepalive packet, then just eat it.
 * If it's an encapsulated packet, then pass it to the
 * IPsec xfrm input.
 * Returns 0 if skb passed to xfrm or was dropped.
 * Returns >0 if skb should be passed to UDP.
 * Returns <0 if skb should be resubmitted (-ret is protocol)
 */
int xfrm4_udp_encap_rcv(struct sock *sk, struct sk_buff *skb)
{
	struct udp_sock *up = udp_sk(sk);
	struct udphdr *uh;
	struct iphdr *iph;
	int iphlen, len;

	__u8 *udpdata;
	__be32 *udpdata32;
	__u16 encap_type = up->encap_type;

	/* if this is not encapsulated socket, then just return now */
	if (!encap_type)
		return 1;

	/* If this is a paged skb, make sure we pull up
	 * whatever data we need to look at. */
	len = skb->len - sizeof(struct udphdr);
	if (!pskb_may_pull(skb, sizeof(struct udphdr) + min(len, 8)))
		return 1;

	/* Now we can get the pointers */
	uh = udp_hdr(skb);
	udpdata = (__u8 *)uh + sizeof(struct udphdr);
	udpdata32 = (__be32 *)udpdata;

	switch (encap_type) {
	default:
	case UDP_ENCAP_ESPINUDP:
		/* Check if this is a keepalive packet.  If so, eat it. */
		if (len == 1 && udpdata[0] == 0xff) {
			goto drop;
		} else if (len > sizeof(struct ip_esp_hdr) && udpdata32[0] != 0) {
			/* ESP Packet without Non-ESP header */
			len = sizeof(struct udphdr);
		} else
			/* Must be an IKE packet.. pass it through */
			return 1;
		break;
	case UDP_ENCAP_ESPINUDP_NON_IKE:
		/* Check if this is a keepalive packet.  If so, eat it. */
		if (len == 1 && udpdata[0] == 0xff) {
			goto drop;
		} else if (len > 2 * sizeof(u32) + sizeof(struct ip_esp_hdr) &&
			   udpdata32[0] == 0 && udpdata32[1] == 0) {

			/* ESP Packet with Non-IKE marker */
			len = sizeof(struct udphdr) + 2 * sizeof(u32);
		} else
			/* Must be an IKE packet.. pass it through */
			return 1;
		break;
	}

	/* At this point we are sure that this is an ESPinUDP packet,
	 * so we need to remove 'len' bytes from the packet (the UDP
	 * header and optional ESP marker bytes) and then modify the
	 * protocol to ESP, and then call into the transform receiver.
	 */
	if (skb_cloned(skb) && pskb_expand_head(skb, 0, 0, GFP_ATOMIC))
		goto drop;

	/* Now we can update and verify the packet length... */
	iph = ip_hdr(skb);
	iphlen = iph->ihl << 2;
	iph->tot_len = htons(ntohs(iph->tot_len) - len);
	if (skb->len < iphlen + len) {
		/* packet is too small!?! */
		goto drop;
	}

	/* pull the data buffer up to the ESP header and set the
	 * transport header to point to ESP.  Keep UDP on the stack
	 * for later.
	 */
	__skb_pull(skb, len);
	skb_reset_transport_header(skb);

	/* process ESP */
	return xfrm4_rcv_encap(skb, IPPROTO_ESP, 0, encap_type);

drop:
	kfree_skb(skb);
	return 0;
}

int xfrm4_rcv(struct sk_buff *skb)
{
	return xfrm4_rcv_spi(skb, ip_hdr(skb)->protocol, 0);
}

EXPORT_SYMBOL(xfrm4_rcv);
