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
 * Wireless configuration interface internals.
 *
 * Copyright 2006, 2007 Johannes Berg <johannes@sipsolutions.net>
 */
#ifndef __NET_WIRELESS_CORE_H
#define __NET_WIRELESS_CORE_H
#include <linux/mutex.h>
#include <linux/list.h>
#include <linux/netdevice.h>
#include <net/genetlink.h>
#include <net/wireless.h>
#include <net/cfg80211.h>
#include "reg.h"

struct cfg80211_registered_device {
	struct cfg80211_ops *ops;
	struct list_head list;
	/* we hold this mutex during any call so that
	 * we cannot do multiple calls at once, and also
	 * to avoid the deregister call to proceed while
	 * any call is in progress */
	struct mutex mtx;

	/* ISO / IEC 3166 alpha2 for which this device is receiving
	 * country IEs on, this can help disregard country IEs from APs
	 * on the same alpha2 quickly. The alpha2 may differ from
	 * cfg80211_regdomain's alpha2 when an intersection has occurred.
	 * If the AP is reconfigured this can also be used to tell us if
	 * the country on the country IE changed. */
	char country_ie_alpha2[2];

	/* If a Country IE has been received this tells us the environment
	 * which its telling us its in. This defaults to ENVIRON_ANY */
	enum environment_cap env;

	/* wiphy index, internal only */
	int idx;

	/* associate netdev list */
	struct mutex devlist_mtx;
	struct list_head netdev_list;

	/* must be last because of the way we do wiphy_priv(),
	 * and it should at least be aligned to NETDEV_ALIGN */
	struct wiphy wiphy __attribute__((__aligned__(NETDEV_ALIGN)));
};

static inline
struct cfg80211_registered_device *wiphy_to_dev(struct wiphy *wiphy)
{
	BUG_ON(!wiphy);
	return container_of(wiphy, struct cfg80211_registered_device, wiphy);
}

extern struct mutex cfg80211_drv_mutex;
extern struct list_head cfg80211_drv_list;

/*
 * This function returns a pointer to the driver
 * that the genl_info item that is passed refers to.
 * If successful, it returns non-NULL and also locks
 * the driver's mutex!
 *
 * This means that you need to call cfg80211_put_dev()
 * before being allowed to acquire &cfg80211_drv_mutex!
 *
 * This is necessary because we need to lock the global
 * mutex to get an item off the list safely, and then
 * we lock the drv mutex so it doesn't go away under us.
 *
 * We don't want to keep cfg80211_drv_mutex locked
 * for all the time in order to allow requests on
 * other interfaces to go through at the same time.
 *
 * The result of this can be a PTR_ERR and hence must
 * be checked with IS_ERR() for errors.
 */
extern struct cfg80211_registered_device *
cfg80211_get_dev_from_info(struct genl_info *info);

/* identical to cfg80211_get_dev_from_info but only operate on ifindex */
extern struct cfg80211_registered_device *
cfg80211_get_dev_from_ifindex(int ifindex);

extern void cfg80211_put_dev(struct cfg80211_registered_device *drv);

/* free object */
extern void cfg80211_dev_free(struct cfg80211_registered_device *drv);

extern int cfg80211_dev_rename(struct cfg80211_registered_device *drv,
			       char *newname);

void ieee80211_set_bitrate_flags(struct wiphy *wiphy);
void wiphy_update_regulatory(struct wiphy *wiphy, enum reg_set_by setby);

#endif /* __NET_WIRELESS_CORE_H */
