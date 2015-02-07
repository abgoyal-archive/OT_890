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
 * via-pmu LED class device
 *
 * Copyright 2006 Johannes Berg <johannes@sipsolutions.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, GOOD TITLE or
 * NON INFRINGEMENT.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 *
 */
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/leds.h>
#include <linux/adb.h>
#include <linux/pmu.h>
#include <asm/prom.h>

static spinlock_t pmu_blink_lock;
static struct adb_request pmu_blink_req;
/* -1: no change, 0: request off, 1: request on */
static int requested_change;

static void pmu_req_done(struct adb_request * req)
{
	unsigned long flags;

	spin_lock_irqsave(&pmu_blink_lock, flags);
	/* if someone requested a change in the meantime
	 * (we only see the last one which is fine)
	 * then apply it now */
	if (requested_change != -1 && !pmu_sys_suspended)
		pmu_request(&pmu_blink_req, NULL, 4, 0xee, 4, 0, requested_change);
	/* reset requested change */
	requested_change = -1;
	spin_unlock_irqrestore(&pmu_blink_lock, flags);
}

static void pmu_led_set(struct led_classdev *led_cdev,
			enum led_brightness brightness)
{
	unsigned long flags;

	spin_lock_irqsave(&pmu_blink_lock, flags);
	switch (brightness) {
	case LED_OFF:
		requested_change = 0;
		break;
	case LED_FULL:
		requested_change = 1;
		break;
	default:
		goto out;
		break;
	}
	/* if request isn't done, then don't do anything */
	if (pmu_blink_req.complete && !pmu_sys_suspended)
		pmu_request(&pmu_blink_req, NULL, 4, 0xee, 4, 0, requested_change);
 out:
 	spin_unlock_irqrestore(&pmu_blink_lock, flags);
}

static struct led_classdev pmu_led = {
	.name = "pmu-front-led",
#ifdef CONFIG_ADB_PMU_LED_IDE
	.default_trigger = "ide-disk",
#endif
	.brightness_set = pmu_led_set,
};

static int __init via_pmu_led_init(void)
{
	struct device_node *dt;
	const char *model;

	/* only do this on keylargo based models */
	if (pmu_get_model() != PMU_KEYLARGO_BASED)
		return -ENODEV;

	dt = of_find_node_by_path("/");
	if (dt == NULL)
		return -ENODEV;
	model = of_get_property(dt, "model", NULL);
	if (model == NULL)
		return -ENODEV;
	if (strncmp(model, "PowerBook", strlen("PowerBook")) != 0 &&
	    strncmp(model, "iBook", strlen("iBook")) != 0 &&
	    strcmp(model, "PowerMac7,2") != 0 &&
	    strcmp(model, "PowerMac7,3") != 0) {
		of_node_put(dt);
		/* ignore */
		return -ENODEV;
	}
	of_node_put(dt);

	spin_lock_init(&pmu_blink_lock);
	/* no outstanding req */
	pmu_blink_req.complete = 1;
	pmu_blink_req.done = pmu_req_done;

	return led_classdev_register(NULL, &pmu_led);
}

late_initcall(via_pmu_led_init);
