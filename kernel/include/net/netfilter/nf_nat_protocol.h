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

/* Header for use in defining a given protocol. */
#ifndef _NF_NAT_PROTOCOL_H
#define _NF_NAT_PROTOCOL_H
#include <net/netfilter/nf_nat.h>
#include <linux/netfilter/nfnetlink_conntrack.h>

struct nf_nat_range;

struct nf_nat_protocol
{
	/* Protocol number. */
	unsigned int protonum;

	struct module *me;

	/* Translate a packet to the target according to manip type.
	   Return true if succeeded. */
	bool (*manip_pkt)(struct sk_buff *skb,
			  unsigned int iphdroff,
			  const struct nf_conntrack_tuple *tuple,
			  enum nf_nat_manip_type maniptype);

	/* Is the manipable part of the tuple between min and max incl? */
	bool (*in_range)(const struct nf_conntrack_tuple *tuple,
			 enum nf_nat_manip_type maniptype,
			 const union nf_conntrack_man_proto *min,
			 const union nf_conntrack_man_proto *max);

	/* Alter the per-proto part of the tuple (depending on
	   maniptype), to give a unique tuple in the given range if
	   possible; return false if not.  Per-protocol part of tuple
	   is initialized to the incoming packet. */
	bool (*unique_tuple)(struct nf_conntrack_tuple *tuple,
			     const struct nf_nat_range *range,
			     enum nf_nat_manip_type maniptype,
			     const struct nf_conn *ct);

	int (*range_to_nlattr)(struct sk_buff *skb,
			       const struct nf_nat_range *range);

	int (*nlattr_to_range)(struct nlattr *tb[],
			       struct nf_nat_range *range);
};

/* Protocol registration. */
extern int nf_nat_protocol_register(const struct nf_nat_protocol *proto);
extern void nf_nat_protocol_unregister(const struct nf_nat_protocol *proto);

extern const struct nf_nat_protocol *nf_nat_proto_find_get(u_int8_t protocol);
extern void nf_nat_proto_put(const struct nf_nat_protocol *proto);

/* Built-in protocols. */
extern const struct nf_nat_protocol nf_nat_protocol_tcp;
extern const struct nf_nat_protocol nf_nat_protocol_udp;
extern const struct nf_nat_protocol nf_nat_protocol_icmp;
extern const struct nf_nat_protocol nf_nat_unknown_protocol;

extern int init_protocols(void) __init;
extern void cleanup_protocols(void);
extern const struct nf_nat_protocol *find_nat_proto(u_int16_t protonum);

extern bool nf_nat_proto_in_range(const struct nf_conntrack_tuple *tuple,
				  enum nf_nat_manip_type maniptype,
				  const union nf_conntrack_man_proto *min,
				  const union nf_conntrack_man_proto *max);

extern bool nf_nat_proto_unique_tuple(struct nf_conntrack_tuple *tuple,
				      const struct nf_nat_range *range,
				      enum nf_nat_manip_type maniptype,
				      const struct nf_conn *ct,
				      u_int16_t *rover);

extern int nf_nat_proto_range_to_nlattr(struct sk_buff *skb,
					const struct nf_nat_range *range);
extern int nf_nat_proto_nlattr_to_range(struct nlattr *tb[],
					struct nf_nat_range *range);

#endif /*_NF_NAT_PROTO_H*/
