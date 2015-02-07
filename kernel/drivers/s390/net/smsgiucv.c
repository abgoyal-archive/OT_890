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
 * IUCV special message driver
 *
 * Copyright 2003 IBM Deutschland Entwicklung GmbH, IBM Corporation
 * Author(s): Martin Schwidefsky (schwidefsky@de.ibm.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/device.h>
#include <net/iucv/iucv.h>
#include <asm/cpcmd.h>
#include <asm/ebcdic.h>
#include "smsgiucv.h"

struct smsg_callback {
	struct list_head list;
	char *prefix;
	int len;
	void (*callback)(char *from, char *str);
};

MODULE_AUTHOR
   ("(C) 2003 IBM Corporation by Martin Schwidefsky (schwidefsky@de.ibm.com)");
MODULE_DESCRIPTION ("Linux for S/390 IUCV special message driver");

static struct iucv_path *smsg_path;

static DEFINE_SPINLOCK(smsg_list_lock);
static LIST_HEAD(smsg_list);

static int smsg_path_pending(struct iucv_path *, u8 ipvmid[8], u8 ipuser[16]);
static void smsg_message_pending(struct iucv_path *, struct iucv_message *);

static struct iucv_handler smsg_handler = {
	.path_pending	 = smsg_path_pending,
	.message_pending = smsg_message_pending,
};

static int smsg_path_pending(struct iucv_path *path, u8 ipvmid[8],
			     u8 ipuser[16])
{
	if (strncmp(ipvmid, "*MSG    ", sizeof(ipvmid)) != 0)
		return -EINVAL;
	/* Path pending from *MSG. */
	return iucv_path_accept(path, &smsg_handler, "SMSGIUCV        ", NULL);
}

static void smsg_message_pending(struct iucv_path *path,
				 struct iucv_message *msg)
{
	struct smsg_callback *cb;
	unsigned char *buffer;
	unsigned char sender[9];
	int rc, i;

	buffer = kmalloc(msg->length + 1, GFP_ATOMIC | GFP_DMA);
	if (!buffer) {
		iucv_message_reject(path, msg);
		return;
	}
	rc = iucv_message_receive(path, msg, 0, buffer, msg->length, NULL);
	if (rc == 0) {
		buffer[msg->length] = 0;
		EBCASC(buffer, msg->length);
		memcpy(sender, buffer, 8);
		sender[8] = 0;
		/* Remove trailing whitespace from the sender name. */
		for (i = 7; i >= 0; i--) {
			if (sender[i] != ' ' && sender[i] != '\t')
				break;
			sender[i] = 0;
		}
		spin_lock(&smsg_list_lock);
		list_for_each_entry(cb, &smsg_list, list)
			if (strncmp(buffer + 8, cb->prefix, cb->len) == 0) {
				cb->callback(sender, buffer + 8);
				break;
			}
		spin_unlock(&smsg_list_lock);
	}
	kfree(buffer);
}

int smsg_register_callback(char *prefix,
			   void (*callback)(char *from, char *str))
{
	struct smsg_callback *cb;

	cb = kmalloc(sizeof(struct smsg_callback), GFP_KERNEL);
	if (!cb)
		return -ENOMEM;
	cb->prefix = prefix;
	cb->len = strlen(prefix);
	cb->callback = callback;
	spin_lock_bh(&smsg_list_lock);
	list_add_tail(&cb->list, &smsg_list);
	spin_unlock_bh(&smsg_list_lock);
	return 0;
}

void smsg_unregister_callback(char *prefix,
			      void (*callback)(char *from, char *str))
{
	struct smsg_callback *cb, *tmp;

	spin_lock_bh(&smsg_list_lock);
	cb = NULL;
	list_for_each_entry(tmp, &smsg_list, list)
		if (tmp->callback == callback &&
		    strcmp(tmp->prefix, prefix) == 0) {
			cb = tmp;
			list_del(&cb->list);
			break;
		}
	spin_unlock_bh(&smsg_list_lock);
	kfree(cb);
}

static struct device_driver smsg_driver = {
	.name = "SMSGIUCV",
	.bus  = &iucv_bus,
};

static void __exit smsg_exit(void)
{
	cpcmd("SET SMSG IUCV", NULL, 0, NULL);
	iucv_unregister(&smsg_handler, 1);
	driver_unregister(&smsg_driver);
}

static int __init smsg_init(void)
{
	int rc;

	if (!MACHINE_IS_VM) {
		rc = -EPROTONOSUPPORT;
		goto out;
	}
	rc = driver_register(&smsg_driver);
	if (rc != 0)
		goto out;
	rc = iucv_register(&smsg_handler, 1);
	if (rc)
		goto out_driver;
	smsg_path = iucv_path_alloc(255, 0, GFP_KERNEL);
	if (!smsg_path) {
		rc = -ENOMEM;
		goto out_register;
	}
	rc = iucv_path_connect(smsg_path, &smsg_handler, "*MSG    ",
			       NULL, NULL, NULL);
	if (rc)
		goto out_free;
	cpcmd("SET SMSG IUCV", NULL, 0, NULL);
	return 0;

out_free:
	iucv_path_free(smsg_path);
out_register:
	iucv_unregister(&smsg_handler, 1);
out_driver:
	driver_unregister(&smsg_driver);
out:
	return rc;
}

module_init(smsg_init);
module_exit(smsg_exit);
MODULE_LICENSE("GPL");

EXPORT_SYMBOL(smsg_register_callback);
EXPORT_SYMBOL(smsg_unregister_callback);
