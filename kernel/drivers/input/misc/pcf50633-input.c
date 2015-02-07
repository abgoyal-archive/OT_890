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

/* NXP PCF50633 Input Driver
 *
 * (C) 2006-2008 by Openmoko, Inc.
 * Author: Balaji Rao <balajirrao@openmoko.org>
 * All rights reserved.
 *
 * Broken down from monstrous PCF50633 driver mainly by
 * Harald Welte, Andy Green and Werner Almesberger
 *
 *  This program is free software; you can redistribute  it and/or modify it
 *  under  the terms of  the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the  License, or (at your
 *  option) any later version.
 *
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/input.h>

#include <linux/mfd/pcf50633/core.h>

#define PCF50633_OOCSTAT_ONKEY	0x01
#define PCF50633_REG_OOCSTAT	0x12
#define PCF50633_REG_OOCMODE	0x10

struct pcf50633_input {
	struct pcf50633 *pcf;
	struct input_dev *input_dev;
};

static void
pcf50633_input_irq(int irq, void *data)
{
	struct pcf50633_input *input;
	int onkey_released;

	input = data;

	/* We report only one event depending on the key press status */
	onkey_released = pcf50633_reg_read(input->pcf, PCF50633_REG_OOCSTAT)
						& PCF50633_OOCSTAT_ONKEY;

	if (irq == PCF50633_IRQ_ONKEYF && !onkey_released)
		input_report_key(input->input_dev, KEY_POWER, 1);
	else if (irq == PCF50633_IRQ_ONKEYR && onkey_released)
		input_report_key(input->input_dev, KEY_POWER, 0);

	input_sync(input->input_dev);
}

static int __devinit pcf50633_input_probe(struct platform_device *pdev)
{
	struct pcf50633_input *input;
	struct pcf50633_subdev_pdata *pdata = pdev->dev.platform_data;
	struct input_dev *input_dev;
	int ret;


	input = kzalloc(sizeof(*input), GFP_KERNEL);
	if (!input)
		return -ENOMEM;

	input_dev = input_allocate_device();
	if (!input_dev) {
		kfree(input);
		return -ENOMEM;
	}

	platform_set_drvdata(pdev, input);
	input->pcf = pdata->pcf;
	input->input_dev = input_dev;

	input_dev->name = "PCF50633 PMU events";
	input_dev->id.bustype = BUS_I2C;
	input_dev->evbit[0] = BIT(EV_KEY) | BIT(EV_PWR);
	set_bit(KEY_POWER, input_dev->keybit);

	ret = input_register_device(input_dev);
	if (ret) {
		input_free_device(input_dev);
		kfree(input);
		return ret;
	}
	pcf50633_register_irq(pdata->pcf, PCF50633_IRQ_ONKEYR,
				pcf50633_input_irq, input);
	pcf50633_register_irq(pdata->pcf, PCF50633_IRQ_ONKEYF,
				pcf50633_input_irq, input);

	return 0;
}

static int __devexit pcf50633_input_remove(struct platform_device *pdev)
{
	struct pcf50633_input *input  = platform_get_drvdata(pdev);

	pcf50633_free_irq(input->pcf, PCF50633_IRQ_ONKEYR);
	pcf50633_free_irq(input->pcf, PCF50633_IRQ_ONKEYF);

	input_unregister_device(input->input_dev);
	kfree(input);

	return 0;
}

static struct platform_driver pcf50633_input_driver = {
	.driver = {
		.name = "pcf50633-input",
	},
	.probe = pcf50633_input_probe,
	.remove = __devexit_p(pcf50633_input_remove),
};

static int __init pcf50633_input_init(void)
{
	return platform_driver_register(&pcf50633_input_driver);
}
module_init(pcf50633_input_init);

static void __exit pcf50633_input_exit(void)
{
	platform_driver_unregister(&pcf50633_input_driver);
}
module_exit(pcf50633_input_exit);

MODULE_AUTHOR("Balaji Rao <balajirrao@openmoko.org>");
MODULE_DESCRIPTION("PCF50633 input driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:pcf50633-input");
