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
 *  Silicon Labs C2 port Linux support
 *
 *  Copyright (c) 2007 Rodolfo Giometti <giometti@linux.it>
 *  Copyright (c) 2007 Eurotech S.p.A. <info@eurotech.it>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation
 */

#include <linux/device.h>

#define C2PORT_NAME_LEN			32

/*
 * C2 port basic structs
 */

/* Main struct */
struct c2port_ops;
struct c2port_device {
	unsigned int access:1;
	unsigned int flash_access:1;

	int id;
	char name[C2PORT_NAME_LEN];
	struct c2port_ops *ops;
	struct mutex mutex;		/* prevent races during read/write */

	struct device *dev;

	void *private_data;
};

/* Basic operations */
struct c2port_ops {
	/* Flash layout */
	unsigned short block_size;	/* flash block size in bytes */
	unsigned short blocks_num;	/* flash blocks number */

	/* Enable or disable the access to C2 port */
	void (*access)(struct c2port_device *dev, int status);

	/* Set C2D data line as input/output */
	void (*c2d_dir)(struct c2port_device *dev, int dir);

	/* Read/write C2D data line */
	int (*c2d_get)(struct c2port_device *dev);
	void (*c2d_set)(struct c2port_device *dev, int status);

	/* Write C2CK clock line */
	void (*c2ck_set)(struct c2port_device *dev, int status);
};

/*
 * Exported functions
 */

#define to_class_dev(obj) container_of((obj), struct class_device, kobj)
#define to_c2port_device(obj) container_of((obj), struct c2port_device, class)

extern struct c2port_device *c2port_device_register(char *name,
					struct c2port_ops *ops, void *devdata);
extern void c2port_device_unregister(struct c2port_device *dev);
