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
 *  m68k beeper driver for Linux
 *
 *  Copyright (c) 2002 Richard Zidlicky
 *  Copyright (c) 2002 Vojtech Pavlik
 *  Copyright (c) 1992 Orest Zborowski
 *
 */

/*
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/input.h>
#include <linux/platform_device.h>
#include <asm/machdep.h>
#include <asm/io.h>

MODULE_AUTHOR("Richard Zidlicky <rz@linux-m68k.org>");
MODULE_DESCRIPTION("m68k beeper driver");
MODULE_LICENSE("GPL");

static struct platform_device *m68kspkr_platform_device;

static int m68kspkr_event(struct input_dev *dev, unsigned int type, unsigned int code, int value)
{
	unsigned int count = 0;

	if (type != EV_SND)
		return -1;

	switch (code) {
		case SND_BELL: if (value) value = 1000;
		case SND_TONE: break;
		default: return -1;
	}

	if (value > 20 && value < 32767)
		count = 1193182 / value;

	mach_beep(count, -1);

	return 0;
}

static int __devinit m68kspkr_probe(struct platform_device *dev)
{
	struct input_dev *input_dev;
	int err;

	input_dev = input_allocate_device();
	if (!input_dev)
		return -ENOMEM;

	input_dev->name = "m68k beeper";
	input_dev->phys = "m68k/generic";
	input_dev->id.bustype = BUS_HOST;
	input_dev->id.vendor  = 0x001f;
	input_dev->id.product = 0x0001;
	input_dev->id.version = 0x0100;
	input_dev->dev.parent = &dev->dev;

	input_dev->evbit[0] = BIT_MASK(EV_SND);
	input_dev->sndbit[0] = BIT_MASK(SND_BELL) | BIT_MASK(SND_TONE);
	input_dev->event = m68kspkr_event;

	err = input_register_device(input_dev);
	if (err) {
		input_free_device(input_dev);
		return err;
	}

	platform_set_drvdata(dev, input_dev);

	return 0;
}

static int __devexit m68kspkr_remove(struct platform_device *dev)
{
	struct input_dev *input_dev = platform_get_drvdata(dev);

	input_unregister_device(input_dev);
	platform_set_drvdata(dev, NULL);
	/* turn off the speaker */
	m68kspkr_event(NULL, EV_SND, SND_BELL, 0);

	return 0;
}

static void m68kspkr_shutdown(struct platform_device *dev)
{
	/* turn off the speaker */
	m68kspkr_event(NULL, EV_SND, SND_BELL, 0);
}

static struct platform_driver m68kspkr_platform_driver = {
	.driver		= {
		.name	= "m68kspkr",
		.owner	= THIS_MODULE,
	},
	.probe		= m68kspkr_probe,
	.remove		= __devexit_p(m68kspkr_remove),
	.shutdown	= m68kspkr_shutdown,
};

static int __init m68kspkr_init(void)
{
	int err;

	if (!mach_beep) {
		printk(KERN_INFO "m68kspkr: no lowlevel beep support\n");
		return -ENODEV;
        }

	err = platform_driver_register(&m68kspkr_platform_driver);
	if (err)
		return err;

	m68kspkr_platform_device = platform_device_alloc("m68kspkr", -1);
	if (!m68kspkr_platform_device) {
		err = -ENOMEM;
		goto err_unregister_driver;
	}

	err = platform_device_add(m68kspkr_platform_device);
	if (err)
		goto err_free_device;

	return 0;

 err_free_device:
	platform_device_put(m68kspkr_platform_device);
 err_unregister_driver:
	platform_driver_unregister(&m68kspkr_platform_driver);

	return err;
}

static void __exit m68kspkr_exit(void)
{
	platform_device_unregister(m68kspkr_platform_device);
	platform_driver_unregister(&m68kspkr_platform_driver);
}

module_init(m68kspkr_init);
module_exit(m68kspkr_exit);
