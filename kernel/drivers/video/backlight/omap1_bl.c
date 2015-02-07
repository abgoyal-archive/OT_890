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
 * Backlight driver for OMAP based boards.
 *
 * Copyright (c) 2006 Andrzej Zaborowski  <balrog@zabor.org>
 *
 * This package is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This package is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this package; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/fb.h>
#include <linux/backlight.h>

#include <mach/hardware.h>
#include <mach/board.h>
#include <mach/mux.h>

#define OMAPBL_MAX_INTENSITY		0xff

struct omap_backlight {
	int powermode;
	int current_intensity;

	struct device *dev;
	struct omap_backlight_config *pdata;
};

static void inline omapbl_send_intensity(int intensity)
{
	omap_writeb(intensity, OMAP_PWL_ENABLE);
}

static void inline omapbl_send_enable(int enable)
{
	omap_writeb(enable, OMAP_PWL_CLK_ENABLE);
}

static void omapbl_blank(struct omap_backlight *bl, int mode)
{
	if (bl->pdata->set_power)
		bl->pdata->set_power(bl->dev, mode);

	switch (mode) {
	case FB_BLANK_NORMAL:
	case FB_BLANK_VSYNC_SUSPEND:
	case FB_BLANK_HSYNC_SUSPEND:
	case FB_BLANK_POWERDOWN:
		omapbl_send_intensity(0);
		omapbl_send_enable(0);
		break;

	case FB_BLANK_UNBLANK:
		omapbl_send_intensity(bl->current_intensity);
		omapbl_send_enable(1);
		break;
	}
}

#ifdef CONFIG_PM
static int omapbl_suspend(struct platform_device *pdev, pm_message_t state)
{
	struct backlight_device *dev = platform_get_drvdata(pdev);
	struct omap_backlight *bl = dev_get_drvdata(&dev->dev);

	omapbl_blank(bl, FB_BLANK_POWERDOWN);
	return 0;
}

static int omapbl_resume(struct platform_device *pdev)
{
	struct backlight_device *dev = platform_get_drvdata(pdev);
	struct omap_backlight *bl = dev_get_drvdata(&dev->dev);

	omapbl_blank(bl, bl->powermode);
	return 0;
}
#else
#define omapbl_suspend	NULL
#define omapbl_resume	NULL
#endif

static int omapbl_set_power(struct backlight_device *dev, int state)
{
	struct omap_backlight *bl = dev_get_drvdata(&dev->dev);

	omapbl_blank(bl, state);
	bl->powermode = state;

	return 0;
}

static int omapbl_update_status(struct backlight_device *dev)
{
	struct omap_backlight *bl = dev_get_drvdata(&dev->dev);

	if (bl->current_intensity != dev->props.brightness) {
		if (bl->powermode == FB_BLANK_UNBLANK)
			omapbl_send_intensity(dev->props.brightness);
		bl->current_intensity = dev->props.brightness;
	}

	if (dev->props.fb_blank != bl->powermode)
		omapbl_set_power(dev, dev->props.fb_blank);

	return 0;
}

static int omapbl_get_intensity(struct backlight_device *dev)
{
	struct omap_backlight *bl = dev_get_drvdata(&dev->dev);
	return bl->current_intensity;
}

static struct backlight_ops omapbl_ops = {
	.get_brightness = omapbl_get_intensity,
	.update_status  = omapbl_update_status,
};

static int omapbl_probe(struct platform_device *pdev)
{
	struct backlight_device *dev;
	struct omap_backlight *bl;
	struct omap_backlight_config *pdata = pdev->dev.platform_data;

	if (!pdata)
		return -ENXIO;

	omapbl_ops.check_fb = pdata->check_fb;

	bl = kzalloc(sizeof(struct omap_backlight), GFP_KERNEL);
	if (unlikely(!bl))
		return -ENOMEM;

	dev = backlight_device_register("omap-bl", &pdev->dev, bl, &omapbl_ops);
	if (IS_ERR(dev)) {
		kfree(bl);
		return PTR_ERR(dev);
	}

	bl->powermode = FB_BLANK_POWERDOWN;
	bl->current_intensity = 0;

	bl->pdata = pdata;
	bl->dev = &pdev->dev;

	platform_set_drvdata(pdev, dev);

	omap_cfg_reg(PWL);	/* Conflicts with UART3 */

	dev->props.fb_blank = FB_BLANK_UNBLANK;
	dev->props.max_brightness = OMAPBL_MAX_INTENSITY;
	dev->props.brightness = pdata->default_intensity;
	omapbl_update_status(dev);

	printk(KERN_INFO "OMAP LCD backlight initialised\n");

	return 0;
}

static int omapbl_remove(struct platform_device *pdev)
{
	struct backlight_device *dev = platform_get_drvdata(pdev);
	struct omap_backlight *bl = dev_get_drvdata(&dev->dev);

	backlight_device_unregister(dev);
	kfree(bl);

	return 0;
}

static struct platform_driver omapbl_driver = {
	.probe		= omapbl_probe,
	.remove		= omapbl_remove,
	.suspend	= omapbl_suspend,
	.resume		= omapbl_resume,
	.driver		= {
		.name	= "omap-bl",
	},
};

static int __init omapbl_init(void)
{
	return platform_driver_register(&omapbl_driver);
}

static void __exit omapbl_exit(void)
{
	platform_driver_unregister(&omapbl_driver);
}

module_init(omapbl_init);
module_exit(omapbl_exit);

MODULE_AUTHOR("Andrzej Zaborowski <balrog@zabor.org>");
MODULE_DESCRIPTION("OMAP LCD Backlight driver");
MODULE_LICENSE("GPL");
