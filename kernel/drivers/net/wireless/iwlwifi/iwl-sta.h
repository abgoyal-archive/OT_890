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

/******************************************************************************
 *
 * Copyright(c) 2003 - 2008 Intel Corporation. All rights reserved.
 *
 * Portions of this file are derived from the ipw3945 project, as well
 * as portions of the ieee80211 subsystem header files.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
 *
 * The full GNU General Public License is included in this distribution in the
 * file called LICENSE.
 *
 * Contact Information:
 *  Intel Linux Wireless <ilw@linux.intel.com>
 * Intel Corporation, 5200 N.E. Elam Young Parkway, Hillsboro, OR 97124-6497
 *
 *****************************************************************************/
#ifndef __iwl_sta_h__
#define __iwl_sta_h__

#define HW_KEY_DYNAMIC 0
#define HW_KEY_DEFAULT 1

/**
 * iwl_find_station - Find station id for a given BSSID
 * @bssid: MAC address of station ID to find
 */
u8 iwl_find_station(struct iwl_priv *priv, const u8 *bssid);

int iwl_send_static_wepkey_cmd(struct iwl_priv *priv, u8 send_if_empty);
int iwl_remove_default_wep_key(struct iwl_priv *priv,
			       struct ieee80211_key_conf *key);
int iwl_set_default_wep_key(struct iwl_priv *priv,
			    struct ieee80211_key_conf *key);
int iwl_set_dynamic_key(struct iwl_priv *priv,
			struct ieee80211_key_conf *key, u8 sta_id);
int iwl_remove_dynamic_key(struct iwl_priv *priv,
			   struct ieee80211_key_conf *key, u8 sta_id);
void iwl_update_tkip_key(struct iwl_priv *priv,
			struct ieee80211_key_conf *keyconf,
			const u8 *addr, u32 iv32, u16 *phase1key);

int iwl_rxon_add_station(struct iwl_priv *priv, const u8 *addr, int is_ap);
int iwl_remove_station(struct iwl_priv *priv, const u8 *addr, int is_ap);
void iwl_clear_stations_table(struct iwl_priv *priv);
int iwl_get_sta_id(struct iwl_priv *priv, struct ieee80211_hdr *hdr);
int iwl_get_ra_sta_id(struct iwl_priv *priv, struct ieee80211_hdr *hdr);
u8 iwl_add_station_flags(struct iwl_priv *priv, const u8 *addr,
			int is_ap, u8 flags,
			struct ieee80211_sta_ht_cap *ht_info);
void iwl_sta_tx_modify_enable_tid(struct iwl_priv *priv, int sta_id, int tid);
int iwl_sta_rx_agg_start(struct iwl_priv *priv,
			 const u8 *addr, int tid, u16 ssn);
int iwl_sta_rx_agg_stop(struct iwl_priv *priv, const u8 *addr, int tid);
void iwl_update_ps_mode(struct iwl_priv *priv, u16 ps_bit, u8 *addr);
#endif /* __iwl_sta_h__ */
