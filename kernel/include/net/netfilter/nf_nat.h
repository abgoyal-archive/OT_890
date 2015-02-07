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

#ifndef _NF_NAT_H
#define _NF_NAT_H
#include <linux/netfilter_ipv4.h>
#include <net/netfilter/nf_conntrack_tuple.h>

#define NF_NAT_MAPPING_TYPE_MAX_NAMELEN 16

enum nf_nat_manip_type
{
	IP_NAT_MANIP_SRC,
	IP_NAT_MANIP_DST
};

/* SRC manip occurs POST_ROUTING or LOCAL_IN */
#define HOOK2MANIP(hooknum) ((hooknum) != NF_INET_POST_ROUTING && \
			     (hooknum) != NF_INET_LOCAL_IN)

#define IP_NAT_RANGE_MAP_IPS 1
#define IP_NAT_RANGE_PROTO_SPECIFIED 2
#define IP_NAT_RANGE_PROTO_RANDOM 4

/* NAT sequence number modifications */
struct nf_nat_seq {
	/* position of the last TCP sequence number modification (if any) */
	u_int32_t correction_pos;

	/* sequence number offset before and after last modification */
	int16_t offset_before, offset_after;
};

/* Single range specification. */
struct nf_nat_range
{
	/* Set to OR of flags above. */
	unsigned int flags;

	/* Inclusive: network order. */
	__be32 min_ip, max_ip;

	/* Inclusive: network order */
	union nf_conntrack_man_proto min, max;
};

/* For backwards compat: don't use in modern code. */
struct nf_nat_multi_range_compat
{
	unsigned int rangesize; /* Must be 1. */

	/* hangs off end. */
	struct nf_nat_range range[1];
};

#ifdef __KERNEL__
#include <linux/list.h>
#include <linux/netfilter/nf_conntrack_pptp.h>
#include <net/netfilter/nf_conntrack_extend.h>

/* per conntrack: nat application helper private data */
union nf_conntrack_nat_help
{
	/* insert nat helper private data here */
	struct nf_nat_pptp nat_pptp_info;
};

struct nf_conn;

/* The structure embedded in the conntrack structure. */
struct nf_conn_nat
{
	struct hlist_node bysource;
	struct nf_nat_seq seq[IP_CT_DIR_MAX];
	struct nf_conn *ct;
	union nf_conntrack_nat_help help;
#if defined(CONFIG_IP_NF_TARGET_MASQUERADE) || \
    defined(CONFIG_IP_NF_TARGET_MASQUERADE_MODULE)
	int masq_index;
#endif
};

/* Set up the info structure to map into this range. */
extern unsigned int nf_nat_setup_info(struct nf_conn *ct,
				      const struct nf_nat_range *range,
				      enum nf_nat_manip_type maniptype);

/* Is this tuple already taken? (not by us)*/
extern int nf_nat_used_tuple(const struct nf_conntrack_tuple *tuple,
			     const struct nf_conn *ignored_conntrack);

static inline struct nf_conn_nat *nfct_nat(const struct nf_conn *ct)
{
	return nf_ct_ext_find(ct, NF_CT_EXT_NAT);
}

#else  /* !__KERNEL__: iptables wants this to compile. */
#define nf_nat_multi_range nf_nat_multi_range_compat
#endif /*__KERNEL__*/
#endif
