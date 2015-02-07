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

/* (C) 1999-2001 Paul `Rusty' Russell
 * (C) 2002-2006 Netfilter Core Team <coreteam@netfilter.org>
 * (C) 2008 Patrick McHardy <kaber@trash.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/types.h>
#include <linux/random.h>
#include <linux/ip.h>

#include <linux/netfilter.h>
#include <net/netfilter/nf_nat.h>
#include <net/netfilter/nf_nat_core.h>
#include <net/netfilter/nf_nat_rule.h>
#include <net/netfilter/nf_nat_protocol.h>

bool nf_nat_proto_in_range(const struct nf_conntrack_tuple *tuple,
			   enum nf_nat_manip_type maniptype,
			   const union nf_conntrack_man_proto *min,
			   const union nf_conntrack_man_proto *max)
{
	__be16 port;

	if (maniptype == IP_NAT_MANIP_SRC)
		port = tuple->src.u.all;
	else
		port = tuple->dst.u.all;

	return ntohs(port) >= ntohs(min->all) &&
	       ntohs(port) <= ntohs(max->all);
}
EXPORT_SYMBOL_GPL(nf_nat_proto_in_range);

bool nf_nat_proto_unique_tuple(struct nf_conntrack_tuple *tuple,
			       const struct nf_nat_range *range,
			       enum nf_nat_manip_type maniptype,
			       const struct nf_conn *ct,
			       u_int16_t *rover)
{
	unsigned int range_size, min, i;
	__be16 *portptr;
	u_int16_t off;

	if (maniptype == IP_NAT_MANIP_SRC)
		portptr = &tuple->src.u.all;
	else
		portptr = &tuple->dst.u.all;

	/* If no range specified... */
	if (!(range->flags & IP_NAT_RANGE_PROTO_SPECIFIED)) {
		/* If it's dst rewrite, can't change port */
		if (maniptype == IP_NAT_MANIP_DST)
			return false;

		if (ntohs(*portptr) < 1024) {
			/* Loose convention: >> 512 is credential passing */
			if (ntohs(*portptr) < 512) {
				min = 1;
				range_size = 511 - min + 1;
			} else {
				min = 600;
				range_size = 1023 - min + 1;
			}
		} else {
			min = 1024;
			range_size = 65535 - 1024 + 1;
		}
	} else {
		min = ntohs(range->min.all);
		range_size = ntohs(range->max.all) - min + 1;
	}

	if (range->flags & IP_NAT_RANGE_PROTO_RANDOM)
		off = secure_ipv4_port_ephemeral(tuple->src.u3.ip, tuple->dst.u3.ip,
						 maniptype == IP_NAT_MANIP_SRC
						 ? tuple->dst.u.all
						 : tuple->src.u.all);
	else
		off = *rover;

	for (i = 0; i < range_size; i++, off++) {
		*portptr = htons(min + off % range_size);
		if (nf_nat_used_tuple(tuple, ct))
			continue;
		if (!(range->flags & IP_NAT_RANGE_PROTO_RANDOM))
			*rover = off;
		return true;
	}
	return false;
}
EXPORT_SYMBOL_GPL(nf_nat_proto_unique_tuple);

#if defined(CONFIG_NF_CT_NETLINK) || defined(CONFIG_NF_CT_NETLINK_MODULE)
int nf_nat_proto_range_to_nlattr(struct sk_buff *skb,
				 const struct nf_nat_range *range)
{
	NLA_PUT_BE16(skb, CTA_PROTONAT_PORT_MIN, range->min.all);
	NLA_PUT_BE16(skb, CTA_PROTONAT_PORT_MAX, range->max.all);
	return 0;

nla_put_failure:
	return -1;
}
EXPORT_SYMBOL_GPL(nf_nat_proto_nlattr_to_range);

int nf_nat_proto_nlattr_to_range(struct nlattr *tb[],
				 struct nf_nat_range *range)
{
	if (tb[CTA_PROTONAT_PORT_MIN]) {
		range->min.all = nla_get_be16(tb[CTA_PROTONAT_PORT_MIN]);
		range->max.all = range->min.tcp.port;
		range->flags |= IP_NAT_RANGE_PROTO_SPECIFIED;
	}
	if (tb[CTA_PROTONAT_PORT_MAX]) {
		range->max.all = nla_get_be16(tb[CTA_PROTONAT_PORT_MAX]);
		range->flags |= IP_NAT_RANGE_PROTO_SPECIFIED;
	}
	return 0;
}
EXPORT_SYMBOL_GPL(nf_nat_proto_range_to_nlattr);
#endif
