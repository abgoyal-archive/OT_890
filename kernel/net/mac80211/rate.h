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
 * Copyright 2002-2005, Instant802 Networks, Inc.
 * Copyright 2005, Devicescape Software, Inc.
 * Copyright (c) 2006 Jiri Benc <jbenc@suse.cz>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef IEEE80211_RATE_H
#define IEEE80211_RATE_H

#include <linux/netdevice.h>
#include <linux/skbuff.h>
#include <linux/types.h>
#include <linux/kref.h>
#include <net/mac80211.h>
#include "ieee80211_i.h"
#include "sta_info.h"

struct rate_control_ref {
	struct ieee80211_local *local;
	struct rate_control_ops *ops;
	void *priv;
	struct kref kref;
};

/* Get a reference to the rate control algorithm. If `name' is NULL, get the
 * first available algorithm. */
struct rate_control_ref *rate_control_alloc(const char *name,
					    struct ieee80211_local *local);
void rate_control_get_rate(struct ieee80211_sub_if_data *sdata,
			   struct sta_info *sta,
			   struct ieee80211_tx_rate_control *txrc);
struct rate_control_ref *rate_control_get(struct rate_control_ref *ref);
void rate_control_put(struct rate_control_ref *ref);

static inline void rate_control_tx_status(struct ieee80211_local *local,
					  struct ieee80211_supported_band *sband,
					  struct sta_info *sta,
					  struct sk_buff *skb)
{
	struct rate_control_ref *ref = local->rate_ctrl;
	struct ieee80211_sta *ista = &sta->sta;
	void *priv_sta = sta->rate_ctrl_priv;

	ref->ops->tx_status(ref->priv, sband, ista, priv_sta, skb);
}


static inline void rate_control_rate_init(struct sta_info *sta)
{
	struct ieee80211_local *local = sta->sdata->local;
	struct rate_control_ref *ref = sta->rate_ctrl;
	struct ieee80211_sta *ista = &sta->sta;
	void *priv_sta = sta->rate_ctrl_priv;
	struct ieee80211_supported_band *sband;

	sband = local->hw.wiphy->bands[local->hw.conf.channel->band];

	ref->ops->rate_init(ref->priv, sband, ista, priv_sta);
}


static inline void *rate_control_alloc_sta(struct rate_control_ref *ref,
					   struct ieee80211_sta *sta,
					   gfp_t gfp)
{
	return ref->ops->alloc_sta(ref->priv, sta, gfp);
}

static inline void rate_control_free_sta(struct sta_info *sta)
{
	struct rate_control_ref *ref = sta->rate_ctrl;
	struct ieee80211_sta *ista = &sta->sta;
	void *priv_sta = sta->rate_ctrl_priv;

	ref->ops->free_sta(ref->priv, ista, priv_sta);
}

static inline void rate_control_add_sta_debugfs(struct sta_info *sta)
{
#ifdef CONFIG_MAC80211_DEBUGFS
	struct rate_control_ref *ref = sta->rate_ctrl;
	if (sta->debugfs.dir && ref->ops->add_sta_debugfs)
		ref->ops->add_sta_debugfs(ref->priv, sta->rate_ctrl_priv,
					  sta->debugfs.dir);
#endif
}

static inline void rate_control_remove_sta_debugfs(struct sta_info *sta)
{
#ifdef CONFIG_MAC80211_DEBUGFS
	struct rate_control_ref *ref = sta->rate_ctrl;
	if (ref->ops->remove_sta_debugfs)
		ref->ops->remove_sta_debugfs(ref->priv, sta->rate_ctrl_priv);
#endif
}

/* functions for rate control related to a device */
int ieee80211_init_rate_ctrl_alg(struct ieee80211_local *local,
				 const char *name);
void rate_control_deinitialize(struct ieee80211_local *local);


/* Rate control algorithms */
#ifdef CONFIG_MAC80211_RC_PID
extern int rc80211_pid_init(void);
extern void rc80211_pid_exit(void);
#else
static inline int rc80211_pid_init(void)
{
	return 0;
}
static inline void rc80211_pid_exit(void)
{
}
#endif

#ifdef CONFIG_MAC80211_RC_MINSTREL
extern int rc80211_minstrel_init(void);
extern void rc80211_minstrel_exit(void);
#else
static inline int rc80211_minstrel_init(void)
{
	return 0;
}
static inline void rc80211_minstrel_exit(void)
{
}
#endif


#endif /* IEEE80211_RATE_H */
