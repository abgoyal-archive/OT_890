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
 * common keywest i2c layer
 *
 * Copyright (c) by Takashi Iwai <tiwai@suse.de>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 */


#include <linux/init.h>
#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <sound/core.h>
#include "pmac.h"

/*
 * we have to keep a static variable here since i2c attach_adapter
 * callback cannot pass a private data.
 */
static struct pmac_keywest *keywest_ctx;


static int keywest_attach_adapter(struct i2c_adapter *adapter);
static int keywest_detach_client(struct i2c_client *client);

struct i2c_driver keywest_driver = {  
	.driver = {
		.name = "PMac Keywest Audio",
	},
	.attach_adapter = &keywest_attach_adapter,
	.detach_client = &keywest_detach_client,
};


#ifndef i2c_device_name
#define i2c_device_name(x)	((x)->name)
#endif

static int keywest_attach_adapter(struct i2c_adapter *adapter)
{
	int err;
	struct i2c_client *new_client;

	if (! keywest_ctx)
		return -EINVAL;

	if (strncmp(i2c_device_name(adapter), "mac-io", 6))
		return 0; /* ignored */

	new_client = kzalloc(sizeof(struct i2c_client), GFP_KERNEL);
	if (! new_client)
		return -ENOMEM;

	new_client->addr = keywest_ctx->addr;
	i2c_set_clientdata(new_client, keywest_ctx);
	new_client->adapter = adapter;
	new_client->driver = &keywest_driver;
	new_client->flags = 0;

	strcpy(i2c_device_name(new_client), keywest_ctx->name);
	keywest_ctx->client = new_client;
	
	/* Tell the i2c layer a new client has arrived */
	if (i2c_attach_client(new_client)) {
		snd_printk(KERN_ERR "tumbler: cannot attach i2c client\n");
		err = -ENODEV;
		goto __err;
	}

	return 0;

 __err:
	kfree(new_client);
	keywest_ctx->client = NULL;
	return err;
}

static int keywest_detach_client(struct i2c_client *client)
{
	if (! keywest_ctx)
		return 0;
	if (client == keywest_ctx->client)
		keywest_ctx->client = NULL;

	i2c_detach_client(client);
	kfree(client);
	return 0;
}

/* exported */
void snd_pmac_keywest_cleanup(struct pmac_keywest *i2c)
{
	if (keywest_ctx && keywest_ctx == i2c) {
		i2c_del_driver(&keywest_driver);
		keywest_ctx = NULL;
	}
}

int __init snd_pmac_tumbler_post_init(void)
{
	int err;
	
	if (!keywest_ctx || !keywest_ctx->client)
		return -ENXIO;

	if ((err = keywest_ctx->init_client(keywest_ctx)) < 0) {
		snd_printk(KERN_ERR "tumbler: %i :cannot initialize the MCS\n", err);
		return err;
	}
	return 0;
}

/* exported */
int __init snd_pmac_keywest_init(struct pmac_keywest *i2c)
{
	int err;

	if (keywest_ctx)
		return -EBUSY;

	keywest_ctx = i2c;

	if ((err = i2c_add_driver(&keywest_driver))) {
		snd_printk(KERN_ERR "cannot register keywest i2c driver\n");
		return err;
	}
	return 0;
}
