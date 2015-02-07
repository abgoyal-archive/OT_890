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
 * drivers/i2c/busses/i2c-ixp2000.c
 *
 * I2C adapter for IXP2000 systems using GPIOs for I2C bus
 *
 * Author: Deepak Saxena <dsaxena@plexity.net>
 * Based on IXDP2400 code by: Naeem M. Afzal <naeem.m.afzal@intel.com>
 * Made generic by: Jeff Daly <jeffrey.daly@intel.com>
 *
 * Copyright (c) 2003-2004 MontaVista Software Inc.
 *
 * This file is licensed under  the terms of the GNU General Public 
 * License version 2. This program is licensed "as is" without any 
 * warranty of any kind, whether express or implied.
 *
 * From Jeff Daly:
 *
 * I2C adapter driver for Intel IXDP2xxx platforms. This should work for any
 * IXP2000 platform if it uses the HW GPIO in the same manner.  Basically, 
 * SDA and SCL GPIOs have external pullups.  Setting the respective GPIO to 
 * an input will make the signal a '1' via the pullup.  Setting them to 
 * outputs will pull them down. 
 *
 * The GPIOs are open drain signals and are used as configuration strap inputs
 * during power-up so there's generally a buffer on the board that needs to be 
 * 'enabled' to drive the GPIOs.
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/i2c-algo-bit.h>

#include <mach/hardware.h>	/* Pick up IXP2000-specific bits */
#include <mach/gpio.h>

static inline int ixp2000_scl_pin(void *data)
{
	return ((struct ixp2000_i2c_pins*)data)->scl_pin;
}

static inline int ixp2000_sda_pin(void *data)
{
	return ((struct ixp2000_i2c_pins*)data)->sda_pin;
}


static void ixp2000_bit_setscl(void *data, int val)
{
	int i = 5000;

	if (val) {
		gpio_line_config(ixp2000_scl_pin(data), GPIO_IN);
		while(!gpio_line_get(ixp2000_scl_pin(data)) && i--);
	} else {
		gpio_line_config(ixp2000_scl_pin(data), GPIO_OUT);
	}
}

static void ixp2000_bit_setsda(void *data, int val)
{
	if (val) {
		gpio_line_config(ixp2000_sda_pin(data), GPIO_IN);
	} else {
		gpio_line_config(ixp2000_sda_pin(data), GPIO_OUT);
	}
}

static int ixp2000_bit_getscl(void *data)
{
	return gpio_line_get(ixp2000_scl_pin(data));
}

static int ixp2000_bit_getsda(void *data)
{
	return gpio_line_get(ixp2000_sda_pin(data));
}

struct ixp2000_i2c_data {
	struct ixp2000_i2c_pins *gpio_pins;
	struct i2c_adapter adapter;
	struct i2c_algo_bit_data algo_data;
};

static int ixp2000_i2c_remove(struct platform_device *plat_dev)
{
	struct ixp2000_i2c_data *drv_data = platform_get_drvdata(plat_dev);

	platform_set_drvdata(plat_dev, NULL);

	i2c_del_adapter(&drv_data->adapter);

	kfree(drv_data);

	return 0;
}

static int ixp2000_i2c_probe(struct platform_device *plat_dev)
{
	int err;
	struct ixp2000_i2c_pins *gpio = plat_dev->dev.platform_data;
	struct ixp2000_i2c_data *drv_data = 
		kzalloc(sizeof(struct ixp2000_i2c_data), GFP_KERNEL);

	if (!drv_data)
		return -ENOMEM;
	drv_data->gpio_pins = gpio;

	drv_data->algo_data.data = gpio;
	drv_data->algo_data.setsda = ixp2000_bit_setsda;
	drv_data->algo_data.setscl = ixp2000_bit_setscl;
	drv_data->algo_data.getsda = ixp2000_bit_getsda;
	drv_data->algo_data.getscl = ixp2000_bit_getscl;
	drv_data->algo_data.udelay = 6;
	drv_data->algo_data.timeout = HZ;

	strlcpy(drv_data->adapter.name, plat_dev->dev.driver->name,
		sizeof(drv_data->adapter.name));
	drv_data->adapter.algo_data = &drv_data->algo_data,

	drv_data->adapter.dev.parent = &plat_dev->dev;

	gpio_line_config(gpio->sda_pin, GPIO_IN);
	gpio_line_config(gpio->scl_pin, GPIO_IN);
	gpio_line_set(gpio->scl_pin, 0);
	gpio_line_set(gpio->sda_pin, 0);

	if ((err = i2c_bit_add_bus(&drv_data->adapter)) != 0) {
		dev_err(&plat_dev->dev, "Could not install, error %d\n", err);
		kfree(drv_data);
		return err;
	} 

	platform_set_drvdata(plat_dev, drv_data);

	return 0;
}

static struct platform_driver ixp2000_i2c_driver = {
	.probe		= ixp2000_i2c_probe,
	.remove		= ixp2000_i2c_remove,
	.driver		= {
		.name	= "IXP2000-I2C",
		.owner	= THIS_MODULE,
	},
};

static int __init ixp2000_i2c_init(void)
{
	return platform_driver_register(&ixp2000_i2c_driver);
}

static void __exit ixp2000_i2c_exit(void)
{
	platform_driver_unregister(&ixp2000_i2c_driver);
}

module_init(ixp2000_i2c_init);
module_exit(ixp2000_i2c_exit);

MODULE_AUTHOR ("Deepak Saxena <dsaxena@plexity.net>");
MODULE_DESCRIPTION("IXP2000 GPIO-based I2C bus driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:IXP2000-I2C");

