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
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

/*
 * LED Kernel Timer Trigger
 *
 * Copyright 2005-2006 Openedhand Ltd.
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
#include <linux/list.h>
#include <linux/spinlock.h>
#include <linux/device.h>
#include <linux/sysdev.h>
#include <linux/timer.h>
#include <linux/ctype.h>
#include <linux/leds.h>
#include "leds.h"

struct timer_trig_data {
	int brightness_on;		/* LED brightness during "on" period.
					 * (LED_OFF < brightness_on <= LED_FULL)
					 */
	unsigned long delay_on;		/* milliseconds on */
	unsigned long delay_off;	/* milliseconds off */
	struct timer_list timer;
};

static void led_timer_function(unsigned long data)
{
	struct led_classdev *led_cdev = (struct led_classdev *) data;
	struct timer_trig_data *timer_data = led_cdev->trigger_data;
	unsigned long brightness;
	unsigned long delay;

	if (!timer_data->delay_on || !timer_data->delay_off) {
		if (!timer_data->delay_on && !timer_data->delay_off)
			led_set_brightness(led_cdev, LED_OFF);
		return;
	}

	brightness = led_get_brightness(led_cdev);
	if (!brightness) {
		/* Time to switch the LED on. */
		brightness = timer_data->brightness_on;
		delay = timer_data->delay_on;
	} else {
		/* Store the current brightness value to be able
		 * to restore it when the delay_off period is over.
		 */
		timer_data->brightness_on = brightness;
		brightness = LED_OFF;
		delay = timer_data->delay_off;
	}

	led_set_brightness(led_cdev, brightness);

	mod_timer(&timer_data->timer, jiffies + msecs_to_jiffies(delay));
}

static ssize_t led_delay_on_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct led_classdev *led_cdev = dev_get_drvdata(dev);
	struct timer_trig_data *timer_data = led_cdev->trigger_data;

	return sprintf(buf, "%lu\n", timer_data->delay_on);
}

static ssize_t led_delay_on_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	struct led_classdev *led_cdev = dev_get_drvdata(dev);
	struct timer_trig_data *timer_data = led_cdev->trigger_data;
	int ret = -EINVAL;
	char *after;
	unsigned long state = simple_strtoul(buf, &after, 10);
	size_t count = after - buf;

	if (*after && isspace(*after))
		count++;

	if (count == size) {
		if (timer_data->delay_on != state) {
			/* the new value differs from the previous */
			timer_data->delay_on = state;

			/* deactivate previous settings */
			del_timer_sync(&timer_data->timer);

			/* try to activate hardware acceleration, if any */
			if (!led_cdev->blink_set ||
			    led_cdev->blink_set(led_cdev,
			      &timer_data->delay_on, &timer_data->delay_off)) {
				/* no hardware acceleration, blink via timer */
				mod_timer(&timer_data->timer, jiffies + 1);
			}
		}
		ret = count;
	}

	return ret;
}

static ssize_t led_delay_off_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct led_classdev *led_cdev = dev_get_drvdata(dev);
	struct timer_trig_data *timer_data = led_cdev->trigger_data;

	return sprintf(buf, "%lu\n", timer_data->delay_off);
}

static ssize_t led_delay_off_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	struct led_classdev *led_cdev = dev_get_drvdata(dev);
	struct timer_trig_data *timer_data = led_cdev->trigger_data;
	int ret = -EINVAL;
	char *after;
	unsigned long state = simple_strtoul(buf, &after, 10);
	size_t count = after - buf;

	if (*after && isspace(*after))
		count++;

	if (count == size) {
		if (timer_data->delay_off != state) {
			/* the new value differs from the previous */
			timer_data->delay_off = state;

			/* deactivate previous settings */
			del_timer_sync(&timer_data->timer);

			/* try to activate hardware acceleration, if any */
			if (!led_cdev->blink_set ||
			    led_cdev->blink_set(led_cdev,
			      &timer_data->delay_on, &timer_data->delay_off)) {
				/* no hardware acceleration, blink via timer */
				mod_timer(&timer_data->timer, jiffies + 1);
			}
		}
		ret = count;
	}

	return ret;
}

static DEVICE_ATTR(delay_on, 0666, led_delay_on_show, led_delay_on_store);
static DEVICE_ATTR(delay_off, 0666, led_delay_off_show, led_delay_off_store);

static void timer_trig_activate(struct led_classdev *led_cdev)
{
	struct timer_trig_data *timer_data;
	int rc;

	timer_data = kzalloc(sizeof(struct timer_trig_data), GFP_KERNEL);
	if (!timer_data)
		return;

	timer_data->brightness_on = led_get_brightness(led_cdev);
	if (timer_data->brightness_on == LED_OFF)
		timer_data->brightness_on = LED_FULL;
	led_cdev->trigger_data = timer_data;

	init_timer(&timer_data->timer);
	timer_data->timer.function = led_timer_function;
	timer_data->timer.data = (unsigned long) led_cdev;

	rc = device_create_file(led_cdev->dev, &dev_attr_delay_on);
	if (rc)
		goto err_out;
	rc = device_create_file(led_cdev->dev, &dev_attr_delay_off);
	if (rc)
		goto err_out_delayon;

	/* If there is hardware support for blinking, start one
	 * user friendly blink rate chosen by the driver.
	 */
	if (led_cdev->blink_set)
		led_cdev->blink_set(led_cdev,
			&timer_data->delay_on, &timer_data->delay_off);

	return;

err_out_delayon:
	device_remove_file(led_cdev->dev, &dev_attr_delay_on);
err_out:
	led_cdev->trigger_data = NULL;
	kfree(timer_data);
}

static void timer_trig_deactivate(struct led_classdev *led_cdev)
{
	struct timer_trig_data *timer_data = led_cdev->trigger_data;
	unsigned long on = 0, off = 0;

	if (timer_data) {
		device_remove_file(led_cdev->dev, &dev_attr_delay_on);
		device_remove_file(led_cdev->dev, &dev_attr_delay_off);
		del_timer_sync(&timer_data->timer);
		kfree(timer_data);
	}

	/* If there is hardware support for blinking, stop it */
	if (led_cdev->blink_set)
		led_cdev->blink_set(led_cdev, &on, &off);
}

static struct led_trigger timer_led_trigger = {
	.name     = "timer",
	.activate = timer_trig_activate,
	.deactivate = timer_trig_deactivate,
};

static int __init timer_trig_init(void)
{
	return led_trigger_register(&timer_led_trigger);
}

static void __exit timer_trig_exit(void)
{
	led_trigger_unregister(&timer_led_trigger);
}

module_init(timer_trig_init);
module_exit(timer_trig_exit);

MODULE_AUTHOR("Richard Purdie <rpurdie@openedhand.com>");
MODULE_DESCRIPTION("Timer LED trigger");
MODULE_LICENSE("GPL");
