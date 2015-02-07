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
 *  i2c-versatile.c
 *
 *  Copyright (C) 2006 ARM Ltd.
 *  written by Russell King, Deep Blue Solutions Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/i2c-algo-bit.h>
#include <linux/init.h>
#include <linux/platform_device.h>

#include <asm/io.h>

#define I2C_CONTROL	0x00
#define I2C_CONTROLS	0x00
#define I2C_CONTROLC	0x04
#define SCL		(1 << 0)
#define SDA		(1 << 1)

struct i2c_versatile {
	struct i2c_adapter	 adap;
	struct i2c_algo_bit_data algo;
	void __iomem		 *base;
};

static void i2c_versatile_setsda(void *data, int state)
{
	struct i2c_versatile *i2c = data;

	writel(SDA, i2c->base + (state ? I2C_CONTROLS : I2C_CONTROLC));
}

static void i2c_versatile_setscl(void *data, int state)
{
	struct i2c_versatile *i2c = data;

	writel(SCL, i2c->base + (state ? I2C_CONTROLS : I2C_CONTROLC));
}

static int i2c_versatile_getsda(void *data)
{
	struct i2c_versatile *i2c = data;
	return !!(readl(i2c->base + I2C_CONTROL) & SDA);
}

static int i2c_versatile_getscl(void *data)
{
	struct i2c_versatile *i2c = data;
	return !!(readl(i2c->base + I2C_CONTROL) & SCL);
}

static struct i2c_algo_bit_data i2c_versatile_algo = {
	.setsda	= i2c_versatile_setsda,
	.setscl = i2c_versatile_setscl,
	.getsda	= i2c_versatile_getsda,
	.getscl = i2c_versatile_getscl,
	.udelay	= 30,
	.timeout = HZ,
};

static int i2c_versatile_probe(struct platform_device *dev)
{
	struct i2c_versatile *i2c;
	struct resource *r;
	int ret;

	r = platform_get_resource(dev, IORESOURCE_MEM, 0);
	if (!r) {
		ret = -EINVAL;
		goto err_out;
	}

	if (!request_mem_region(r->start, r->end - r->start + 1, "versatile-i2c")) {
		ret = -EBUSY;
		goto err_out;
	}

	i2c = kzalloc(sizeof(struct i2c_versatile), GFP_KERNEL);
	if (!i2c) {
		ret = -ENOMEM;
		goto err_release;
	}

	i2c->base = ioremap(r->start, r->end - r->start + 1);
	if (!i2c->base) {
		ret = -ENOMEM;
		goto err_free;
	}

	writel(SCL | SDA, i2c->base + I2C_CONTROLS);

	i2c->adap.owner = THIS_MODULE;
	strlcpy(i2c->adap.name, "Versatile I2C adapter", sizeof(i2c->adap.name));
	i2c->adap.algo_data = &i2c->algo;
	i2c->adap.dev.parent = &dev->dev;
	i2c->algo = i2c_versatile_algo;
	i2c->algo.data = i2c;

	ret = i2c_bit_add_bus(&i2c->adap);
	if (ret >= 0) {
		platform_set_drvdata(dev, i2c);
		return 0;
	}

	iounmap(i2c->base);
 err_free:
	kfree(i2c);
 err_release:
	release_mem_region(r->start, r->end - r->start + 1);
 err_out:
	return ret;
}

static int i2c_versatile_remove(struct platform_device *dev)
{
	struct i2c_versatile *i2c = platform_get_drvdata(dev);

	platform_set_drvdata(dev, NULL);

	i2c_del_adapter(&i2c->adap);
	return 0;
}

static struct platform_driver i2c_versatile_driver = {
	.probe		= i2c_versatile_probe,
	.remove		= i2c_versatile_remove,
	.driver		= {
		.name	= "versatile-i2c",
		.owner	= THIS_MODULE,
	},
};

static int __init i2c_versatile_init(void)
{
	return platform_driver_register(&i2c_versatile_driver);
}

static void __exit i2c_versatile_exit(void)
{
	platform_driver_unregister(&i2c_versatile_driver);
}

module_init(i2c_versatile_init);
module_exit(i2c_versatile_exit);

MODULE_DESCRIPTION("ARM Versatile I2C bus driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:versatile-i2c");
