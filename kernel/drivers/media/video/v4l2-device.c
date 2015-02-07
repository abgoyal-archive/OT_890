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
    V4L2 device support.

    Copyright (C) 2008  Hans Verkuil <hverkuil@xs4all.nl>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <linux/types.h>
#include <linux/ioctl.h>
#include <linux/i2c.h>
#include <linux/videodev2.h>
#include <media/v4l2-device.h>

int v4l2_device_register(struct device *dev, struct v4l2_device *v4l2_dev)
{
	if (dev == NULL || v4l2_dev == NULL)
		return -EINVAL;
	/* Warn if we apparently re-register a device */
	WARN_ON(dev_get_drvdata(dev) != NULL);
	INIT_LIST_HEAD(&v4l2_dev->subdevs);
	spin_lock_init(&v4l2_dev->lock);
	v4l2_dev->dev = dev;
	snprintf(v4l2_dev->name, sizeof(v4l2_dev->name), "%s %s",
			dev->driver->name, dev->bus_id);
	dev_set_drvdata(dev, v4l2_dev);
	return 0;
}
EXPORT_SYMBOL_GPL(v4l2_device_register);

void v4l2_device_unregister(struct v4l2_device *v4l2_dev)
{
	struct v4l2_subdev *sd, *next;

	if (v4l2_dev == NULL || v4l2_dev->dev == NULL)
		return;
	dev_set_drvdata(v4l2_dev->dev, NULL);
	/* unregister subdevs */
	list_for_each_entry_safe(sd, next, &v4l2_dev->subdevs, list)
		v4l2_device_unregister_subdev(sd);

	v4l2_dev->dev = NULL;
}
EXPORT_SYMBOL_GPL(v4l2_device_unregister);

int v4l2_device_register_subdev(struct v4l2_device *dev, struct v4l2_subdev *sd)
{
	/* Check for valid input */
	if (dev == NULL || sd == NULL || !sd->name[0])
		return -EINVAL;
	/* Warn if we apparently re-register a subdev */
	WARN_ON(sd->dev != NULL);
	if (!try_module_get(sd->owner))
		return -ENODEV;
	sd->dev = dev;
	spin_lock(&dev->lock);
	list_add_tail(&sd->list, &dev->subdevs);
	spin_unlock(&dev->lock);
	return 0;
}
EXPORT_SYMBOL_GPL(v4l2_device_register_subdev);

void v4l2_device_unregister_subdev(struct v4l2_subdev *sd)
{
	/* return if it isn't registered */
	if (sd == NULL || sd->dev == NULL)
		return;
	spin_lock(&sd->dev->lock);
	list_del(&sd->list);
	spin_unlock(&sd->dev->lock);
	sd->dev = NULL;
	module_put(sd->owner);
}
EXPORT_SYMBOL_GPL(v4l2_device_unregister_subdev);
