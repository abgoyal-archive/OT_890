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
 * Interface for Dynamic Logical Partitioning of I/O Slots on
 * RPA-compliant PPC64 platform.
 *
 * John Rose <johnrose@austin.ibm.com>
 * October 2003
 *
 * Copyright (C) 2003 IBM.
 *
 *      This program is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU General Public License
 *      as published by the Free Software Foundation; either version
 *      2 of the License, or (at your option) any later version.
 */
#include <linux/kobject.h>
#include <linux/string.h>
#include <linux/pci.h>
#include <linux/pci_hotplug.h>
#include "rpadlpar.h"
#include "../pci.h"

#define DLPAR_KOBJ_NAME       "control"

/* Those two have no quotes because they are passed to __ATTR() which
 * stringifies the argument (yuck !)
 */
#define ADD_SLOT_ATTR_NAME    add_slot
#define REMOVE_SLOT_ATTR_NAME remove_slot

#define MAX_DRC_NAME_LEN 64

static ssize_t add_slot_store(struct kobject *kobj, struct kobj_attribute *attr,
			      const char *buf, size_t nbytes)
{
	char drc_name[MAX_DRC_NAME_LEN];
	char *end;
	int rc;

	if (nbytes >= MAX_DRC_NAME_LEN)
		return 0;

	memcpy(drc_name, buf, nbytes);

	end = strchr(drc_name, '\n');
	if (!end)
		end = &drc_name[nbytes];
	*end = '\0';

	rc = dlpar_add_slot(drc_name);
	if (rc)
		return rc;

	return nbytes;
}

static ssize_t add_slot_show(struct kobject *kobj,
			     struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "0\n");
}

static ssize_t remove_slot_store(struct kobject *kobj,
				 struct kobj_attribute *attr,
				 const char *buf, size_t nbytes)
{
	char drc_name[MAX_DRC_NAME_LEN];
	int rc;
	char *end;

	if (nbytes >= MAX_DRC_NAME_LEN)
		return 0;

	memcpy(drc_name, buf, nbytes);

	end = strchr(drc_name, '\n');
	if (!end)
		end = &drc_name[nbytes];
	*end = '\0';

	rc = dlpar_remove_slot(drc_name);
	if (rc)
		return rc;

	return nbytes;
}

static ssize_t remove_slot_show(struct kobject *kobj,
				struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "0\n");
}

static struct kobj_attribute add_slot_attr =
	__ATTR(ADD_SLOT_ATTR_NAME, 0644, add_slot_show, add_slot_store);

static struct kobj_attribute remove_slot_attr =
	__ATTR(REMOVE_SLOT_ATTR_NAME, 0644, remove_slot_show, remove_slot_store);

static struct attribute *default_attrs[] = {
	&add_slot_attr.attr,
	&remove_slot_attr.attr,
	NULL,
};

static struct attribute_group dlpar_attr_group = {
	.attrs = default_attrs,
};

static struct kobject *dlpar_kobj;

int dlpar_sysfs_init(void)
{
	int error;

	dlpar_kobj = kobject_create_and_add(DLPAR_KOBJ_NAME,
					    &pci_slots_kset->kobj);
	if (!dlpar_kobj)
		return -EINVAL;

	error = sysfs_create_group(dlpar_kobj, &dlpar_attr_group);
	if (error)
		kobject_put(dlpar_kobj);
	return error;
}

void dlpar_sysfs_exit(void)
{
	sysfs_remove_group(dlpar_kobj, &dlpar_attr_group);
	kobject_put(dlpar_kobj);
}
