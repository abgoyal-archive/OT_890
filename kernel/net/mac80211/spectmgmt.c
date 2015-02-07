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
 * spectrum management
 *
 * Copyright 2003, Jouni Malinen <jkmaline@cc.hut.fi>
 * Copyright 2002-2005, Instant802 Networks, Inc.
 * Copyright 2005-2006, Devicescape Software, Inc.
 * Copyright 2006-2007  Jiri Benc <jbenc@suse.cz>
 * Copyright 2007, Michael Wu <flamingice@sourmilk.net>
 * Copyright 2007-2008, Intel Corporation
 * Copyright 2008, Johannes Berg <johannes@sipsolutions.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/ieee80211.h>
#include <net/wireless.h>
#include <net/mac80211.h>
#include "ieee80211_i.h"
#include "sta_info.h"
#include "wme.h"

static void ieee80211_send_refuse_measurement_request(struct ieee80211_sub_if_data *sdata,
					struct ieee80211_msrment_ie *request_ie,
					const u8 *da, const u8 *bssid,
					u8 dialog_token)
{
	struct ieee80211_local *local = sdata->local;
	struct sk_buff *skb;
	struct ieee80211_mgmt *msr_report;

	skb = dev_alloc_skb(sizeof(*msr_report) + local->hw.extra_tx_headroom +
				sizeof(struct ieee80211_msrment_ie));

	if (!skb) {
		printk(KERN_ERR "%s: failed to allocate buffer for "
				"measurement report frame\n", sdata->dev->name);
		return;
	}

	skb_reserve(skb, local->hw.extra_tx_headroom);
	msr_report = (struct ieee80211_mgmt *)skb_put(skb, 24);
	memset(msr_report, 0, 24);
	memcpy(msr_report->da, da, ETH_ALEN);
	memcpy(msr_report->sa, sdata->dev->dev_addr, ETH_ALEN);
	memcpy(msr_report->bssid, bssid, ETH_ALEN);
	msr_report->frame_control = cpu_to_le16(IEEE80211_FTYPE_MGMT |
						IEEE80211_STYPE_ACTION);

	skb_put(skb, 1 + sizeof(msr_report->u.action.u.measurement));
	msr_report->u.action.category = WLAN_CATEGORY_SPECTRUM_MGMT;
	msr_report->u.action.u.measurement.action_code =
				WLAN_ACTION_SPCT_MSR_RPRT;
	msr_report->u.action.u.measurement.dialog_token = dialog_token;

	msr_report->u.action.u.measurement.element_id = WLAN_EID_MEASURE_REPORT;
	msr_report->u.action.u.measurement.length =
			sizeof(struct ieee80211_msrment_ie);

	memset(&msr_report->u.action.u.measurement.msr_elem, 0,
		sizeof(struct ieee80211_msrment_ie));
	msr_report->u.action.u.measurement.msr_elem.token = request_ie->token;
	msr_report->u.action.u.measurement.msr_elem.mode |=
			IEEE80211_SPCT_MSR_RPRT_MODE_REFUSED;
	msr_report->u.action.u.measurement.msr_elem.type = request_ie->type;

	ieee80211_tx_skb(sdata, skb, 0);
}

void ieee80211_process_measurement_req(struct ieee80211_sub_if_data *sdata,
				       struct ieee80211_mgmt *mgmt,
				       size_t len)
{
	/*
	 * Ignoring measurement request is spec violation.
	 * Mandatory measurements must be reported optional
	 * measurements might be refused or reported incapable
	 * For now just refuse
	 * TODO: Answer basic measurement as unmeasured
	 */
	ieee80211_send_refuse_measurement_request(sdata,
			&mgmt->u.action.u.measurement.msr_elem,
			mgmt->sa, mgmt->bssid,
			mgmt->u.action.u.measurement.dialog_token);
}
