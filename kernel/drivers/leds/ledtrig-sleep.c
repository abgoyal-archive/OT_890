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

/* drivers/leds/ledtrig-sleep.c
 *
 * Copyright (C) 2007 Google, Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/earlysuspend.h>
#include <linux/leds.h>
#include <linux/suspend.h>

static int ledtrig_sleep_pm_callback(struct notifier_block *nfb,
					unsigned long action,
					void *ignored);

DEFINE_LED_TRIGGER(ledtrig_sleep)
static struct notifier_block ledtrig_sleep_pm_notifier = {
	.notifier_call = ledtrig_sleep_pm_callback,
	.priority = 0,
};

static void ledtrig_sleep_early_suspend(struct early_suspend *h)
{
	led_trigger_event(ledtrig_sleep, LED_FULL);
}

static void ledtrig_sleep_early_resume(struct early_suspend *h)
{
	led_trigger_event(ledtrig_sleep, LED_OFF);
}

static struct early_suspend ledtrig_sleep_early_suspend_handler = {
	.suspend = ledtrig_sleep_early_suspend,
	.resume = ledtrig_sleep_early_resume,
};

static int ledtrig_sleep_pm_callback(struct notifier_block *nfb,
					unsigned long action,
					void *ignored)
{
	switch (action) {
	case PM_HIBERNATION_PREPARE:
	case PM_SUSPEND_PREPARE:
		led_trigger_event(ledtrig_sleep, LED_OFF);
		return NOTIFY_OK;
	case PM_POST_HIBERNATION:
	case PM_POST_SUSPEND:
		led_trigger_event(ledtrig_sleep, LED_FULL);
		return NOTIFY_OK;
	}

	return NOTIFY_DONE;
}

static int __init ledtrig_sleep_init(void)
{
	led_trigger_register_simple("sleep", &ledtrig_sleep);
	register_pm_notifier(&ledtrig_sleep_pm_notifier);
	register_early_suspend(&ledtrig_sleep_early_suspend_handler);
	return 0;
}

static void __exit ledtrig_sleep_exit(void)
{
	unregister_early_suspend(&ledtrig_sleep_early_suspend_handler);
	unregister_pm_notifier(&ledtrig_sleep_pm_notifier);
	led_trigger_unregister_simple(ledtrig_sleep);
}

module_init(ledtrig_sleep_init);
module_exit(ledtrig_sleep_exit);

