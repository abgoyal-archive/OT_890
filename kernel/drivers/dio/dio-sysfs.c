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
 *  File Attributes for DIO Devices
 *
 *  Copyright (C) 2004 Jochen Friedrich
 *
 *  Loosely based on drivers/pci/pci-sysfs.c and drivers/zorro/zorro-sysfs.c
 *
 *  This file is subject to the terms and conditions of the GNU General Public
 *  License.  See the file COPYING in the main directory of this archive
 *  for more details.
 */


#include <linux/kernel.h>
#include <linux/dio.h>
#include <linux/stat.h>

/* show configuration fields */

static ssize_t dio_show_id(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct dio_dev *d;

	d = to_dio_dev(dev);
	return sprintf(buf, "0x%02x\n", (d->id & 0xff));
}
static DEVICE_ATTR(id, S_IRUGO, dio_show_id, NULL);

static ssize_t dio_show_ipl(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct dio_dev *d;

	d = to_dio_dev(dev);
	return sprintf(buf, "0x%02x\n", d->ipl);
}
static DEVICE_ATTR(ipl, S_IRUGO, dio_show_ipl, NULL);

static ssize_t dio_show_secid(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct dio_dev *d;

	d = to_dio_dev(dev);
	return sprintf(buf, "0x%02x\n", ((d->id >> 8)& 0xff));
}
static DEVICE_ATTR(secid, S_IRUGO, dio_show_secid, NULL);

static ssize_t dio_show_name(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct dio_dev *d;

	d = to_dio_dev(dev);
	return sprintf(buf, "%s\n", d->name);
}
static DEVICE_ATTR(name, S_IRUGO, dio_show_name, NULL);

static ssize_t dio_show_resource(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct dio_dev *d = to_dio_dev(dev);

	return sprintf(buf, "0x%08lx 0x%08lx 0x%08lx\n",
		       (unsigned long)dio_resource_start(d),
		       (unsigned long)dio_resource_end(d),
		       dio_resource_flags(d));
}
static DEVICE_ATTR(resource, S_IRUGO, dio_show_resource, NULL);

int dio_create_sysfs_dev_files(struct dio_dev *d)
{
	struct device *dev = &d->dev;
	int error;

	/* current configuration's attributes */
	if ((error = device_create_file(dev, &dev_attr_id)) ||
	    (error = device_create_file(dev, &dev_attr_ipl)) ||
	    (error = device_create_file(dev, &dev_attr_secid)) ||
	    (error = device_create_file(dev, &dev_attr_name)) ||
	    (error = device_create_file(dev, &dev_attr_resource)))
		return error;

	return 0;
}

