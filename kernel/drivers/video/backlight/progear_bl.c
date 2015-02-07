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
 *  Backlight Driver for Frontpath ProGear HX1050+
 *
 *  Copyright (c) 2006 Marcin Juszkiewicz
 *
 *  Based on Progear LCD driver by M Schacht
 *  <mschacht at alumni dot washington dot edu>
 *
 *  Based on Sharp's Corgi Backlight Driver
 *  Based on Backlight Driver for HP Jornada 680
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
#include <linux/pci.h>

#define PMU_LPCR               0xB0
#define SB_MPS1                0x61
#define HW_LEVEL_MAX           0x77
#define HW_LEVEL_MIN           0x4f

static struct pci_dev *pmu_dev = NULL;
static struct pci_dev *sb_dev = NULL;

static int progearbl_set_intensity(struct backlight_device *bd)
{
	int intensity = bd->props.brightness;

	if (bd->props.power != FB_BLANK_UNBLANK)
		intensity = 0;
	if (bd->props.fb_blank != FB_BLANK_UNBLANK)
		intensity = 0;

	pci_write_config_byte(pmu_dev, PMU_LPCR, intensity + HW_LEVEL_MIN);

	return 0;
}

static int progearbl_get_intensity(struct backlight_device *bd)
{
	u8 intensity;
	pci_read_config_byte(pmu_dev, PMU_LPCR, &intensity);

	return intensity - HW_LEVEL_MIN;
}

static struct backlight_ops progearbl_ops = {
	.get_brightness = progearbl_get_intensity,
	.update_status = progearbl_set_intensity,
};

static int progearbl_probe(struct platform_device *pdev)
{
	u8 temp;
	struct backlight_device *progear_backlight_device;

	pmu_dev = pci_get_device(PCI_VENDOR_ID_AL, PCI_DEVICE_ID_AL_M7101, NULL);
	if (!pmu_dev) {
		printk("ALI M7101 PMU not found.\n");
		return -ENODEV;
	}

	sb_dev = pci_get_device(PCI_VENDOR_ID_AL, PCI_DEVICE_ID_AL_M1533, NULL);
	if (!sb_dev) {
		printk("ALI 1533 SB not found.\n");
		pci_dev_put(pmu_dev);
		return -ENODEV;
	}

	/*     Set SB_MPS1 to enable brightness control. */
	pci_read_config_byte(sb_dev, SB_MPS1, &temp);
	pci_write_config_byte(sb_dev, SB_MPS1, temp | 0x20);

	progear_backlight_device = backlight_device_register("progear-bl",
							     &pdev->dev, NULL,
							     &progearbl_ops);
	if (IS_ERR(progear_backlight_device))
		return PTR_ERR(progear_backlight_device);

	platform_set_drvdata(pdev, progear_backlight_device);

	progear_backlight_device->props.power = FB_BLANK_UNBLANK;
	progear_backlight_device->props.brightness = HW_LEVEL_MAX - HW_LEVEL_MIN;
	progear_backlight_device->props.max_brightness = HW_LEVEL_MAX - HW_LEVEL_MIN;
	progearbl_set_intensity(progear_backlight_device);

	return 0;
}

static int progearbl_remove(struct platform_device *pdev)
{
	struct backlight_device *bd = platform_get_drvdata(pdev);
	backlight_device_unregister(bd);

	return 0;
}

static struct platform_driver progearbl_driver = {
	.probe = progearbl_probe,
	.remove = progearbl_remove,
	.driver = {
		   .name = "progear-bl",
		   },
};

static struct platform_device *progearbl_device;

static int __init progearbl_init(void)
{
	int ret = platform_driver_register(&progearbl_driver);

	if (ret)
		return ret;
	progearbl_device = platform_device_register_simple("progear-bl", -1,
								NULL, 0);
	if (IS_ERR(progearbl_device)) {
		platform_driver_unregister(&progearbl_driver);
		return PTR_ERR(progearbl_device);
	}

	return 0;
}

static void __exit progearbl_exit(void)
{
	pci_dev_put(pmu_dev);
	pci_dev_put(sb_dev);

	platform_device_unregister(progearbl_device);
	platform_driver_unregister(&progearbl_driver);
}

module_init(progearbl_init);
module_exit(progearbl_exit);

MODULE_AUTHOR("Marcin Juszkiewicz <linux@hrw.one.pl>");
MODULE_DESCRIPTION("ProGear Backlight Driver");
MODULE_LICENSE("GPL");
