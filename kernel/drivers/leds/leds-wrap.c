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
 * LEDs driver for PCEngines WRAP
 *
 * Copyright (C) 2006 Kristian Kielhofner <kris@krisk.org>
 *
 * Based on leds-net48xx.c
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/leds.h>
#include <linux/err.h>
#include <asm/io.h>
#include <linux/scx200_gpio.h>

#define DRVNAME "wrap-led"
#define WRAP_POWER_LED_GPIO	2
#define WRAP_ERROR_LED_GPIO	3
#define WRAP_EXTRA_LED_GPIO	18

static struct platform_device *pdev;

static void wrap_power_led_set(struct led_classdev *led_cdev,
		enum led_brightness value)
{
	if (value)
		scx200_gpio_set_low(WRAP_POWER_LED_GPIO);
	else
		scx200_gpio_set_high(WRAP_POWER_LED_GPIO);
}

static void wrap_error_led_set(struct led_classdev *led_cdev,
		enum led_brightness value)
{
	if (value)
		scx200_gpio_set_low(WRAP_ERROR_LED_GPIO);
	else
		scx200_gpio_set_high(WRAP_ERROR_LED_GPIO);
}

static void wrap_extra_led_set(struct led_classdev *led_cdev,
		enum led_brightness value)
{
	if (value)
		scx200_gpio_set_low(WRAP_EXTRA_LED_GPIO);
	else
		scx200_gpio_set_high(WRAP_EXTRA_LED_GPIO);
}

static struct led_classdev wrap_power_led = {
	.name			= "wrap::power",
	.brightness_set		= wrap_power_led_set,
	.default_trigger	= "default-on",
	.flags			= LED_CORE_SUSPENDRESUME,
};

static struct led_classdev wrap_error_led = {
	.name		= "wrap::error",
	.brightness_set	= wrap_error_led_set,
	.flags			= LED_CORE_SUSPENDRESUME,
};

static struct led_classdev wrap_extra_led = {
	.name           = "wrap::extra",
	.brightness_set = wrap_extra_led_set,
	.flags			= LED_CORE_SUSPENDRESUME,
};

static int wrap_led_probe(struct platform_device *pdev)
{
	int ret;

	ret = led_classdev_register(&pdev->dev, &wrap_power_led);
	if (ret < 0)
		return ret;

	ret = led_classdev_register(&pdev->dev, &wrap_error_led);
	if (ret < 0)
		goto err1;

	ret = led_classdev_register(&pdev->dev, &wrap_extra_led);
	if (ret < 0)
		goto err2;

	return ret;

err2:
	led_classdev_unregister(&wrap_error_led);
err1:
	led_classdev_unregister(&wrap_power_led);

	return ret;
}

static int wrap_led_remove(struct platform_device *pdev)
{
	led_classdev_unregister(&wrap_power_led);
	led_classdev_unregister(&wrap_error_led);
	led_classdev_unregister(&wrap_extra_led);
	return 0;
}

static struct platform_driver wrap_led_driver = {
	.probe		= wrap_led_probe,
	.remove		= wrap_led_remove,
	.driver		= {
		.name		= DRVNAME,
		.owner		= THIS_MODULE,
	},
};

static int __init wrap_led_init(void)
{
	int ret;

	if (!scx200_gpio_present()) {
		ret = -ENODEV;
		goto out;
	}

	ret = platform_driver_register(&wrap_led_driver);
	if (ret < 0)
		goto out;

	pdev = platform_device_register_simple(DRVNAME, -1, NULL, 0);
	if (IS_ERR(pdev)) {
		ret = PTR_ERR(pdev);
		platform_driver_unregister(&wrap_led_driver);
		goto out;
	}

out:
	return ret;
}

static void __exit wrap_led_exit(void)
{
	platform_device_unregister(pdev);
	platform_driver_unregister(&wrap_led_driver);
}

module_init(wrap_led_init);
module_exit(wrap_led_exit);

MODULE_AUTHOR("Kristian Kielhofner <kris@krisk.org>");
MODULE_DESCRIPTION("PCEngines WRAP LED driver");
MODULE_LICENSE("GPL");

