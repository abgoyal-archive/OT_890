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

#ifndef __NET_FIB_RULES_H
#define __NET_FIB_RULES_H

#include <linux/types.h>
#include <linux/netdevice.h>
#include <linux/fib_rules.h>
#include <net/flow.h>
#include <net/rtnetlink.h>

struct fib_rule
{
	struct list_head	list;
	atomic_t		refcnt;
	int			ifindex;
	char			ifname[IFNAMSIZ];
	u32			mark;
	u32			mark_mask;
	u32			pref;
	u32			flags;
	u32			table;
	u8			action;
	u32			target;
	struct fib_rule *	ctarget;
	struct rcu_head		rcu;
	struct net *		fr_net;
};

struct fib_lookup_arg
{
	void			*lookup_ptr;
	void			*result;
	struct fib_rule		*rule;
};

struct fib_rules_ops
{
	int			family;
	struct list_head	list;
	int			rule_size;
	int			addr_size;
	int			unresolved_rules;
	int			nr_goto_rules;

	int			(*action)(struct fib_rule *,
					  struct flowi *, int,
					  struct fib_lookup_arg *);
	int			(*match)(struct fib_rule *,
					 struct flowi *, int);
	int			(*configure)(struct fib_rule *,
					     struct sk_buff *,
					     struct nlmsghdr *,
					     struct fib_rule_hdr *,
					     struct nlattr **);
	int			(*compare)(struct fib_rule *,
					   struct fib_rule_hdr *,
					   struct nlattr **);
	int			(*fill)(struct fib_rule *, struct sk_buff *,
					struct nlmsghdr *,
					struct fib_rule_hdr *);
	u32			(*default_pref)(struct fib_rules_ops *ops);
	size_t			(*nlmsg_payload)(struct fib_rule *);

	/* Called after modifications to the rules set, must flush
	 * the route cache if one exists. */
	void			(*flush_cache)(struct fib_rules_ops *ops);

	int			nlgroup;
	const struct nla_policy	*policy;
	struct list_head	rules_list;
	struct module		*owner;
	struct net		*fro_net;
};

#define FRA_GENERIC_POLICY \
	[FRA_IFNAME]	= { .type = NLA_STRING, .len = IFNAMSIZ - 1 }, \
	[FRA_PRIORITY]	= { .type = NLA_U32 }, \
	[FRA_FWMARK]	= { .type = NLA_U32 }, \
	[FRA_FWMASK]	= { .type = NLA_U32 }, \
	[FRA_TABLE]     = { .type = NLA_U32 }, \
	[FRA_GOTO]	= { .type = NLA_U32 }

static inline void fib_rule_get(struct fib_rule *rule)
{
	atomic_inc(&rule->refcnt);
}

static inline void fib_rule_put_rcu(struct rcu_head *head)
{
	struct fib_rule *rule = container_of(head, struct fib_rule, rcu);
	release_net(rule->fr_net);
	kfree(rule);
}

static inline void fib_rule_put(struct fib_rule *rule)
{
	if (atomic_dec_and_test(&rule->refcnt))
		call_rcu(&rule->rcu, fib_rule_put_rcu);
}

static inline u32 frh_get_table(struct fib_rule_hdr *frh, struct nlattr **nla)
{
	if (nla[FRA_TABLE])
		return nla_get_u32(nla[FRA_TABLE]);
	return frh->table;
}

extern int fib_rules_register(struct fib_rules_ops *);
extern void fib_rules_unregister(struct fib_rules_ops *);
extern void                     fib_rules_cleanup_ops(struct fib_rules_ops *);

extern int			fib_rules_lookup(struct fib_rules_ops *,
						 struct flowi *, int flags,
						 struct fib_lookup_arg *);
extern int			fib_default_rule_add(struct fib_rules_ops *,
						     u32 pref, u32 table,
						     u32 flags);
#endif
