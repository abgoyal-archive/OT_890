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

/* drivers/video/backlight/platform_lcd.c
 *
 * Copyright 2008 Simtec Electronics
 *	Ben Dooks <ben@simtec.co.uk>
 *
 * Generic platform-device LCD power control interface.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
*/

#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/fb.h>
#include <linux/backlight.h>
#include <linux/lcd.h>

#include <video/platform_lcd.h>

struct platform_lcd {
	struct device		*us;
	struct lcd_device	*lcd;
	struct plat_lcd_data	*pdata;

	unsigned int		 power;
	unsigned int		 suspended : 1;
};

static inline struct platform_lcd *to_our_lcd(struct lcd_device *lcd)
{
	return lcd_get_data(lcd);
}

static int platform_lcd_get_power(struct lcd_device *lcd)
{
	struct platform_lcd *plcd = to_our_lcd(lcd);

	return plcd->power;
}

static int platform_lcd_set_power(struct lcd_device *lcd, int power)
{
	struct platform_lcd *plcd = to_our_lcd(lcd);
	int lcd_power = 1;

	if (power == FB_BLANK_POWERDOWN || plcd->suspended)
		lcd_power = 0;

	plcd->pdata->set_power(plcd->pdata, lcd_power);
	plcd->power = power;

	return 0;
}

static int platform_lcd_match(struct lcd_device *lcd, struct fb_info *info)
{
	struct platform_lcd *plcd = to_our_lcd(lcd);
	struct plat_lcd_data *pdata = plcd->pdata;

	if (pdata->match_fb)
		return pdata->match_fb(pdata, info);

	return plcd->us->parent == info->device;
}

static struct lcd_ops platform_lcd_ops = {
	.get_power	= platform_lcd_get_power,
	.set_power	= platform_lcd_set_power,
	.check_fb	= platform_lcd_match,
};

static int __devinit platform_lcd_probe(struct platform_device *pdev)
{
	struct plat_lcd_data *pdata;
	struct platform_lcd *plcd;
	struct device *dev = &pdev->dev;
	int err;

	pdata = pdev->dev.platform_data;
	if (!pdata) {
		dev_err(dev, "no platform data supplied\n");
		return -EINVAL;
	}

	plcd = kzalloc(sizeof(struct platform_lcd), GFP_KERNEL);
	if (!plcd) {
		dev_err(dev, "no memory for state\n");
		return -ENOMEM;
	}

	plcd->us = dev;
	plcd->pdata = pdata;
	plcd->lcd = lcd_device_register(dev_name(dev), dev,
					plcd, &platform_lcd_ops);
	if (IS_ERR(plcd->lcd)) {
		dev_err(dev, "cannot register lcd device\n");
		err = PTR_ERR(plcd->lcd);
		goto err_mem;
	}

	platform_set_drvdata(pdev, plcd);
	platform_lcd_set_power(plcd->lcd, FB_BLANK_NORMAL);

	return 0;

 err_mem:
	kfree(plcd);
	return err;
}

static int __devexit platform_lcd_remove(struct platform_device *pdev)
{
	struct platform_lcd *plcd = platform_get_drvdata(pdev);

	lcd_device_unregister(plcd->lcd);
	kfree(plcd);

	return 0;
}

#ifdef CONFIG_PM
static int platform_lcd_suspend(struct platform_device *pdev, pm_message_t st)
{
	struct platform_lcd *plcd = platform_get_drvdata(pdev);

	plcd->suspended = 1;
	platform_lcd_set_power(plcd->lcd, plcd->power);

	return 0;
}

static int platform_lcd_resume(struct platform_device *pdev)
{
	struct platform_lcd *plcd = platform_get_drvdata(pdev);

	plcd->suspended = 0;
	platform_lcd_set_power(plcd->lcd, plcd->power);

	return 0;
}
#else
#define platform_lcd_suspend NULL
#define platform_lcd_resume NULL
#endif

static struct platform_driver platform_lcd_driver = {
	.driver		= {
		.name	= "platform-lcd",
		.owner	= THIS_MODULE,
	},
	.probe		= platform_lcd_probe,
	.remove		= __devexit_p(platform_lcd_remove),
	.suspend        = platform_lcd_suspend,
	.resume         = platform_lcd_resume,
};

static int __init platform_lcd_init(void)
{
	return platform_driver_register(&platform_lcd_driver);
}

static void __exit platform_lcd_cleanup(void)
{
	platform_driver_unregister(&platform_lcd_driver);
}

module_init(platform_lcd_init);
module_exit(platform_lcd_cleanup);

MODULE_AUTHOR("Ben Dooks <ben-linux@fluff.org>");
MODULE_LICENSE("GPL v2");
MODULE_ALIAS("platform:platform-lcd");
