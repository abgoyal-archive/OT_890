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
 * fixed.c
 *
 * Copyright 2008 Wolfson Microelectronics PLC.
 *
 * Author: Mark Brown <broonie@opensource.wolfsonmicro.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This is useful for systems with mixed controllable and
 * non-controllable regulators, as well as for allowing testing on
 * systems with no controllable regulators.
 */

#include <linux/err.h>
#include <linux/mutex.h>
#include <linux/platform_device.h>
#include <linux/regulator/driver.h>
#include <linux/regulator/fixed.h>

struct fixed_voltage_data {
	struct regulator_desc desc;
	struct regulator_dev *dev;
	int microvolts;
};

static int fixed_voltage_is_enabled(struct regulator_dev *dev)
{
	return 1;
}

static int fixed_voltage_enable(struct regulator_dev *dev)
{
	return 0;
}

static int fixed_voltage_get_voltage(struct regulator_dev *dev)
{
	struct fixed_voltage_data *data = rdev_get_drvdata(dev);

	return data->microvolts;
}

static struct regulator_ops fixed_voltage_ops = {
	.is_enabled = fixed_voltage_is_enabled,
	.enable = fixed_voltage_enable,
	.get_voltage = fixed_voltage_get_voltage,
};

static int regulator_fixed_voltage_probe(struct platform_device *pdev)
{
	struct fixed_voltage_config *config = pdev->dev.platform_data;
	struct fixed_voltage_data *drvdata;
	int ret;

	drvdata = kzalloc(sizeof(struct fixed_voltage_data), GFP_KERNEL);
	if (drvdata == NULL) {
		ret = -ENOMEM;
		goto err;
	}

	drvdata->desc.name = kstrdup(config->supply_name, GFP_KERNEL);
	if (drvdata->desc.name == NULL) {
		ret = -ENOMEM;
		goto err;
	}
	drvdata->desc.type = REGULATOR_VOLTAGE;
	drvdata->desc.owner = THIS_MODULE;
	drvdata->desc.ops = &fixed_voltage_ops,

	drvdata->microvolts = config->microvolts;

	drvdata->dev = regulator_register(&drvdata->desc, drvdata);
	if (IS_ERR(drvdata->dev)) {
		ret = PTR_ERR(drvdata->dev);
		goto err_name;
	}

	platform_set_drvdata(pdev, drvdata);

	dev_dbg(&pdev->dev, "%s supplying %duV\n", drvdata->desc.name,
		drvdata->microvolts);

	return 0;

err_name:
	kfree(drvdata->desc.name);
err:
	kfree(drvdata);
	return ret;
}

static int regulator_fixed_voltage_remove(struct platform_device *pdev)
{
	struct fixed_voltage_data *drvdata = platform_get_drvdata(pdev);

	regulator_unregister(drvdata->dev);
	kfree(drvdata->desc.name);
	kfree(drvdata);

	return 0;
}

static struct platform_driver regulator_fixed_voltage_driver = {
	.probe		= regulator_fixed_voltage_probe,
	.remove		= regulator_fixed_voltage_remove,
	.driver		= {
		.name		= "reg-fixed-voltage",
	},
};

static int __init regulator_fixed_voltage_init(void)
{
	return platform_driver_register(&regulator_fixed_voltage_driver);
}
module_init(regulator_fixed_voltage_init);

static void __exit regulator_fixed_voltage_exit(void)
{
	platform_driver_unregister(&regulator_fixed_voltage_driver);
}
module_exit(regulator_fixed_voltage_exit);

MODULE_AUTHOR("Mark Brown <broonie@opensource.wolfsonmicro.com>");
MODULE_DESCRIPTION("Fixed voltage regulator");
MODULE_LICENSE("GPL");
