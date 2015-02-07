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
 * linux/drivers/leds/leds-locomo.c
 *
 * Copyright (C) 2005 John Lenz <lenz@cs.wisc.edu>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/leds.h>

#include <mach/hardware.h>
#include <asm/hardware/locomo.h>

static void locomoled_brightness_set(struct led_classdev *led_cdev,
				enum led_brightness value, int offset)
{
	struct locomo_dev *locomo_dev = LOCOMO_DEV(led_cdev->dev->parent);
	unsigned long flags;

	local_irq_save(flags);
	if (value)
		locomo_writel(LOCOMO_LPT_TOFH, locomo_dev->mapbase + offset);
	else
		locomo_writel(LOCOMO_LPT_TOFL, locomo_dev->mapbase + offset);
	local_irq_restore(flags);
}

static void locomoled_brightness_set0(struct led_classdev *led_cdev,
				enum led_brightness value)
{
	locomoled_brightness_set(led_cdev, value, LOCOMO_LPT0);
}

static void locomoled_brightness_set1(struct led_classdev *led_cdev,
				enum led_brightness value)
{
	locomoled_brightness_set(led_cdev, value, LOCOMO_LPT1);
}

static struct led_classdev locomo_led0 = {
	.name			= "locomo:amber:charge",
	.default_trigger	= "sharpsl-charge",
	.brightness_set		= locomoled_brightness_set0,
};

static struct led_classdev locomo_led1 = {
	.name			= "locomo:green:mail",
	.default_trigger	= "nand-disk",
	.brightness_set		= locomoled_brightness_set1,
};

static int locomoled_probe(struct locomo_dev *ldev)
{
	int ret;

	ret = led_classdev_register(&ldev->dev, &locomo_led0);
	if (ret < 0)
		return ret;

	ret = led_classdev_register(&ldev->dev, &locomo_led1);
	if (ret < 0)
		led_classdev_unregister(&locomo_led0);

	return ret;
}

static int locomoled_remove(struct locomo_dev *dev)
{
	led_classdev_unregister(&locomo_led0);
	led_classdev_unregister(&locomo_led1);
	return 0;
}

static struct locomo_driver locomoled_driver = {
	.drv = {
		.name = "locomoled"
	},
	.devid	= LOCOMO_DEVID_LED,
	.probe	= locomoled_probe,
	.remove	= locomoled_remove,
};

static int __init locomoled_init(void)
{
	return locomo_driver_register(&locomoled_driver);
}
module_init(locomoled_init);

MODULE_AUTHOR("John Lenz <lenz@cs.wisc.edu>");
MODULE_DESCRIPTION("Locomo LED driver");
MODULE_LICENSE("GPL");
