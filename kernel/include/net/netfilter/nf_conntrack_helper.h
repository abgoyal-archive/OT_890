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
 * connection tracking helpers.
 *
 * 16 Dec 2003: Yasuyuki Kozakai @USAGI <yasuyuki.kozakai@toshiba.co.jp>
 *	- generalize L3 protocol dependent part.
 *
 * Derived from include/linux/netfiter_ipv4/ip_conntrack_helper.h
 */

#ifndef _NF_CONNTRACK_HELPER_H
#define _NF_CONNTRACK_HELPER_H
#include <net/netfilter/nf_conntrack.h>
#include <net/netfilter/nf_conntrack_extend.h>

struct module;

struct nf_conntrack_helper
{
	struct hlist_node hnode;	/* Internal use. */

	const char *name;		/* name of the module */
	struct module *me;		/* pointer to self */
	const struct nf_conntrack_expect_policy *expect_policy;

	/* Tuple of things we will help (compared against server response) */
	struct nf_conntrack_tuple tuple;

	/* Function to call when data passes; return verdict, or -1 to
           invalidate. */
	int (*help)(struct sk_buff *skb,
		    unsigned int protoff,
		    struct nf_conn *ct,
		    enum ip_conntrack_info conntrackinfo);

	void (*destroy)(struct nf_conn *ct);

	int (*to_nlattr)(struct sk_buff *skb, const struct nf_conn *ct);
	unsigned int expect_class_max;
};

extern struct nf_conntrack_helper *
__nf_conntrack_helper_find_byname(const char *name);

extern int nf_conntrack_helper_register(struct nf_conntrack_helper *);
extern void nf_conntrack_helper_unregister(struct nf_conntrack_helper *);

extern struct nf_conn_help *nf_ct_helper_ext_add(struct nf_conn *ct, gfp_t gfp);

extern int __nf_ct_try_assign_helper(struct nf_conn *ct, gfp_t flags);

static inline struct nf_conn_help *nfct_help(const struct nf_conn *ct)
{
	return nf_ct_ext_find(ct, NF_CT_EXT_HELPER);
}

extern int nf_conntrack_helper_init(void);
extern void nf_conntrack_helper_fini(void);

#endif /*_NF_CONNTRACK_HELPER_H*/
