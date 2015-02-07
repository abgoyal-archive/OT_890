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
 * ipv6 in net namespaces
 */

#include <net/inet_frag.h>

#ifndef __NETNS_IPV6_H__
#define __NETNS_IPV6_H__

struct ctl_table_header;

struct netns_sysctl_ipv6 {
#ifdef CONFIG_SYSCTL
	struct ctl_table_header *table;
	struct ctl_table_header *frags_hdr;
#endif
	int bindv6only;
	int flush_delay;
	int ip6_rt_max_size;
	int ip6_rt_gc_min_interval;
	int ip6_rt_gc_timeout;
	int ip6_rt_gc_interval;
	int ip6_rt_gc_elasticity;
	int ip6_rt_mtu_expires;
	int ip6_rt_min_advmss;
	int icmpv6_time;
};

struct netns_ipv6 {
	struct netns_sysctl_ipv6 sysctl;
	struct ipv6_devconf	*devconf_all;
	struct ipv6_devconf	*devconf_dflt;
	struct netns_frags	frags;
#ifdef CONFIG_NETFILTER
	struct xt_table		*ip6table_filter;
	struct xt_table		*ip6table_mangle;
	struct xt_table		*ip6table_raw;
	struct xt_table		*ip6table_security;
#endif
	struct rt6_info         *ip6_null_entry;
	struct rt6_statistics   *rt6_stats;
	struct timer_list       ip6_fib_timer;
	struct hlist_head       *fib_table_hash;
	struct fib6_table       *fib6_main_tbl;
	struct dst_ops		*ip6_dst_ops;
	unsigned int		 ip6_rt_gc_expire;
	unsigned long		 ip6_rt_last_gc;
#ifdef CONFIG_IPV6_MULTIPLE_TABLES
	struct rt6_info         *ip6_prohibit_entry;
	struct rt6_info         *ip6_blk_hole_entry;
	struct fib6_table       *fib6_local_tbl;
	struct fib_rules_ops    *fib6_rules_ops;
#endif
	struct sock		**icmp_sk;
	struct sock             *ndisc_sk;
	struct sock             *tcp_sk;
	struct sock             *igmp_sk;
#ifdef CONFIG_IPV6_MROUTE
	struct sock		*mroute6_sk;
	struct mfc6_cache	**mfc6_cache_array;
	struct mif_device	*vif6_table;
	int			maxvif;
	atomic_t		cache_resolve_queue_len;
	int			mroute_do_assert;
	int			mroute_do_pim;
#ifdef CONFIG_IPV6_PIMSM_V2
	int			mroute_reg_vif_num;
#endif
#endif
};
#endif
