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
 *  LEDs triggers for power supply class
 *
 *  Copyright © 2007  Anton Vorontsov <cbou@mail.ru>
 *  Copyright © 2004  Szabolcs Gyurko
 *  Copyright © 2003  Ian Molton <spyro@f2s.com>
 *
 *  Modified: 2004, Oct     Szabolcs Gyurko
 *
 *  You may use this code as per GPL version 2
 */

#include <linux/kernel.h>
#include <linux/power_supply.h>

#include "power_supply.h"

/* Battery specific LEDs triggers. */

static void power_supply_update_bat_leds(struct power_supply *psy)
{
	union power_supply_propval status;

	if (psy->get_property(psy, POWER_SUPPLY_PROP_STATUS, &status))
		return;

	dev_dbg(psy->dev, "%s %d\n", __func__, status.intval);

	switch (status.intval) {
	case POWER_SUPPLY_STATUS_FULL:
		led_trigger_event(psy->charging_full_trig, LED_FULL);
		led_trigger_event(psy->charging_trig, LED_OFF);
		led_trigger_event(psy->full_trig, LED_FULL);
		break;
	case POWER_SUPPLY_STATUS_CHARGING:
		led_trigger_event(psy->charging_full_trig, LED_FULL);
		led_trigger_event(psy->charging_trig, LED_FULL);
		led_trigger_event(psy->full_trig, LED_OFF);
		break;
	default:
		led_trigger_event(psy->charging_full_trig, LED_OFF);
		led_trigger_event(psy->charging_trig, LED_OFF);
		led_trigger_event(psy->full_trig, LED_OFF);
		break;
	}
}

static int power_supply_create_bat_triggers(struct power_supply *psy)
{
	int rc = 0;

	psy->charging_full_trig_name = kasprintf(GFP_KERNEL,
					"%s-charging-or-full", psy->name);
	if (!psy->charging_full_trig_name)
		goto charging_full_failed;

	psy->charging_trig_name = kasprintf(GFP_KERNEL,
					"%s-charging", psy->name);
	if (!psy->charging_trig_name)
		goto charging_failed;

	psy->full_trig_name = kasprintf(GFP_KERNEL, "%s-full", psy->name);
	if (!psy->full_trig_name)
		goto full_failed;

	led_trigger_register_simple(psy->charging_full_trig_name,
				    &psy->charging_full_trig);
	led_trigger_register_simple(psy->charging_trig_name,
				    &psy->charging_trig);
	led_trigger_register_simple(psy->full_trig_name,
				    &psy->full_trig);

	goto success;

full_failed:
	kfree(psy->charging_trig_name);
charging_failed:
	kfree(psy->charging_full_trig_name);
charging_full_failed:
	rc = -ENOMEM;
success:
	return rc;
}

static void power_supply_remove_bat_triggers(struct power_supply *psy)
{
	led_trigger_unregister_simple(psy->charging_full_trig);
	led_trigger_unregister_simple(psy->charging_trig);
	led_trigger_unregister_simple(psy->full_trig);
	kfree(psy->full_trig_name);
	kfree(psy->charging_trig_name);
	kfree(psy->charging_full_trig_name);
}

/* Generated power specific LEDs triggers. */

static void power_supply_update_gen_leds(struct power_supply *psy)
{
	union power_supply_propval online;

	if (psy->get_property(psy, POWER_SUPPLY_PROP_ONLINE, &online))
		return;

	dev_dbg(psy->dev, "%s %d\n", __func__, online.intval);

	if (online.intval)
		led_trigger_event(psy->online_trig, LED_FULL);
	else
		led_trigger_event(psy->online_trig, LED_OFF);
}

static int power_supply_create_gen_triggers(struct power_supply *psy)
{
	int rc = 0;

	psy->online_trig_name = kasprintf(GFP_KERNEL, "%s-online", psy->name);
	if (!psy->online_trig_name)
		goto online_failed;

	led_trigger_register_simple(psy->online_trig_name, &psy->online_trig);

	goto success;

online_failed:
	rc = -ENOMEM;
success:
	return rc;
}

static void power_supply_remove_gen_triggers(struct power_supply *psy)
{
	led_trigger_unregister_simple(psy->online_trig);
	kfree(psy->online_trig_name);
}

/* Choice what triggers to create&update. */

void power_supply_update_leds(struct power_supply *psy)
{
	if (psy->type == POWER_SUPPLY_TYPE_BATTERY)
		power_supply_update_bat_leds(psy);
	else
		power_supply_update_gen_leds(psy);
}

int power_supply_create_triggers(struct power_supply *psy)
{
	if (psy->type == POWER_SUPPLY_TYPE_BATTERY)
		return power_supply_create_bat_triggers(psy);
	return power_supply_create_gen_triggers(psy);
}

void power_supply_remove_triggers(struct power_supply *psy)
{
	if (psy->type == POWER_SUPPLY_TYPE_BATTERY)
		power_supply_remove_bat_triggers(psy);
	else
		power_supply_remove_gen_triggers(psy);
}
