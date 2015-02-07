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
 * drivers/w1/slaves/w1_bq27000.c
 *
 * Copyright (C) 2007 Texas Instruments, Inc.
 *
 * This file is licensed under the terms of the GNU General Public License
 * version 2. This program is licensed "as is" without any warranty of any
 * kind, whether express or implied.
 *
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/types.h>
#include <linux/platform_device.h>
#include <linux/mutex.h>

#include "../w1.h"
#include "../w1_int.h"
#include "../w1_family.h"

#define HDQ_CMD_READ	(0)
#define HDQ_CMD_WRITE	(1<<7)

static int F_ID;

void w1_bq27000_write(struct device *dev, u8 buf, u8 reg)
{
	struct w1_slave *sl = container_of(dev, struct w1_slave, dev);

	if (!dev) {
		pr_info("Could not obtain slave dev ptr\n");
		return;
	}

	w1_write_8(sl->master, HDQ_CMD_WRITE | reg);
	w1_write_8(sl->master, buf);
}
EXPORT_SYMBOL(w1_bq27000_write);

int w1_bq27000_read(struct device *dev, u8 reg)
{
	u8 val;
	struct w1_slave *sl = container_of(dev, struct w1_slave, dev);

	if (!dev)
		return 0;

	w1_write_8(sl->master, HDQ_CMD_READ | reg);
	val = w1_read_8(sl->master);

	return val;
}
EXPORT_SYMBOL(w1_bq27000_read);

static int w1_bq27000_add_slave(struct w1_slave *sl)
{
	int ret;
	int id = 1;
	struct platform_device *pdev;

	pdev = platform_device_alloc("bq27000-battery", id);
	if (!pdev) {
		ret = -ENOMEM;
		return ret;
	}
	pdev->dev.parent = &sl->dev;

	ret = platform_device_add(pdev);
	if (ret)
		goto pdev_add_failed;

	dev_set_drvdata(&sl->dev, pdev);

	goto success;

pdev_add_failed:
	platform_device_unregister(pdev);
success:
	return ret;
}

static void w1_bq27000_remove_slave(struct w1_slave *sl)
{
	struct platform_device *pdev = dev_get_drvdata(&sl->dev);

	platform_device_unregister(pdev);
}

static struct w1_family_ops w1_bq27000_fops = {
	.add_slave	= w1_bq27000_add_slave,
	.remove_slave	= w1_bq27000_remove_slave,
};

static struct w1_family w1_bq27000_family = {
	.fid = 1,
	.fops = &w1_bq27000_fops,
};

static int __init w1_bq27000_init(void)
{
	if (F_ID)
		w1_bq27000_family.fid = F_ID;

	return w1_register_family(&w1_bq27000_family);
}

static void __exit w1_bq27000_exit(void)
{
	w1_unregister_family(&w1_bq27000_family);
}


module_init(w1_bq27000_init);
module_exit(w1_bq27000_exit);

module_param(F_ID, int, S_IRUSR);
MODULE_PARM_DESC(F_ID, "1-wire slave FID for BQ device");

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Texas Instruments Ltd");
MODULE_DESCRIPTION("HDQ/1-wire slave driver bq27000 battery monitor chip");
