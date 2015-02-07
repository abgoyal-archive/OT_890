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

#ifndef __NETNS_XFRM_H
#define __NETNS_XFRM_H

#include <linux/list.h>
#include <linux/wait.h>
#include <linux/workqueue.h>
#include <linux/xfrm.h>

struct ctl_table_header;

struct xfrm_policy_hash {
	struct hlist_head	*table;
	unsigned int		hmask;
};

struct netns_xfrm {
	struct list_head	state_all;
	/*
	 * Hash table to find appropriate SA towards given target (endpoint of
	 * tunnel or destination of transport mode) allowed by selector.
	 *
	 * Main use is finding SA after policy selected tunnel or transport
	 * mode. Also, it can be used by ah/esp icmp error handler to find
	 * offending SA.
	 */
	struct hlist_head	*state_bydst;
	struct hlist_head	*state_bysrc;
	struct hlist_head	*state_byspi;
	unsigned int		state_hmask;
	unsigned int		state_num;
	struct work_struct	state_hash_work;
	struct hlist_head	state_gc_list;
	struct work_struct	state_gc_work;

	wait_queue_head_t	km_waitq;

	struct list_head	policy_all;
	struct hlist_head	*policy_byidx;
	unsigned int		policy_idx_hmask;
	struct hlist_head	policy_inexact[XFRM_POLICY_MAX * 2];
	struct xfrm_policy_hash	policy_bydst[XFRM_POLICY_MAX * 2];
	unsigned int		policy_count[XFRM_POLICY_MAX * 2];
	struct work_struct	policy_hash_work;

	struct sock		*nlsk;

	u32			sysctl_aevent_etime;
	u32			sysctl_aevent_rseqth;
	int			sysctl_larval_drop;
	u32			sysctl_acq_expires;
#ifdef CONFIG_SYSCTL
	struct ctl_table_header	*sysctl_hdr;
#endif
};

#endif
