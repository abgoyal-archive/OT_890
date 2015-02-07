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
 *  Backlight Driver for the KB3886 Backlight
 *
 *  Copyright (c) 2007-2008 Claudio Nieder
 *
 *  Based on corgi_bl.c by Richard Purdie and kb3886 driver by Robert Woerle
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 *
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/mutex.h>
#include <linux/fb.h>
#include <linux/backlight.h>
#include <linux/delay.h>
#include <linux/dmi.h>

#define KB3886_PARENT 0x64
#define KB3886_IO 0x60
#define KB3886_ADC_DAC_PWM 0xC4
#define KB3886_PWM0_WRITE 0x81
#define KB3886_PWM0_READ 0x41

static DEFINE_MUTEX(bl_mutex);

static void kb3886_bl_set_intensity(int intensity)
{
	mutex_lock(&bl_mutex);
	intensity = intensity&0xff;
	outb(KB3886_ADC_DAC_PWM, KB3886_PARENT);
	msleep(10);
	outb(KB3886_PWM0_WRITE, KB3886_IO);
	msleep(10);
	outb(intensity, KB3886_IO);
	mutex_unlock(&bl_mutex);
}

struct kb3886bl_machinfo {
	int max_intensity;
	int default_intensity;
	int limit_mask;
	void (*set_bl_intensity)(int intensity);
};

static struct kb3886bl_machinfo kb3886_bl_machinfo = {
	.max_intensity = 0xff,
	.default_intensity = 0xa0,
	.limit_mask = 0x7f,
	.set_bl_intensity = kb3886_bl_set_intensity,
};

static struct platform_device kb3886bl_device = {
	.name		= "kb3886-bl",
	.dev		= {
		.platform_data	= &kb3886_bl_machinfo,
	},
	.id		= -1,
};

static struct platform_device *devices[] __initdata = {
	&kb3886bl_device,
};

/*
 * Back to driver
 */

static int kb3886bl_intensity;
static struct backlight_device *kb3886_backlight_device;
static struct kb3886bl_machinfo *bl_machinfo;

static unsigned long kb3886bl_flags;
#define KB3886BL_SUSPENDED     0x01

static struct dmi_system_id __initdata kb3886bl_device_table[] = {
	{
		.ident = "Sahara Touch-iT",
		.matches = {
			DMI_MATCH(DMI_SYS_VENDOR, "SDV"),
			DMI_MATCH(DMI_PRODUCT_NAME, "iTouch T201"),
		},
	},
	{ }
};

static int kb3886bl_send_intensity(struct backlight_device *bd)
{
	int intensity = bd->props.brightness;

	if (bd->props.power != FB_BLANK_UNBLANK)
		intensity = 0;
	if (bd->props.fb_blank != FB_BLANK_UNBLANK)
		intensity = 0;
	if (kb3886bl_flags & KB3886BL_SUSPENDED)
		intensity = 0;

	bl_machinfo->set_bl_intensity(intensity);

	kb3886bl_intensity = intensity;
	return 0;
}

#ifdef CONFIG_PM
static int kb3886bl_suspend(struct platform_device *pdev, pm_message_t state)
{
	struct backlight_device *bd = platform_get_drvdata(pdev);

	kb3886bl_flags |= KB3886BL_SUSPENDED;
	backlight_update_status(bd);
	return 0;
}

static int kb3886bl_resume(struct platform_device *pdev)
{
	struct backlight_device *bd = platform_get_drvdata(pdev);

	kb3886bl_flags &= ~KB3886BL_SUSPENDED;
	backlight_update_status(bd);
	return 0;
}
#else
#define kb3886bl_suspend	NULL
#define kb3886bl_resume		NULL
#endif

static int kb3886bl_get_intensity(struct backlight_device *bd)
{
	return kb3886bl_intensity;
}

static struct backlight_ops kb3886bl_ops = {
	.get_brightness = kb3886bl_get_intensity,
	.update_status  = kb3886bl_send_intensity,
};

static int kb3886bl_probe(struct platform_device *pdev)
{
	struct kb3886bl_machinfo *machinfo = pdev->dev.platform_data;

	bl_machinfo = machinfo;
	if (!machinfo->limit_mask)
		machinfo->limit_mask = -1;

	kb3886_backlight_device = backlight_device_register("kb3886-bl",
		&pdev->dev, NULL, &kb3886bl_ops);
	if (IS_ERR(kb3886_backlight_device))
		return PTR_ERR(kb3886_backlight_device);

	platform_set_drvdata(pdev, kb3886_backlight_device);

	kb3886_backlight_device->props.max_brightness = machinfo->max_intensity;
	kb3886_backlight_device->props.power = FB_BLANK_UNBLANK;
	kb3886_backlight_device->props.brightness = machinfo->default_intensity;
	backlight_update_status(kb3886_backlight_device);

	return 0;
}

static int kb3886bl_remove(struct platform_device *pdev)
{
	struct backlight_device *bd = platform_get_drvdata(pdev);

	backlight_device_unregister(bd);

	return 0;
}

static struct platform_driver kb3886bl_driver = {
	.probe		= kb3886bl_probe,
	.remove		= kb3886bl_remove,
	.suspend	= kb3886bl_suspend,
	.resume		= kb3886bl_resume,
	.driver		= {
		.name	= "kb3886-bl",
	},
};

static int __init kb3886_init(void)
{
	if (!dmi_check_system(kb3886bl_device_table))
		return -ENODEV;

	platform_add_devices(devices, ARRAY_SIZE(devices));
	return platform_driver_register(&kb3886bl_driver);
}

static void __exit kb3886_exit(void)
{
	platform_driver_unregister(&kb3886bl_driver);
}

module_init(kb3886_init);
module_exit(kb3886_exit);

MODULE_AUTHOR("Claudio Nieder <private@claudio.ch>");
MODULE_DESCRIPTION("Tabletkiosk Sahara Touch-iT Backlight Driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("dmi:*:svnSDV:pniTouchT201:*");
