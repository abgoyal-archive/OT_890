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
 * Copyright 2006 - Florian Fainelli <florian@openwrt.org>
 *
 * Control the Cobalt Qube/RaQ front LED
 */
#include <linux/init.h>
#include <linux/io.h>
#include <linux/ioport.h>
#include <linux/leds.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/types.h>

#define LED_FRONT_LEFT	0x01
#define LED_FRONT_RIGHT	0x02

static void __iomem *led_port;
static u8 led_value;

static void qube_front_led_set(struct led_classdev *led_cdev,
			       enum led_brightness brightness)
{
	if (brightness)
		led_value = LED_FRONT_LEFT | LED_FRONT_RIGHT;
	else
		led_value = ~(LED_FRONT_LEFT | LED_FRONT_RIGHT);
	writeb(led_value, led_port);
}

static struct led_classdev qube_front_led = {
	.name			= "qube-front",
	.brightness		= LED_FULL,
	.brightness_set		= qube_front_led_set,
	.default_trigger	= "ide-disk",
};

static int __devinit cobalt_qube_led_probe(struct platform_device *pdev)
{
	struct resource *res;
	int retval;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res)
		return -EBUSY;

	led_port = ioremap(res->start, res->end - res->start + 1);
	if (!led_port)
		return -ENOMEM;

	led_value = LED_FRONT_LEFT | LED_FRONT_RIGHT;
	writeb(led_value, led_port);

	retval = led_classdev_register(&pdev->dev, &qube_front_led);
	if (retval)
		goto err_iounmap;

	return 0;

err_iounmap:
	iounmap(led_port);
	led_port = NULL;

	return retval;
}

static int __devexit cobalt_qube_led_remove(struct platform_device *pdev)
{
	led_classdev_unregister(&qube_front_led);

	if (led_port) {
		iounmap(led_port);
		led_port = NULL;
	}

	return 0;
}

/* work with hotplug and coldplug */
MODULE_ALIAS("platform:cobalt-qube-leds");

static struct platform_driver cobalt_qube_led_driver = {
	.probe	= cobalt_qube_led_probe,
	.remove	= __devexit_p(cobalt_qube_led_remove),
	.driver	= {
		.name	= "cobalt-qube-leds",
		.owner	= THIS_MODULE,
	},
};

static int __init cobalt_qube_led_init(void)
{
	return platform_driver_register(&cobalt_qube_led_driver);
}

static void __exit cobalt_qube_led_exit(void)
{
	platform_driver_unregister(&cobalt_qube_led_driver);
}

module_init(cobalt_qube_led_init);
module_exit(cobalt_qube_led_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Front LED support for Cobalt Server");
MODULE_AUTHOR("Florian Fainelli <florian@openwrt.org>");
