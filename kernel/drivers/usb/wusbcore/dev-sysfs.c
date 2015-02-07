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
 * WUSB devices
 * sysfs bindings
 *
 * Copyright (C) 2007 Intel Corporation
 * Inaky Perez-Gonzalez <inaky.perez-gonzalez@intel.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version
 * 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *
 *
 * Get them out of the way...
 */

#include <linux/jiffies.h>
#include <linux/ctype.h>
#include <linux/workqueue.h>
#include "wusbhc.h"

static ssize_t wusb_disconnect_store(struct device *dev,
				     struct device_attribute *attr,
				     const char *buf, size_t size)
{
	struct usb_device *usb_dev;
	struct wusbhc *wusbhc;
	unsigned command;
	u8 port_idx;

	if (sscanf(buf, "%u", &command) != 1)
		return -EINVAL;
	if (command == 0)
		return size;
	usb_dev = to_usb_device(dev);
	wusbhc = wusbhc_get_by_usb_dev(usb_dev);
	if (wusbhc == NULL)
		return -ENODEV;

	mutex_lock(&wusbhc->mutex);
	port_idx = wusb_port_no_to_idx(usb_dev->portnum);
	__wusbhc_dev_disable(wusbhc, port_idx);
	mutex_unlock(&wusbhc->mutex);
	wusbhc_put(wusbhc);
	return size;
}
static DEVICE_ATTR(wusb_disconnect, 0200, NULL, wusb_disconnect_store);

static ssize_t wusb_cdid_show(struct device *dev,
			      struct device_attribute *attr, char *buf)
{
	ssize_t result;
	struct wusb_dev *wusb_dev;

	wusb_dev = wusb_dev_get_by_usb_dev(to_usb_device(dev));
	if (wusb_dev == NULL)
		return -ENODEV;
	result = ckhdid_printf(buf, PAGE_SIZE, &wusb_dev->cdid);
	strcat(buf, "\n");
	wusb_dev_put(wusb_dev);
	return result + 1;
}
static DEVICE_ATTR(wusb_cdid, 0444, wusb_cdid_show, NULL);

static ssize_t wusb_ck_store(struct device *dev,
			     struct device_attribute *attr,
			     const char *buf, size_t size)
{
	int result;
	struct usb_device *usb_dev;
	struct wusbhc *wusbhc;
	struct wusb_ckhdid ck;

	result = sscanf(buf,
			"%02hhx %02hhx %02hhx %02hhx "
			"%02hhx %02hhx %02hhx %02hhx "
			"%02hhx %02hhx %02hhx %02hhx "
			"%02hhx %02hhx %02hhx %02hhx\n",
			&ck.data[0] , &ck.data[1],
			&ck.data[2] , &ck.data[3],
			&ck.data[4] , &ck.data[5],
			&ck.data[6] , &ck.data[7],
			&ck.data[8] , &ck.data[9],
			&ck.data[10], &ck.data[11],
			&ck.data[12], &ck.data[13],
			&ck.data[14], &ck.data[15]);
	if (result != 16)
		return -EINVAL;

	usb_dev = to_usb_device(dev);
	wusbhc = wusbhc_get_by_usb_dev(usb_dev);
	if (wusbhc == NULL)
		return -ENODEV;
	result = wusb_dev_4way_handshake(wusbhc, usb_dev->wusb_dev, &ck);
	memset(&ck, 0, sizeof(ck));
	wusbhc_put(wusbhc);
	return result < 0 ? result : size;
}
static DEVICE_ATTR(wusb_ck, 0200, NULL, wusb_ck_store);

static struct attribute *wusb_dev_attrs[] = {
		&dev_attr_wusb_disconnect.attr,
		&dev_attr_wusb_cdid.attr,
		&dev_attr_wusb_ck.attr,
		NULL,
};

static struct attribute_group wusb_dev_attr_group = {
	.name = NULL,	/* we want them in the same directory */
	.attrs = wusb_dev_attrs,
};

int wusb_dev_sysfs_add(struct wusbhc *wusbhc, struct usb_device *usb_dev,
		       struct wusb_dev *wusb_dev)
{
	int result = sysfs_create_group(&usb_dev->dev.kobj,
					&wusb_dev_attr_group);
	struct device *dev = &usb_dev->dev;
	if (result < 0)
		dev_err(dev, "Cannot register WUSB-dev attributes: %d\n",
			result);
	return result;
}

void wusb_dev_sysfs_rm(struct wusb_dev *wusb_dev)
{
	struct usb_device *usb_dev = wusb_dev->usb_dev;
	if (usb_dev)
		sysfs_remove_group(&usb_dev->dev.kobj, &wusb_dev_attr_group);
}
