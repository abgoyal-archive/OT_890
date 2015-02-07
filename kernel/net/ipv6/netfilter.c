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

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/ipv6.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv6.h>
#include <net/dst.h>
#include <net/ipv6.h>
#include <net/ip6_route.h>
#include <net/xfrm.h>
#include <net/ip6_checksum.h>
#include <net/netfilter/nf_queue.h>

int ip6_route_me_harder(struct sk_buff *skb)
{
	struct net *net = dev_net(skb->dst->dev);
	struct ipv6hdr *iph = ipv6_hdr(skb);
	struct dst_entry *dst;
	struct flowi fl = {
		.oif = skb->sk ? skb->sk->sk_bound_dev_if : 0,
		.mark = skb->mark,
		.nl_u =
		{ .ip6_u =
		  { .daddr = iph->daddr,
		    .saddr = iph->saddr, } },
	};

	dst = ip6_route_output(net, skb->sk, &fl);

#ifdef CONFIG_XFRM
	if (!(IP6CB(skb)->flags & IP6SKB_XFRM_TRANSFORMED) &&
	    xfrm_decode_session(skb, &fl, AF_INET6) == 0)
		if (xfrm_lookup(net, &skb->dst, &fl, skb->sk, 0))
			return -1;
#endif

	if (dst->error) {
		IP6_INC_STATS(net, ip6_dst_idev(dst), IPSTATS_MIB_OUTNOROUTES);
		LIMIT_NETDEBUG(KERN_DEBUG "ip6_route_me_harder: No more route.\n");
		dst_release(dst);
		return -EINVAL;
	}

	/* Drop old route. */
	dst_release(skb->dst);

	skb->dst = dst;
	return 0;
}
EXPORT_SYMBOL(ip6_route_me_harder);

/*
 * Extra routing may needed on local out, as the QUEUE target never
 * returns control to the table.
 */

struct ip6_rt_info {
	struct in6_addr daddr;
	struct in6_addr saddr;
	u_int32_t mark;
};

static void nf_ip6_saveroute(const struct sk_buff *skb,
			     struct nf_queue_entry *entry)
{
	struct ip6_rt_info *rt_info = nf_queue_entry_reroute(entry);

	if (entry->hook == NF_INET_LOCAL_OUT) {
		struct ipv6hdr *iph = ipv6_hdr(skb);

		rt_info->daddr = iph->daddr;
		rt_info->saddr = iph->saddr;
		rt_info->mark = skb->mark;
	}
}

static int nf_ip6_reroute(struct sk_buff *skb,
			  const struct nf_queue_entry *entry)
{
	struct ip6_rt_info *rt_info = nf_queue_entry_reroute(entry);

	if (entry->hook == NF_INET_LOCAL_OUT) {
		struct ipv6hdr *iph = ipv6_hdr(skb);
		if (!ipv6_addr_equal(&iph->daddr, &rt_info->daddr) ||
		    !ipv6_addr_equal(&iph->saddr, &rt_info->saddr) ||
		    skb->mark != rt_info->mark)
			return ip6_route_me_harder(skb);
	}
	return 0;
}

static int nf_ip6_route(struct dst_entry **dst, struct flowi *fl)
{
	*dst = ip6_route_output(&init_net, NULL, fl);
	return (*dst)->error;
}

__sum16 nf_ip6_checksum(struct sk_buff *skb, unsigned int hook,
			     unsigned int dataoff, u_int8_t protocol)
{
	struct ipv6hdr *ip6h = ipv6_hdr(skb);
	__sum16 csum = 0;

	switch (skb->ip_summed) {
	case CHECKSUM_COMPLETE:
		if (hook != NF_INET_PRE_ROUTING && hook != NF_INET_LOCAL_IN)
			break;
		if (!csum_ipv6_magic(&ip6h->saddr, &ip6h->daddr,
				     skb->len - dataoff, protocol,
				     csum_sub(skb->csum,
					      skb_checksum(skb, 0,
							   dataoff, 0)))) {
			skb->ip_summed = CHECKSUM_UNNECESSARY;
			break;
		}
		/* fall through */
	case CHECKSUM_NONE:
		skb->csum = ~csum_unfold(
				csum_ipv6_magic(&ip6h->saddr, &ip6h->daddr,
					     skb->len - dataoff,
					     protocol,
					     csum_sub(0,
						      skb_checksum(skb, 0,
								   dataoff, 0))));
		csum = __skb_checksum_complete(skb);
	}
	return csum;
}
EXPORT_SYMBOL(nf_ip6_checksum);

static __sum16 nf_ip6_checksum_partial(struct sk_buff *skb, unsigned int hook,
				       unsigned int dataoff, unsigned int len,
				       u_int8_t protocol)
{
	struct ipv6hdr *ip6h = ipv6_hdr(skb);
	__wsum hsum;
	__sum16 csum = 0;

	switch (skb->ip_summed) {
	case CHECKSUM_COMPLETE:
		if (len == skb->len - dataoff)
			return nf_ip6_checksum(skb, hook, dataoff, protocol);
		/* fall through */
	case CHECKSUM_NONE:
		hsum = skb_checksum(skb, 0, dataoff, 0);
		skb->csum = ~csum_unfold(csum_ipv6_magic(&ip6h->saddr,
							 &ip6h->daddr,
							 skb->len - dataoff,
							 protocol,
							 csum_sub(0, hsum)));
		skb->ip_summed = CHECKSUM_NONE;
		csum = __skb_checksum_complete_head(skb, dataoff + len);
		if (!csum)
			skb->ip_summed = CHECKSUM_UNNECESSARY;
	}
	return csum;
};

static const struct nf_afinfo nf_ip6_afinfo = {
	.family			= AF_INET6,
	.checksum		= nf_ip6_checksum,
	.checksum_partial	= nf_ip6_checksum_partial,
	.route			= nf_ip6_route,
	.saveroute		= nf_ip6_saveroute,
	.reroute		= nf_ip6_reroute,
	.route_key_size		= sizeof(struct ip6_rt_info),
};

int __init ipv6_netfilter_init(void)
{
	return nf_register_afinfo(&nf_ip6_afinfo);
}

/* This can be called from inet6_init() on errors, so it cannot
 * be marked __exit. -DaveM
 */
void ipv6_netfilter_fini(void)
{
	nf_unregister_afinfo(&nf_ip6_afinfo);
}
