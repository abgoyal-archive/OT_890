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
 * LED Triggers Core
 * For the HP Jornada 620/660/680/690 handhelds
 *
 * Copyright 2008 Kristoffer Ericson <kristoffer.ericson@gmail.com>
 *     this driver is based on leds-spitz.c by Richard Purdie.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/leds.h>
#include <asm/hd64461.h>
#include <mach/hp6xx.h>

static void hp6xxled_green_set(struct led_classdev *led_cdev,
			       enum led_brightness value)
{
	u8 v8;

	v8 = inb(PKDR);
	if (value)
		outb(v8 & (~PKDR_LED_GREEN), PKDR);
	else
		outb(v8 | PKDR_LED_GREEN, PKDR);
}

static void hp6xxled_red_set(struct led_classdev *led_cdev,
			     enum led_brightness value)
{
	u16 v16;

	v16 = inw(HD64461_GPBDR);
	if (value)
		outw(v16 & (~HD64461_GPBDR_LED_RED), HD64461_GPBDR);
	else
		outw(v16 | HD64461_GPBDR_LED_RED, HD64461_GPBDR);
}

static struct led_classdev hp6xx_red_led = {
	.name			= "hp6xx:red",
	.default_trigger	= "hp6xx-charge",
	.brightness_set		= hp6xxled_red_set,
	.flags			= LED_CORE_SUSPENDRESUME,
};

static struct led_classdev hp6xx_green_led = {
	.name			= "hp6xx:green",
	.default_trigger	= "ide-disk",
	.brightness_set		= hp6xxled_green_set,
	.flags			= LED_CORE_SUSPENDRESUME,
};

static int hp6xxled_probe(struct platform_device *pdev)
{
	int ret;

	ret = led_classdev_register(&pdev->dev, &hp6xx_red_led);
	if (ret < 0)
		return ret;

	ret = led_classdev_register(&pdev->dev, &hp6xx_green_led);
	if (ret < 0)
		led_classdev_unregister(&hp6xx_red_led);

	return ret;
}

static int hp6xxled_remove(struct platform_device *pdev)
{
	led_classdev_unregister(&hp6xx_red_led);
	led_classdev_unregister(&hp6xx_green_led);

	return 0;
}

/* work with hotplug and coldplug */
MODULE_ALIAS("platform:hp6xx-led");

static struct platform_driver hp6xxled_driver = {
	.probe		= hp6xxled_probe,
	.remove		= hp6xxled_remove,
	.driver		= {
		.name		= "hp6xx-led",
		.owner		= THIS_MODULE,
	},
};

static int __init hp6xxled_init(void)
{
	return platform_driver_register(&hp6xxled_driver);
}

static void __exit hp6xxled_exit(void)
{
	platform_driver_unregister(&hp6xxled_driver);
}

module_init(hp6xxled_init);
module_exit(hp6xxled_exit);

MODULE_AUTHOR("Kristoffer Ericson <kristoffer.ericson@gmail.com>");
MODULE_DESCRIPTION("HP Jornada 6xx LED driver");
MODULE_LICENSE("GPL");
