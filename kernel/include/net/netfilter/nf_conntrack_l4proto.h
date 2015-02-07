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
 * Header for use in defining a given L4 protocol for connection tracking.
 *
 * 16 Dec 2003: Yasuyuki Kozakai @USAGI <yasuyuki.kozakai@toshiba.co.jp>
 *	- generalized L3 protocol dependent part.
 *
 * Derived from include/linux/netfiter_ipv4/ip_conntrack_protcol.h
 */

#ifndef _NF_CONNTRACK_L4PROTO_H
#define _NF_CONNTRACK_L4PROTO_H
#include <linux/netlink.h>
#include <net/netlink.h>
#include <net/netfilter/nf_conntrack.h>

struct seq_file;

struct nf_conntrack_l4proto
{
	/* L3 Protocol number. */
	u_int16_t l3proto;

	/* L4 Protocol number. */
	u_int8_t l4proto;

	/* Try to fill in the third arg: dataoff is offset past network protocol
           hdr.  Return true if possible. */
	bool (*pkt_to_tuple)(const struct sk_buff *skb, unsigned int dataoff,
			     struct nf_conntrack_tuple *tuple);

	/* Invert the per-proto part of the tuple: ie. turn xmit into reply.
	 * Some packets can't be inverted: return 0 in that case.
	 */
	bool (*invert_tuple)(struct nf_conntrack_tuple *inverse,
			     const struct nf_conntrack_tuple *orig);

	/* Returns verdict for packet, or -1 for invalid. */
	int (*packet)(struct nf_conn *ct,
		      const struct sk_buff *skb,
		      unsigned int dataoff,
		      enum ip_conntrack_info ctinfo,
		      u_int8_t pf,
		      unsigned int hooknum);

	/* Called when a new connection for this protocol found;
	 * returns TRUE if it's OK.  If so, packet() called next. */
	bool (*new)(struct nf_conn *ct, const struct sk_buff *skb,
		    unsigned int dataoff);

	/* Called when a conntrack entry is destroyed */
	void (*destroy)(struct nf_conn *ct);

	int (*error)(struct net *net, struct sk_buff *skb, unsigned int dataoff,
		     enum ip_conntrack_info *ctinfo,
		     u_int8_t pf, unsigned int hooknum);

	/* Print out the per-protocol part of the tuple. Return like seq_* */
	int (*print_tuple)(struct seq_file *s,
			   const struct nf_conntrack_tuple *);

	/* Print out the private part of the conntrack. */
	int (*print_conntrack)(struct seq_file *s, const struct nf_conn *);

	/* convert protoinfo to nfnetink attributes */
	int (*to_nlattr)(struct sk_buff *skb, struct nlattr *nla,
			 const struct nf_conn *ct);

	/* convert nfnetlink attributes to protoinfo */
	int (*from_nlattr)(struct nlattr *tb[], struct nf_conn *ct);

	int (*tuple_to_nlattr)(struct sk_buff *skb,
			       const struct nf_conntrack_tuple *t);
	int (*nlattr_to_tuple)(struct nlattr *tb[],
			       struct nf_conntrack_tuple *t);
	const struct nla_policy *nla_policy;

#ifdef CONFIG_SYSCTL
	struct ctl_table_header	**ctl_table_header;
	struct ctl_table	*ctl_table;
	unsigned int		*ctl_table_users;
#ifdef CONFIG_NF_CONNTRACK_PROC_COMPAT
	struct ctl_table_header	*ctl_compat_table_header;
	struct ctl_table	*ctl_compat_table;
#endif
#endif
	/* Protocol name */
	const char *name;

	/* Module (if any) which this is connected to. */
	struct module *me;
};

/* Existing built-in protocols */
extern struct nf_conntrack_l4proto nf_conntrack_l4proto_tcp6;
extern struct nf_conntrack_l4proto nf_conntrack_l4proto_udp4;
extern struct nf_conntrack_l4proto nf_conntrack_l4proto_udp6;
extern struct nf_conntrack_l4proto nf_conntrack_l4proto_generic;

#define MAX_NF_CT_PROTO 256

extern struct nf_conntrack_l4proto *
__nf_ct_l4proto_find(u_int16_t l3proto, u_int8_t l4proto);

extern struct nf_conntrack_l4proto *
nf_ct_l4proto_find_get(u_int16_t l3proto, u_int8_t protocol);

extern void nf_ct_l4proto_put(struct nf_conntrack_l4proto *p);

/* Protocol registration. */
extern int nf_conntrack_l4proto_register(struct nf_conntrack_l4proto *proto);
extern void nf_conntrack_l4proto_unregister(struct nf_conntrack_l4proto *proto);

/* Generic netlink helpers */
extern int nf_ct_port_tuple_to_nlattr(struct sk_buff *skb,
				      const struct nf_conntrack_tuple *tuple);
extern int nf_ct_port_nlattr_to_tuple(struct nlattr *tb[],
				      struct nf_conntrack_tuple *t);
extern const struct nla_policy nf_ct_port_nla_policy[];

#ifdef CONFIG_SYSCTL
#ifdef DEBUG_INVALID_PACKETS
#define LOG_INVALID(net, proto)				\
	((net)->ct.sysctl_log_invalid == (proto) ||	\
	 (net)->ct.sysctl_log_invalid == IPPROTO_RAW)
#else
#define LOG_INVALID(net, proto)				\
	(((net)->ct.sysctl_log_invalid == (proto) ||	\
	  (net)->ct.sysctl_log_invalid == IPPROTO_RAW)	\
	 && net_ratelimit())
#endif
#else
static inline int LOG_INVALID(struct net *net, int proto) { return 0; }
#endif /* CONFIG_SYSCTL */

#endif /*_NF_CONNTRACK_PROTOCOL_H*/
