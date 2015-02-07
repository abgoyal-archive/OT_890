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
 *  output.c - Display Output Switch driver
 *
 *  Copyright (C) 2006 Luming Yu <luming.yu@intel.com>
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or (at
 *  your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
#include <linux/module.h>
#include <linux/video_output.h>
#include <linux/err.h>
#include <linux/ctype.h>


MODULE_DESCRIPTION("Display Output Switcher Lowlevel Control Abstraction");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Luming Yu <luming.yu@intel.com>");

static ssize_t video_output_show_state(struct device *dev,
				       struct device_attribute *attr, char *buf)
{
	ssize_t ret_size = 0;
	struct output_device *od = to_output_device(dev);
	if (od->props)
		ret_size = sprintf(buf,"%.8x\n",od->props->get_status(od));
	return ret_size;
}

static ssize_t video_output_store_state(struct device *dev,
					struct device_attribute *attr,
					const char *buf,size_t count)
{
	char *endp;
	struct output_device *od = to_output_device(dev);
	int request_state = simple_strtoul(buf,&endp,0);
	size_t size = endp - buf;

	if (*endp && isspace(*endp))
		size++;
	if (size != count)
		return -EINVAL;

	if (od->props) {
		od->request_state = request_state;
		od->props->set_state(od);
	}
	return count;
}

static void video_output_release(struct device *dev)
{
	struct output_device *od = to_output_device(dev);
	kfree(od);
}

static struct device_attribute video_output_attributes[] = {
	__ATTR(state, 0644, video_output_show_state, video_output_store_state),
	__ATTR_NULL,
};


static struct class video_output_class = {
	.name = "video_output",
	.dev_release = video_output_release,
	.dev_attrs = video_output_attributes,
};

struct output_device *video_output_register(const char *name,
	struct device *dev,
	void *devdata,
	struct output_properties *op)
{
	struct output_device *new_dev;
	int ret_code = 0;

	new_dev = kzalloc(sizeof(struct output_device),GFP_KERNEL);
	if (!new_dev) {
		ret_code = -ENOMEM;
		goto error_return;
	}
	new_dev->props = op;
	new_dev->dev.class = &video_output_class;
	new_dev->dev.parent = dev;
	dev_set_name(&new_dev->dev, name);
	dev_set_drvdata(&new_dev->dev, devdata);
	ret_code = device_register(&new_dev->dev);
	if (ret_code) {
		kfree(new_dev);
		goto error_return;
	}
	return new_dev;

error_return:
	return ERR_PTR(ret_code);
}
EXPORT_SYMBOL(video_output_register);

void video_output_unregister(struct output_device *dev)
{
	if (!dev)
		return;
	device_unregister(&dev->dev);
}
EXPORT_SYMBOL(video_output_unregister);

static void __exit video_output_class_exit(void)
{
	class_unregister(&video_output_class);
}

static int __init video_output_class_init(void)
{
	return class_register(&video_output_class);
}

postcore_initcall(video_output_class_init);
module_exit(video_output_class_exit);
