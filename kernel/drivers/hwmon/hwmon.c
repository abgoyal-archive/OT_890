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
    hwmon.c - part of lm_sensors, Linux kernel modules for hardware monitoring

    This file defines the sysfs class "hwmon", for use by sensors drivers.

    Copyright (C) 2005 Mark M. Hoffman <mhoffman@lightlink.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; version 2 of the License.
*/

#include <linux/module.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/kdev_t.h>
#include <linux/idr.h>
#include <linux/hwmon.h>
#include <linux/gfp.h>
#include <linux/spinlock.h>

#define HWMON_ID_PREFIX "hwmon"
#define HWMON_ID_FORMAT HWMON_ID_PREFIX "%d"

static struct class *hwmon_class;

static DEFINE_IDR(hwmon_idr);
static DEFINE_SPINLOCK(idr_lock);

/**
 * hwmon_device_register - register w/ hwmon
 * @dev: the device to register
 *
 * hwmon_device_unregister() must be called when the device is no
 * longer needed.
 *
 * Returns the pointer to the new device.
 */
struct device *hwmon_device_register(struct device *dev)
{
	struct device *hwdev;
	int id, err;

again:
	if (unlikely(idr_pre_get(&hwmon_idr, GFP_KERNEL) == 0))
		return ERR_PTR(-ENOMEM);

	spin_lock(&idr_lock);
	err = idr_get_new(&hwmon_idr, NULL, &id);
	spin_unlock(&idr_lock);

	if (unlikely(err == -EAGAIN))
		goto again;
	else if (unlikely(err))
		return ERR_PTR(err);

	id = id & MAX_ID_MASK;
	hwdev = device_create(hwmon_class, dev, MKDEV(0, 0), NULL,
			      HWMON_ID_FORMAT, id);

	if (IS_ERR(hwdev)) {
		spin_lock(&idr_lock);
		idr_remove(&hwmon_idr, id);
		spin_unlock(&idr_lock);
	}

	return hwdev;
}

/**
 * hwmon_device_unregister - removes the previously registered class device
 *
 * @dev: the class device to destroy
 */
void hwmon_device_unregister(struct device *dev)
{
	int id;

	if (likely(sscanf(dev_name(dev), HWMON_ID_FORMAT, &id) == 1)) {
		device_unregister(dev);
		spin_lock(&idr_lock);
		idr_remove(&hwmon_idr, id);
		spin_unlock(&idr_lock);
	} else
		dev_dbg(dev->parent,
			"hwmon_device_unregister() failed: bad class ID!\n");
}

static int __init hwmon_init(void)
{
	hwmon_class = class_create(THIS_MODULE, "hwmon");
	if (IS_ERR(hwmon_class)) {
		printk(KERN_ERR "hwmon.c: couldn't create sysfs class\n");
		return PTR_ERR(hwmon_class);
	}
	return 0;
}

static void __exit hwmon_exit(void)
{
	class_destroy(hwmon_class);
}

subsys_initcall(hwmon_init);
module_exit(hwmon_exit);

EXPORT_SYMBOL_GPL(hwmon_device_register);
EXPORT_SYMBOL_GPL(hwmon_device_unregister);

MODULE_AUTHOR("Mark M. Hoffman <mhoffman@lightlink.com>");
MODULE_DESCRIPTION("hardware monitoring sysfs/class support");
MODULE_LICENSE("GPL");

