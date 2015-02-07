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
 * Copyright (C) 2008 Red Hat, Inc. All rights reserved.
 *
 * This file is released under the GPL.
 */

#include <linux/sysfs.h>
#include <linux/dm-ioctl.h>
#include "dm.h"

struct dm_sysfs_attr {
	struct attribute attr;
	ssize_t (*show)(struct mapped_device *, char *);
	ssize_t (*store)(struct mapped_device *, char *);
};

#define DM_ATTR_RO(_name) \
struct dm_sysfs_attr dm_attr_##_name = \
	__ATTR(_name, S_IRUGO, dm_attr_##_name##_show, NULL)

static ssize_t dm_attr_show(struct kobject *kobj, struct attribute *attr,
			    char *page)
{
	struct dm_sysfs_attr *dm_attr;
	struct mapped_device *md;
	ssize_t ret;

	dm_attr = container_of(attr, struct dm_sysfs_attr, attr);
	if (!dm_attr->show)
		return -EIO;

	md = dm_get_from_kobject(kobj);
	if (!md)
		return -EINVAL;

	ret = dm_attr->show(md, page);
	dm_put(md);

	return ret;
}

static ssize_t dm_attr_name_show(struct mapped_device *md, char *buf)
{
	if (dm_copy_name_and_uuid(md, buf, NULL))
		return -EIO;

	strcat(buf, "\n");
	return strlen(buf);
}

static ssize_t dm_attr_uuid_show(struct mapped_device *md, char *buf)
{
	if (dm_copy_name_and_uuid(md, NULL, buf))
		return -EIO;

	strcat(buf, "\n");
	return strlen(buf);
}

static DM_ATTR_RO(name);
static DM_ATTR_RO(uuid);

static struct attribute *dm_attrs[] = {
	&dm_attr_name.attr,
	&dm_attr_uuid.attr,
	NULL,
};

static struct sysfs_ops dm_sysfs_ops = {
	.show	= dm_attr_show,
};

/*
 * dm kobject is embedded in mapped_device structure
 * no need to define release function here
 */
static struct kobj_type dm_ktype = {
	.sysfs_ops	= &dm_sysfs_ops,
	.default_attrs	= dm_attrs,
};

/*
 * Initialize kobj
 * because nobody using md yet, no need to call explicit dm_get/put
 */
int dm_sysfs_init(struct mapped_device *md)
{
	return kobject_init_and_add(dm_kobject(md), &dm_ktype,
				    &disk_to_dev(dm_disk(md))->kobj,
				    "%s", "dm");
}

/*
 * Remove kobj, called after all references removed
 */
void dm_sysfs_exit(struct mapped_device *md)
{
	kobject_put(dm_kobject(md));
}
