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

#include <linux/string.h>
#include <linux/kernel.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/mod_devicetable.h>
#include <linux/slab.h>

#include <asm/errno.h>

/**
 * of_match_device - Tell if an of_device structure has a matching
 * of_match structure
 * @ids: array of of device match structures to search in
 * @dev: the of device structure to match against
 *
 * Used by a driver to check whether an of_device present in the
 * system is in its list of supported devices.
 */
const struct of_device_id *of_match_device(const struct of_device_id *matches,
					const struct of_device *dev)
{
	if (!dev->node)
		return NULL;
	return of_match_node(matches, dev->node);
}
EXPORT_SYMBOL(of_match_device);

struct of_device *of_dev_get(struct of_device *dev)
{
	struct device *tmp;

	if (!dev)
		return NULL;
	tmp = get_device(&dev->dev);
	if (tmp)
		return to_of_device(tmp);
	else
		return NULL;
}
EXPORT_SYMBOL(of_dev_get);

void of_dev_put(struct of_device *dev)
{
	if (dev)
		put_device(&dev->dev);
}
EXPORT_SYMBOL(of_dev_put);

static ssize_t devspec_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	struct of_device *ofdev;

	ofdev = to_of_device(dev);
	return sprintf(buf, "%s\n", ofdev->node->full_name);
}

static ssize_t name_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	struct of_device *ofdev;

	ofdev = to_of_device(dev);
	return sprintf(buf, "%s\n", ofdev->node->name);
}

static ssize_t modalias_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	struct of_device *ofdev = to_of_device(dev);
	ssize_t len = 0;

	len = of_device_get_modalias(ofdev, buf, PAGE_SIZE - 2);
	buf[len] = '\n';
	buf[len+1] = 0;
	return len+1;
}

struct device_attribute of_platform_device_attrs[] = {
	__ATTR_RO(devspec),
	__ATTR_RO(name),
	__ATTR_RO(modalias),
	__ATTR_NULL
};

/**
 * of_release_dev - free an of device structure when all users of it are finished.
 * @dev: device that's been disconnected
 *
 * Will be called only by the device core when all users of this of device are
 * done.
 */
void of_release_dev(struct device *dev)
{
	struct of_device *ofdev;

	ofdev = to_of_device(dev);
	of_node_put(ofdev->node);
	kfree(ofdev);
}
EXPORT_SYMBOL(of_release_dev);

int of_device_register(struct of_device *ofdev)
{
	BUG_ON(ofdev->node == NULL);

	device_initialize(&ofdev->dev);

	/* device_add will assume that this device is on the same node as
	 * the parent. If there is no parent defined, set the node
	 * explicitly */
	if (!ofdev->dev.parent)
		set_dev_node(&ofdev->dev, of_node_to_nid(ofdev->node));

	return device_add(&ofdev->dev);
}
EXPORT_SYMBOL(of_device_register);

void of_device_unregister(struct of_device *ofdev)
{
	device_unregister(&ofdev->dev);
}
EXPORT_SYMBOL(of_device_unregister);

ssize_t of_device_get_modalias(struct of_device *ofdev,
				char *str, ssize_t len)
{
	const char *compat;
	int cplen, i;
	ssize_t tsize, csize, repend;

	/* Name & Type */
	csize = snprintf(str, len, "of:N%sT%s",
				ofdev->node->name, ofdev->node->type);

	/* Get compatible property if any */
	compat = of_get_property(ofdev->node, "compatible", &cplen);
	if (!compat)
		return csize;

	/* Find true end (we tolerate multiple \0 at the end */
	for (i = (cplen - 1); i >= 0 && !compat[i]; i--)
		cplen--;
	if (!cplen)
		return csize;
	cplen++;

	/* Check space (need cplen+1 chars including final \0) */
	tsize = csize + cplen;
	repend = tsize;

	if (csize >= len)		/* @ the limit, all is already filled */
		return tsize;

	if (tsize >= len) {		/* limit compat list */
		cplen = len - csize - 1;
		repend = len;
	}

	/* Copy and do char replacement */
	memcpy(&str[csize + 1], compat, cplen);
	for (i = csize; i < repend; i++) {
		char c = str[i];
		if (c == '\0')
			str[i] = 'C';
		else if (c == ' ')
			str[i] = '_';
	}

	return tsize;
}
