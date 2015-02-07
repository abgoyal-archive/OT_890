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

/* cxgb3i_init.c: Chelsio S3xx iSCSI driver.
 *
 * Copyright (c) 2008 Chelsio Communications, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation.
 *
 * Written by: Karen Xie (kxie@chelsio.com)
 */

#include "cxgb3i.h"

#define DRV_MODULE_NAME         "cxgb3i"
#define DRV_MODULE_VERSION	"1.0.1"
#define DRV_MODULE_RELDATE	"Jan. 2009"

static char version[] =
	"Chelsio S3xx iSCSI Driver " DRV_MODULE_NAME
	" v" DRV_MODULE_VERSION " (" DRV_MODULE_RELDATE ")\n";

MODULE_AUTHOR("Karen Xie <kxie@chelsio.com>");
MODULE_DESCRIPTION("Chelsio S3xx iSCSI Driver");
MODULE_LICENSE("GPL");
MODULE_VERSION(DRV_MODULE_VERSION);

static void open_s3_dev(struct t3cdev *);
static void close_s3_dev(struct t3cdev *);

static cxgb3_cpl_handler_func cxgb3i_cpl_handlers[NUM_CPL_CMDS];
static struct cxgb3_client t3c_client = {
	.name = "iscsi_cxgb3",
	.handlers = cxgb3i_cpl_handlers,
	.add = open_s3_dev,
	.remove = close_s3_dev,
};

/**
 * open_s3_dev - register with cxgb3 LLD
 * @t3dev:	cxgb3 adapter instance
 */
static void open_s3_dev(struct t3cdev *t3dev)
{
	static int vers_printed;

	if (!vers_printed) {
		printk(KERN_INFO "%s", version);
		vers_printed = 1;
	}

	cxgb3i_sdev_add(t3dev, &t3c_client);
	cxgb3i_adapter_add(t3dev);
}

/**
 * close_s3_dev - de-register with cxgb3 LLD
 * @t3dev:	cxgb3 adapter instance
 */
static void close_s3_dev(struct t3cdev *t3dev)
{
	cxgb3i_adapter_remove(t3dev);
	cxgb3i_sdev_remove(t3dev);
}

/**
 * cxgb3i_init_module - module init entry point
 *
 * initialize any driver wide global data structures and register itself
 *	with the cxgb3 module
 */
static int __init cxgb3i_init_module(void)
{
	int err;

	err = cxgb3i_sdev_init(cxgb3i_cpl_handlers);
	if (err < 0)
		return err;

	err = cxgb3i_iscsi_init();
	if (err < 0)
		return err;

	err = cxgb3i_pdu_init();
	if (err < 0)
		return err;

	cxgb3_register_client(&t3c_client);

	return 0;
}

/**
 * cxgb3i_exit_module - module cleanup/exit entry point
 *
 * go through the driver hba list and for each hba, release any resource held.
 *	and unregisters iscsi transport and the cxgb3 module
 */
static void __exit cxgb3i_exit_module(void)
{
	cxgb3_unregister_client(&t3c_client);
	cxgb3i_pdu_cleanup();
	cxgb3i_iscsi_cleanup();
	cxgb3i_sdev_cleanup();
}

module_init(cxgb3i_init_module);
module_exit(cxgb3i_exit_module);
