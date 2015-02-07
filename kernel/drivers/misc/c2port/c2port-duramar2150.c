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
 *  Silicon Labs C2 port Linux support for Eurotech Duramar 2150
 *
 *  Copyright (c) 2008 Rodolfo Giometti <giometti@linux.it>
 *  Copyright (c) 2008 Eurotech S.p.A. <info@eurotech.it>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation
 */

#include <linux/errno.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/io.h>
#include <linux/c2port.h>

#define DATA_PORT	0x325
#define DIR_PORT	0x326
#define    C2D		   (1 << 0)
#define    C2CK		   (1 << 1)

static DEFINE_MUTEX(update_lock);

/*
 * C2 port operations
 */

static void duramar2150_c2port_access(struct c2port_device *dev, int status)
{
	u8 v;

	mutex_lock(&update_lock);

	v = inb(DIR_PORT);

	/* 0 = input, 1 = output */
	if (status)
		outb(v | (C2D | C2CK), DIR_PORT);
	else
		/* When access is "off" is important that both lines are set
		 * as inputs or hi-impedence */
		outb(v & ~(C2D | C2CK), DIR_PORT);

	mutex_unlock(&update_lock);
}

static void duramar2150_c2port_c2d_dir(struct c2port_device *dev, int dir)
{
	u8 v;

	mutex_lock(&update_lock);

	v = inb(DIR_PORT);

	if (dir)
		outb(v & ~C2D, DIR_PORT);
	else
		outb(v | C2D, DIR_PORT);

	mutex_unlock(&update_lock);
}

static int duramar2150_c2port_c2d_get(struct c2port_device *dev)
{
	return inb(DATA_PORT) & C2D;
}

static void duramar2150_c2port_c2d_set(struct c2port_device *dev, int status)
{
	u8 v;

	mutex_lock(&update_lock);

	v = inb(DATA_PORT);

	if (status)
		outb(v | C2D, DATA_PORT);
	else
		outb(v & ~C2D, DATA_PORT);

	mutex_unlock(&update_lock);
}

static void duramar2150_c2port_c2ck_set(struct c2port_device *dev, int status)
{
	u8 v;

	mutex_lock(&update_lock);

	v = inb(DATA_PORT);

	if (status)
		outb(v | C2CK, DATA_PORT);
	else
		outb(v & ~C2CK, DATA_PORT);

	mutex_unlock(&update_lock);
}

static struct c2port_ops duramar2150_c2port_ops = {
	.block_size	= 512,	/* bytes */
	.blocks_num	= 30,	/* total flash size: 15360 bytes */

	.access		= duramar2150_c2port_access,
	.c2d_dir	= duramar2150_c2port_c2d_dir,
	.c2d_get	= duramar2150_c2port_c2d_get,
	.c2d_set	= duramar2150_c2port_c2d_set,
	.c2ck_set	= duramar2150_c2port_c2ck_set,
};

static struct c2port_device *duramar2150_c2port_dev;

/*
 * Module stuff
 */

static int __init duramar2150_c2port_init(void)
{
	struct resource *res;
	int ret = 0;

	res = request_region(0x325, 2, "c2port");
	if (!res)
		return -EBUSY;

	duramar2150_c2port_dev = c2port_device_register("uc",
					&duramar2150_c2port_ops, NULL);
	if (!duramar2150_c2port_dev) {
		ret = -ENODEV;
		goto free_region;
	}

	return 0;

free_region:
	release_region(0x325, 2);
	return ret;
}

static void __exit duramar2150_c2port_exit(void)
{
	/* Setup the GPIOs as input by default (access = 0) */
	duramar2150_c2port_access(duramar2150_c2port_dev, 0);

	c2port_device_unregister(duramar2150_c2port_dev);

	release_region(0x325, 2);
}

module_init(duramar2150_c2port_init);
module_exit(duramar2150_c2port_exit);

MODULE_AUTHOR("Rodolfo Giometti <giometti@linux.it>");
MODULE_DESCRIPTION("Silicon Labs C2 port Linux support for Duramar 2150");
MODULE_LICENSE("GPL");
