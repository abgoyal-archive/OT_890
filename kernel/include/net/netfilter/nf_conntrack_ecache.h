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
 * connection tracking event cache.
 */

#ifndef _NF_CONNTRACK_ECACHE_H
#define _NF_CONNTRACK_ECACHE_H
#include <net/netfilter/nf_conntrack.h>

#include <linux/notifier.h>
#include <linux/interrupt.h>
#include <net/net_namespace.h>
#include <net/netfilter/nf_conntrack_expect.h>

#ifdef CONFIG_NF_CONNTRACK_EVENTS
struct nf_conntrack_ecache {
	struct nf_conn *ct;
	unsigned int events;
};

/* This structure is passed to event handler */
struct nf_ct_event {
	struct nf_conn *ct;
	u32 pid;
	int report;
};

extern struct atomic_notifier_head nf_conntrack_chain;
extern int nf_conntrack_register_notifier(struct notifier_block *nb);
extern int nf_conntrack_unregister_notifier(struct notifier_block *nb);

extern void nf_ct_deliver_cached_events(const struct nf_conn *ct);
extern void __nf_ct_event_cache_init(struct nf_conn *ct);
extern void nf_ct_event_cache_flush(struct net *net);

static inline void
nf_conntrack_event_cache(enum ip_conntrack_events event, struct nf_conn *ct)
{
	struct net *net = nf_ct_net(ct);
	struct nf_conntrack_ecache *ecache;

	local_bh_disable();
	ecache = per_cpu_ptr(net->ct.ecache, raw_smp_processor_id());
	if (ct != ecache->ct)
		__nf_ct_event_cache_init(ct);
	ecache->events |= event;
	local_bh_enable();
}

static inline void
nf_conntrack_event_report(enum ip_conntrack_events event,
			  struct nf_conn *ct,
			  u32 pid,
			  int report)
{
	struct nf_ct_event item = {
		.ct 	= ct,
		.pid	= pid,
		.report = report
	};
	if (nf_ct_is_confirmed(ct) && !nf_ct_is_dying(ct))
		atomic_notifier_call_chain(&nf_conntrack_chain, event, &item);
}

static inline void
nf_conntrack_event(enum ip_conntrack_events event, struct nf_conn *ct)
{
	nf_conntrack_event_report(event, ct, 0, 0);
}

struct nf_exp_event {
	struct nf_conntrack_expect *exp;
	u32 pid;
	int report;
};

extern struct atomic_notifier_head nf_ct_expect_chain;
extern int nf_ct_expect_register_notifier(struct notifier_block *nb);
extern int nf_ct_expect_unregister_notifier(struct notifier_block *nb);

static inline void
nf_ct_expect_event_report(enum ip_conntrack_expect_events event,
			  struct nf_conntrack_expect *exp,
			  u32 pid,
			  int report)
{
	struct nf_exp_event item = {
		.exp	= exp,
		.pid	= pid,
		.report = report
	};
	atomic_notifier_call_chain(&nf_ct_expect_chain, event, &item);
}

static inline void
nf_ct_expect_event(enum ip_conntrack_expect_events event,
		   struct nf_conntrack_expect *exp)
{
	nf_ct_expect_event_report(event, exp, 0, 0);
}

extern int nf_conntrack_ecache_init(struct net *net);
extern void nf_conntrack_ecache_fini(struct net *net);

#else /* CONFIG_NF_CONNTRACK_EVENTS */

static inline void nf_conntrack_event_cache(enum ip_conntrack_events event,
					    struct nf_conn *ct) {}
static inline void nf_conntrack_event(enum ip_conntrack_events event,
				      struct nf_conn *ct) {}
static inline void nf_conntrack_event_report(enum ip_conntrack_events event,
					     struct nf_conn *ct,
					     u32 pid,
					     int report) {}
static inline void nf_ct_deliver_cached_events(const struct nf_conn *ct) {}
static inline void nf_ct_expect_event(enum ip_conntrack_expect_events event,
				      struct nf_conntrack_expect *exp) {}
static inline void nf_ct_expect_event_report(enum ip_conntrack_expect_events e,
					     struct nf_conntrack_expect *exp,
 					     u32 pid,
 					     int report) {}
static inline void nf_ct_event_cache_flush(struct net *net) {}

static inline int nf_conntrack_ecache_init(struct net *net)
{
	return 0;
}

static inline void nf_conntrack_ecache_fini(struct net *net)
{
}
#endif /* CONFIG_NF_CONNTRACK_EVENTS */

#endif /*_NF_CONNTRACK_ECACHE_H*/

