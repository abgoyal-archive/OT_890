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
 * w1-gpio - GPIO w1 bus master driver
 *
 * Copyright (C) 2007 Ville Syrjala <syrjala@sci.fi>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/w1-gpio.h>

#include "../w1.h"
#include "../w1_int.h"

#include <asm/gpio.h>

static void w1_gpio_write_bit_dir(void *data, u8 bit)
{
	struct w1_gpio_platform_data *pdata = data;

	if (bit)
		gpio_direction_input(pdata->pin);
	else
		gpio_direction_output(pdata->pin, 0);
}

static void w1_gpio_write_bit_val(void *data, u8 bit)
{
	struct w1_gpio_platform_data *pdata = data;

	gpio_set_value(pdata->pin, bit);
}

static u8 w1_gpio_read_bit(void *data)
{
	struct w1_gpio_platform_data *pdata = data;

	return gpio_get_value(pdata->pin) ? 1 : 0;
}

static int __init w1_gpio_probe(struct platform_device *pdev)
{
	struct w1_bus_master *master;
	struct w1_gpio_platform_data *pdata = pdev->dev.platform_data;
	int err;

	if (!pdata)
		return -ENXIO;

	master = kzalloc(sizeof(struct w1_bus_master), GFP_KERNEL);
	if (!master)
		return -ENOMEM;

	err = gpio_request(pdata->pin, "w1");
	if (err)
		goto free_master;

	master->data = pdata;
	master->read_bit = w1_gpio_read_bit;

	if (pdata->is_open_drain) {
		gpio_direction_output(pdata->pin, 1);
		master->write_bit = w1_gpio_write_bit_val;
	} else {
		gpio_direction_input(pdata->pin);
		master->write_bit = w1_gpio_write_bit_dir;
	}

	err = w1_add_master_device(master);
	if (err)
		goto free_gpio;

	platform_set_drvdata(pdev, master);

	return 0;

 free_gpio:
	gpio_free(pdata->pin);
 free_master:
	kfree(master);

	return err;
}

static int __exit w1_gpio_remove(struct platform_device *pdev)
{
	struct w1_bus_master *master = platform_get_drvdata(pdev);
	struct w1_gpio_platform_data *pdata = pdev->dev.platform_data;

	w1_remove_master_device(master);
	gpio_free(pdata->pin);
	kfree(master);

	return 0;
}

static struct platform_driver w1_gpio_driver = {
	.driver = {
		.name	= "w1-gpio",
		.owner	= THIS_MODULE,
	},
	.remove	= __exit_p(w1_gpio_remove),
};

static int __init w1_gpio_init(void)
{
	return platform_driver_probe(&w1_gpio_driver, w1_gpio_probe);
}

static void __exit w1_gpio_exit(void)
{
	platform_driver_unregister(&w1_gpio_driver);
}

module_init(w1_gpio_init);
module_exit(w1_gpio_exit);

MODULE_DESCRIPTION("GPIO w1 bus master driver");
MODULE_AUTHOR("Ville Syrjala <syrjala@sci.fi>");
MODULE_LICENSE("GPL");
