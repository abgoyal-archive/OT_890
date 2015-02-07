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
 * LED Driver for the Freecom FSG-3
 *
 * Copyright (c) 2008 Rod Whitby <rod@whitby.id.au>
 *
 * Author: Rod Whitby <rod@whitby.id.au>
 *
 * Based on leds-spitz.c
 * Copyright 2005-2006 Openedhand Ltd.
 * Author: Richard Purdie <rpurdie@openedhand.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/leds.h>
#include <mach/hardware.h>
#include <asm/io.h>

static short __iomem *latch_address;
static unsigned short latch_value;


static void fsg_led_wlan_set(struct led_classdev *led_cdev,
			     enum led_brightness value)
{
	if (value) {
		latch_value &= ~(1 << FSG_LED_WLAN_BIT);
		*latch_address = latch_value;
	} else {
		latch_value |=  (1 << FSG_LED_WLAN_BIT);
		*latch_address = latch_value;
	}
}

static void fsg_led_wan_set(struct led_classdev *led_cdev,
			    enum led_brightness value)
{
	if (value) {
		latch_value &= ~(1 << FSG_LED_WAN_BIT);
		*latch_address = latch_value;
	} else {
		latch_value |=  (1 << FSG_LED_WAN_BIT);
		*latch_address = latch_value;
	}
}

static void fsg_led_sata_set(struct led_classdev *led_cdev,
			     enum led_brightness value)
{
	if (value) {
		latch_value &= ~(1 << FSG_LED_SATA_BIT);
		*latch_address = latch_value;
	} else {
		latch_value |=  (1 << FSG_LED_SATA_BIT);
		*latch_address = latch_value;
	}
}

static void fsg_led_usb_set(struct led_classdev *led_cdev,
			    enum led_brightness value)
{
	if (value) {
		latch_value &= ~(1 << FSG_LED_USB_BIT);
		*latch_address = latch_value;
	} else {
		latch_value |=  (1 << FSG_LED_USB_BIT);
		*latch_address = latch_value;
	}
}

static void fsg_led_sync_set(struct led_classdev *led_cdev,
			     enum led_brightness value)
{
	if (value) {
		latch_value &= ~(1 << FSG_LED_SYNC_BIT);
		*latch_address = latch_value;
	} else {
		latch_value |=  (1 << FSG_LED_SYNC_BIT);
		*latch_address = latch_value;
	}
}

static void fsg_led_ring_set(struct led_classdev *led_cdev,
			     enum led_brightness value)
{
	if (value) {
		latch_value &= ~(1 << FSG_LED_RING_BIT);
		*latch_address = latch_value;
	} else {
		latch_value |=  (1 << FSG_LED_RING_BIT);
		*latch_address = latch_value;
	}
}


static struct led_classdev fsg_wlan_led = {
	.name			= "fsg:blue:wlan",
	.brightness_set		= fsg_led_wlan_set,
	.flags			= LED_CORE_SUSPENDRESUME,
};

static struct led_classdev fsg_wan_led = {
	.name			= "fsg:blue:wan",
	.brightness_set		= fsg_led_wan_set,
	.flags			= LED_CORE_SUSPENDRESUME,
};

static struct led_classdev fsg_sata_led = {
	.name			= "fsg:blue:sata",
	.brightness_set		= fsg_led_sata_set,
	.flags			= LED_CORE_SUSPENDRESUME,
};

static struct led_classdev fsg_usb_led = {
	.name			= "fsg:blue:usb",
	.brightness_set		= fsg_led_usb_set,
	.flags			= LED_CORE_SUSPENDRESUME,
};

static struct led_classdev fsg_sync_led = {
	.name			= "fsg:blue:sync",
	.brightness_set		= fsg_led_sync_set,
	.flags			= LED_CORE_SUSPENDRESUME,
};

static struct led_classdev fsg_ring_led = {
	.name			= "fsg:blue:ring",
	.brightness_set		= fsg_led_ring_set,
	.flags			= LED_CORE_SUSPENDRESUME,
};


static int fsg_led_probe(struct platform_device *pdev)
{
	int ret;

	/* Map the LED chip select address space */
	latch_address = (unsigned short *) ioremap(IXP4XX_EXP_BUS_BASE(2), 512);
	if (!latch_address) {
		ret = -ENOMEM;
		goto failremap;
	}

	latch_value = 0xffff;
	*latch_address = latch_value;

	ret = led_classdev_register(&pdev->dev, &fsg_wlan_led);
	if (ret < 0)
		goto failwlan;

	ret = led_classdev_register(&pdev->dev, &fsg_wan_led);
	if (ret < 0)
		goto failwan;

	ret = led_classdev_register(&pdev->dev, &fsg_sata_led);
	if (ret < 0)
		goto failsata;

	ret = led_classdev_register(&pdev->dev, &fsg_usb_led);
	if (ret < 0)
		goto failusb;

	ret = led_classdev_register(&pdev->dev, &fsg_sync_led);
	if (ret < 0)
		goto failsync;

	ret = led_classdev_register(&pdev->dev, &fsg_ring_led);
	if (ret < 0)
		goto failring;

	return ret;

 failring:
	led_classdev_unregister(&fsg_sync_led);
 failsync:
	led_classdev_unregister(&fsg_usb_led);
 failusb:
	led_classdev_unregister(&fsg_sata_led);
 failsata:
	led_classdev_unregister(&fsg_wan_led);
 failwan:
	led_classdev_unregister(&fsg_wlan_led);
 failwlan:
	iounmap(latch_address);
 failremap:

	return ret;
}

static int fsg_led_remove(struct platform_device *pdev)
{
	led_classdev_unregister(&fsg_wlan_led);
	led_classdev_unregister(&fsg_wan_led);
	led_classdev_unregister(&fsg_sata_led);
	led_classdev_unregister(&fsg_usb_led);
	led_classdev_unregister(&fsg_sync_led);
	led_classdev_unregister(&fsg_ring_led);

	iounmap(latch_address);

	return 0;
}


static struct platform_driver fsg_led_driver = {
	.probe		= fsg_led_probe,
	.remove		= fsg_led_remove,
	.driver		= {
		.name		= "fsg-led",
	},
};


static int __init fsg_led_init(void)
{
	return platform_driver_register(&fsg_led_driver);
}

static void __exit fsg_led_exit(void)
{
	platform_driver_unregister(&fsg_led_driver);
}


module_init(fsg_led_init);
module_exit(fsg_led_exit);

MODULE_AUTHOR("Rod Whitby <rod@whitby.id.au>");
MODULE_DESCRIPTION("Freecom FSG-3 LED driver");
MODULE_LICENSE("GPL");
