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
 * (C) Copyright IBM Corp. 2004   All Rights Reserved.
 * tape_class.h
 *
 * Tape class device support
 *
 * Author: Stefan Bader <shbader@de.ibm.com>
 * Based on simple class device code by Greg K-H
 */
#ifndef __TAPE_CLASS_H__
#define __TAPE_CLASS_H__

#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/major.h>
#include <linux/kobject.h>
#include <linux/kobj_map.h>
#include <linux/cdev.h>

#include <linux/device.h>
#include <linux/kdev_t.h>

#define TAPECLASS_NAME_LEN	32

struct tape_class_device {
	struct cdev		*char_device;
	struct device		*class_device;
	char			device_name[TAPECLASS_NAME_LEN];
	char			mode_name[TAPECLASS_NAME_LEN];
};

/*
 * Register a tape device and return a pointer to the tape class device
 * created by the call.
 *
 * device
 *	The pointer to the struct device of the physical (base) device.
 * dev
 *	The intended major/minor number. The major number may be 0 to
 *	get a dynamic major number.
 * fops
 *	The pointer to the drivers file operations for the tape device.
 * device_name
 *	Pointer to the logical device name (will also be used as kobject name
 *	of the cdev). This can also be called the name of the tape class
 *	device.
 * mode_name
 *	Points to the name of the tape mode. This creates a link with that
 *	name from the physical device to the logical device (class).
 */
struct tape_class_device *register_tape_dev(
	struct device *		device,
	dev_t			dev,
	const struct file_operations *fops,
	char *			device_name,
	char *			node_name
);
void unregister_tape_dev(struct device *device, struct tape_class_device *tcd);

#endif /* __TAPE_CLASS_H__ */
