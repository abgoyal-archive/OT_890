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
 * LED IDE-Disk Activity Trigger
 *
 * Copyright 2006 Openedhand Ltd.
 *
 * Author: Richard Purdie <rpurdie@openedhand.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#include <linux/module.h>
#include <linux/jiffies.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/timer.h>
#include <linux/leds.h>

static void ledtrig_ide_timerfunc(unsigned long data);

DEFINE_LED_TRIGGER(ledtrig_ide);
static DEFINE_TIMER(ledtrig_ide_timer, ledtrig_ide_timerfunc, 0, 0);
static int ide_activity;
static int ide_lastactivity;

void ledtrig_ide_activity(void)
{
	ide_activity++;
	if (!timer_pending(&ledtrig_ide_timer))
		mod_timer(&ledtrig_ide_timer, jiffies + msecs_to_jiffies(10));
}
EXPORT_SYMBOL(ledtrig_ide_activity);

static void ledtrig_ide_timerfunc(unsigned long data)
{
	if (ide_lastactivity != ide_activity) {
		ide_lastactivity = ide_activity;
		led_trigger_event(ledtrig_ide, LED_FULL);
		mod_timer(&ledtrig_ide_timer, jiffies + msecs_to_jiffies(10));
	} else {
		led_trigger_event(ledtrig_ide, LED_OFF);
	}
}

static int __init ledtrig_ide_init(void)
{
	led_trigger_register_simple("ide-disk", &ledtrig_ide);
	return 0;
}

static void __exit ledtrig_ide_exit(void)
{
	led_trigger_unregister_simple(ledtrig_ide);
}

module_init(ledtrig_ide_init);
module_exit(ledtrig_ide_exit);

MODULE_AUTHOR("Richard Purdie <rpurdie@openedhand.com>");
MODULE_DESCRIPTION("LED IDE Disk Activity Trigger");
MODULE_LICENSE("GPL");
