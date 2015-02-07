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
 * drivers/leds/leds-h1940.c
 * Copyright (c) Arnaud Patard <arnaud.patard@rtp-net.org>
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file COPYING in the main directory of this archive for
 * more details.
 *
 * H1940 leds driver
 *
 */

#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/string.h>
#include <linux/ctype.h>
#include <linux/leds.h>
#include <mach/regs-gpio.h>
#include <mach/hardware.h>
#include <mach/h1940-latch.h>

/*
 * Green led.
 */
static void h1940_greenled_set(struct led_classdev *led_dev,
			       enum led_brightness value)
{
	switch (value) {
	case LED_HALF:
		h1940_latch_control(0, H1940_LATCH_LED_FLASH);
		s3c2410_gpio_setpin(S3C2410_GPA7, 1);
		break;
	case LED_FULL:
		h1940_latch_control(0, H1940_LATCH_LED_GREEN);
		s3c2410_gpio_setpin(S3C2410_GPA7, 1);
		break;
	default:
	case LED_OFF:
		h1940_latch_control(H1940_LATCH_LED_FLASH, 0);
		h1940_latch_control(H1940_LATCH_LED_GREEN, 0);
		s3c2410_gpio_setpin(S3C2410_GPA7, 0);
		break;
	}
}

static struct led_classdev h1940_greenled = {
	.name			= "h1940:green",
	.brightness_set		= h1940_greenled_set,
	.default_trigger	= "h1940-charger",
};

/*
 * Red led.
 */
static void h1940_redled_set(struct led_classdev *led_dev,
			     enum led_brightness value)
{
	switch (value) {
	case LED_HALF:
		h1940_latch_control(0, H1940_LATCH_LED_FLASH);
		s3c2410_gpio_setpin(S3C2410_GPA1, 1);
		break;
	case LED_FULL:
		h1940_latch_control(0, H1940_LATCH_LED_RED);
		s3c2410_gpio_setpin(S3C2410_GPA1, 1);
		break;
	default:
	case LED_OFF:
		h1940_latch_control(H1940_LATCH_LED_FLASH, 0);
		h1940_latch_control(H1940_LATCH_LED_RED, 0);
		s3c2410_gpio_setpin(S3C2410_GPA1, 0);
		break;
	}
}

static struct led_classdev h1940_redled = {
	.name			= "h1940:red",
	.brightness_set		= h1940_redled_set,
	.default_trigger	= "h1940-charger",
};

/*
 * Blue led.
 * (it can only be blue flashing led)
 */
static void h1940_blueled_set(struct led_classdev *led_dev,
			      enum led_brightness value)
{
	if (value) {
		/* flashing Blue */
		h1940_latch_control(0, H1940_LATCH_LED_FLASH);
		s3c2410_gpio_setpin(S3C2410_GPA3, 1);
	} else {
		h1940_latch_control(H1940_LATCH_LED_FLASH, 0);
		s3c2410_gpio_setpin(S3C2410_GPA3, 0);
	}

}

static struct led_classdev h1940_blueled = {
	.name			= "h1940:blue",
	.brightness_set		= h1940_blueled_set,
	.default_trigger	= "h1940-bluetooth",
};

static int __init h1940leds_probe(struct platform_device *pdev)
{
	int ret;

	ret = led_classdev_register(&pdev->dev, &h1940_greenled);
	if (ret)
		goto err_green;

	ret = led_classdev_register(&pdev->dev, &h1940_redled);
	if (ret)
		goto err_red;

	ret = led_classdev_register(&pdev->dev, &h1940_blueled);
	if (ret)
		goto err_blue;

	return 0;

err_blue:
	led_classdev_unregister(&h1940_redled);
err_red:
	led_classdev_unregister(&h1940_greenled);
err_green:
	return ret;
}

static int h1940leds_remove(struct platform_device *pdev)
{
	led_classdev_unregister(&h1940_greenled);
	led_classdev_unregister(&h1940_redled);
	led_classdev_unregister(&h1940_blueled);
	return 0;
}


static struct platform_driver h1940leds_driver = {
	.driver		= {
		.name	= "h1940-leds",
		.owner	= THIS_MODULE,
	},
	.probe		= h1940leds_probe,
	.remove		= h1940leds_remove,
};


static int __init h1940leds_init(void)
{
	return platform_driver_register(&h1940leds_driver);
}

static void __exit h1940leds_exit(void)
{
	platform_driver_unregister(&h1940leds_driver);
}

module_init(h1940leds_init);
module_exit(h1940leds_exit);

MODULE_AUTHOR("Arnaud Patard <arnaud.patard@rtp-net.org>");
MODULE_DESCRIPTION("LED driver for the iPAQ H1940");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:h1940-leds");
