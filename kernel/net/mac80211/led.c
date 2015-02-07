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
 * Copyright 2006, Johannes Berg <johannes@sipsolutions.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

/* just for IFNAMSIZ */
#include <linux/if.h>
#include "led.h"

void ieee80211_led_rx(struct ieee80211_local *local)
{
	if (unlikely(!local->rx_led))
		return;
	if (local->rx_led_counter++ % 2 == 0)
		led_trigger_event(local->rx_led, LED_OFF);
	else
		led_trigger_event(local->rx_led, LED_FULL);
}

/* q is 1 if a packet was enqueued, 0 if it has been transmitted */
void ieee80211_led_tx(struct ieee80211_local *local, int q)
{
	if (unlikely(!local->tx_led))
		return;
	/* not sure how this is supposed to work ... */
	local->tx_led_counter += 2*q-1;
	if (local->tx_led_counter % 2 == 0)
		led_trigger_event(local->tx_led, LED_OFF);
	else
		led_trigger_event(local->tx_led, LED_FULL);
}

void ieee80211_led_assoc(struct ieee80211_local *local, bool associated)
{
	if (unlikely(!local->assoc_led))
		return;
	if (associated)
		led_trigger_event(local->assoc_led, LED_FULL);
	else
		led_trigger_event(local->assoc_led, LED_OFF);
}

void ieee80211_led_radio(struct ieee80211_local *local, bool enabled)
{
	if (unlikely(!local->radio_led))
		return;
	if (enabled)
		led_trigger_event(local->radio_led, LED_FULL);
	else
		led_trigger_event(local->radio_led, LED_OFF);
}

void ieee80211_led_init(struct ieee80211_local *local)
{
	local->rx_led = kzalloc(sizeof(struct led_trigger), GFP_KERNEL);
	if (local->rx_led) {
		snprintf(local->rx_led_name, sizeof(local->rx_led_name),
			 "%srx", wiphy_name(local->hw.wiphy));
		local->rx_led->name = local->rx_led_name;
		if (led_trigger_register(local->rx_led)) {
			kfree(local->rx_led);
			local->rx_led = NULL;
		}
	}

	local->tx_led = kzalloc(sizeof(struct led_trigger), GFP_KERNEL);
	if (local->tx_led) {
		snprintf(local->tx_led_name, sizeof(local->tx_led_name),
			 "%stx", wiphy_name(local->hw.wiphy));
		local->tx_led->name = local->tx_led_name;
		if (led_trigger_register(local->tx_led)) {
			kfree(local->tx_led);
			local->tx_led = NULL;
		}
	}

	local->assoc_led = kzalloc(sizeof(struct led_trigger), GFP_KERNEL);
	if (local->assoc_led) {
		snprintf(local->assoc_led_name, sizeof(local->assoc_led_name),
			 "%sassoc", wiphy_name(local->hw.wiphy));
		local->assoc_led->name = local->assoc_led_name;
		if (led_trigger_register(local->assoc_led)) {
			kfree(local->assoc_led);
			local->assoc_led = NULL;
		}
	}

	local->radio_led = kzalloc(sizeof(struct led_trigger), GFP_KERNEL);
	if (local->radio_led) {
		snprintf(local->radio_led_name, sizeof(local->radio_led_name),
			 "%sradio", wiphy_name(local->hw.wiphy));
		local->radio_led->name = local->radio_led_name;
		if (led_trigger_register(local->radio_led)) {
			kfree(local->radio_led);
			local->radio_led = NULL;
		}
	}
}

void ieee80211_led_exit(struct ieee80211_local *local)
{
	if (local->radio_led) {
		led_trigger_unregister(local->radio_led);
		kfree(local->radio_led);
	}
	if (local->assoc_led) {
		led_trigger_unregister(local->assoc_led);
		kfree(local->assoc_led);
	}
	if (local->tx_led) {
		led_trigger_unregister(local->tx_led);
		kfree(local->tx_led);
	}
	if (local->rx_led) {
		led_trigger_unregister(local->rx_led);
		kfree(local->rx_led);
	}
}

char *__ieee80211_get_radio_led_name(struct ieee80211_hw *hw)
{
	struct ieee80211_local *local = hw_to_local(hw);

	if (local->radio_led)
		return local->radio_led_name;
	return NULL;
}
EXPORT_SYMBOL(__ieee80211_get_radio_led_name);

char *__ieee80211_get_assoc_led_name(struct ieee80211_hw *hw)
{
	struct ieee80211_local *local = hw_to_local(hw);

	if (local->assoc_led)
		return local->assoc_led_name;
	return NULL;
}
EXPORT_SYMBOL(__ieee80211_get_assoc_led_name);

char *__ieee80211_get_tx_led_name(struct ieee80211_hw *hw)
{
	struct ieee80211_local *local = hw_to_local(hw);

	if (local->tx_led)
		return local->tx_led_name;
	return NULL;
}
EXPORT_SYMBOL(__ieee80211_get_tx_led_name);

char *__ieee80211_get_rx_led_name(struct ieee80211_hw *hw)
{
	struct ieee80211_local *local = hw_to_local(hw);

	if (local->rx_led)
		return local->rx_led_name;
	return NULL;
}
EXPORT_SYMBOL(__ieee80211_get_rx_led_name);
