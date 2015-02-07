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

#ifndef __NET_GEN_STATS_H
#define __NET_GEN_STATS_H

#include <linux/gen_stats.h>
#include <linux/socket.h>
#include <linux/rtnetlink.h>
#include <linux/pkt_sched.h>

struct gnet_dump
{
	spinlock_t *      lock;
	struct sk_buff *  skb;
	struct nlattr *   tail;

	/* Backward compatability */
	int               compat_tc_stats;
	int               compat_xstats;
	void *            xstats;
	int               xstats_len;
	struct tc_stats   tc_stats;
};

extern int gnet_stats_start_copy(struct sk_buff *skb, int type,
				 spinlock_t *lock, struct gnet_dump *d);

extern int gnet_stats_start_copy_compat(struct sk_buff *skb, int type,
					int tc_stats_type,int xstats_type,
					spinlock_t *lock, struct gnet_dump *d);

extern int gnet_stats_copy_basic(struct gnet_dump *d,
				 struct gnet_stats_basic *b);
extern int gnet_stats_copy_rate_est(struct gnet_dump *d,
				    struct gnet_stats_rate_est *r);
extern int gnet_stats_copy_queue(struct gnet_dump *d,
				 struct gnet_stats_queue *q);
extern int gnet_stats_copy_app(struct gnet_dump *d, void *st, int len);

extern int gnet_stats_finish_copy(struct gnet_dump *d);

extern int gen_new_estimator(struct gnet_stats_basic *bstats,
			     struct gnet_stats_rate_est *rate_est,
			     spinlock_t *stats_lock, struct nlattr *opt);
extern void gen_kill_estimator(struct gnet_stats_basic *bstats,
			       struct gnet_stats_rate_est *rate_est);
extern int gen_replace_estimator(struct gnet_stats_basic *bstats,
				 struct gnet_stats_rate_est *rate_est,
				 spinlock_t *stats_lock, struct nlattr *opt);
extern bool gen_estimator_active(const struct gnet_stats_basic *bstats,
				 const struct gnet_stats_rate_est *rate_est);
#endif
