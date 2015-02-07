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
 * wm8350-i2c.c  --  Generic I2C driver for Wolfson WM8350 PMIC
 *
 * Copyright 2007, 2008 Wolfson Microelectronics PLC.
 *
 * Author: Liam Girdwood
 *         linux@wolfsonmicro.com
 *
 *  This program is free software; you can redistribute  it and/or modify it
 *  under  the terms of  the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the  License, or (at your
 *  option) any later version.
 *
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/i2c.h>
#include <linux/platform_device.h>
#include <linux/mfd/wm8350/core.h>

static int wm8350_i2c_read_device(struct wm8350 *wm8350, char reg,
				  int bytes, void *dest)
{
	int ret;

	ret = i2c_master_send(wm8350->i2c_client, &reg, 1);
	if (ret < 0)
		return ret;
	ret = i2c_master_recv(wm8350->i2c_client, dest, bytes);
	if (ret < 0)
		return ret;
	if (ret != bytes)
		return -EIO;
	return 0;
}

static int wm8350_i2c_write_device(struct wm8350 *wm8350, char reg,
				   int bytes, void *src)
{
	/* we add 1 byte for device register */
	u8 msg[(WM8350_MAX_REGISTER << 1) + 1];
	int ret;

	if (bytes > ((WM8350_MAX_REGISTER << 1) + 1))
		return -EINVAL;

	msg[0] = reg;
	memcpy(&msg[1], src, bytes);
	ret = i2c_master_send(wm8350->i2c_client, msg, bytes + 1);
	if (ret < 0)
		return ret;
	if (ret != bytes + 1)
		return -EIO;
	return 0;
}

static int wm8350_i2c_probe(struct i2c_client *i2c,
			    const struct i2c_device_id *id)
{
	struct wm8350 *wm8350;
	int ret = 0;

	wm8350 = kzalloc(sizeof(struct wm8350), GFP_KERNEL);
	if (wm8350 == NULL) {
		kfree(i2c);
		return -ENOMEM;
	}

	i2c_set_clientdata(i2c, wm8350);
	wm8350->dev = &i2c->dev;
	wm8350->i2c_client = i2c;
	wm8350->read_dev = wm8350_i2c_read_device;
	wm8350->write_dev = wm8350_i2c_write_device;

	ret = wm8350_device_init(wm8350, i2c->irq, i2c->dev.platform_data);
	if (ret < 0)
		goto err;

	return ret;

err:
	kfree(wm8350);
	return ret;
}

static int wm8350_i2c_remove(struct i2c_client *i2c)
{
	struct wm8350 *wm8350 = i2c_get_clientdata(i2c);

	wm8350_device_exit(wm8350);
	kfree(wm8350);

	return 0;
}

static const struct i2c_device_id wm8350_i2c_id[] = {
       { "wm8350", 0 },
       { "wm8351", 0 },
       { "wm8352", 0 },
       { }
};
MODULE_DEVICE_TABLE(i2c, wm8350_i2c_id);


static struct i2c_driver wm8350_i2c_driver = {
	.driver = {
		   .name = "wm8350",
		   .owner = THIS_MODULE,
	},
	.probe = wm8350_i2c_probe,
	.remove = wm8350_i2c_remove,
	.id_table = wm8350_i2c_id,
};

static int __init wm8350_i2c_init(void)
{
	return i2c_add_driver(&wm8350_i2c_driver);
}
/* init early so consumer devices can complete system boot */
subsys_initcall(wm8350_i2c_init);

static void __exit wm8350_i2c_exit(void)
{
	i2c_del_driver(&wm8350_i2c_driver);
}
module_exit(wm8350_i2c_exit);

MODULE_DESCRIPTION("I2C support for the WM8350 AudioPlus PMIC");
MODULE_LICENSE("GPL");
