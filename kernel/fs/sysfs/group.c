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
 * fs/sysfs/group.c - Operations for adding/removing multiple files at once.
 *
 * Copyright (c) 2003 Patrick Mochel
 * Copyright (c) 2003 Open Source Development Lab
 *
 * This file is released undert the GPL v2. 
 *
 */

#include <linux/kobject.h>
#include <linux/module.h>
#include <linux/dcache.h>
#include <linux/namei.h>
#include <linux/err.h>
#include "sysfs.h"


static void remove_files(struct sysfs_dirent *dir_sd, struct kobject *kobj,
			 const struct attribute_group *grp)
{
	struct attribute *const* attr;
	int i;

	for (i = 0, attr = grp->attrs; *attr; i++, attr++)
		sysfs_hash_and_remove(dir_sd, (*attr)->name);
}

static int create_files(struct sysfs_dirent *dir_sd, struct kobject *kobj,
			const struct attribute_group *grp, int update)
{
	struct attribute *const* attr;
	int error = 0, i;

	for (i = 0, attr = grp->attrs; *attr && !error; i++, attr++) {
		mode_t mode = 0;

		/* in update mode, we're changing the permissions or
		 * visibility.  Do this by first removing then
		 * re-adding (if required) the file */
		if (update)
			sysfs_hash_and_remove(dir_sd, (*attr)->name);
		if (grp->is_visible) {
			mode = grp->is_visible(kobj, *attr, i);
			if (!mode)
				continue;
		}
		error = sysfs_add_file_mode(dir_sd, *attr, SYSFS_KOBJ_ATTR,
					    (*attr)->mode | mode);
		if (unlikely(error))
			break;
	}
	if (error)
		remove_files(dir_sd, kobj, grp);
	return error;
}


static int internal_create_group(struct kobject *kobj, int update,
				 const struct attribute_group *grp)
{
	struct sysfs_dirent *sd;
	int error;

	BUG_ON(!kobj || (!update && !kobj->sd));

	/* Updates may happen before the object has been instantiated */
	if (unlikely(update && !kobj->sd))
		return -EINVAL;

	if (grp->name) {
		error = sysfs_create_subdir(kobj, grp->name, &sd);
		if (error)
			return error;
	} else
		sd = kobj->sd;
	sysfs_get(sd);
	error = create_files(sd, kobj, grp, update);
	if (error) {
		if (grp->name)
			sysfs_remove_subdir(sd);
	}
	sysfs_put(sd);
	return error;
}

/**
 * sysfs_create_group - given a directory kobject, create an attribute group
 * @kobj:	The kobject to create the group on
 * @grp:	The attribute group to create
 *
 * This function creates a group for the first time.  It will explicitly
 * warn and error if any of the attribute files being created already exist.
 *
 * Returns 0 on success or error.
 */
int sysfs_create_group(struct kobject *kobj,
		       const struct attribute_group *grp)
{
	return internal_create_group(kobj, 0, grp);
}

/**
 * sysfs_update_group - given a directory kobject, create an attribute group
 * @kobj:	The kobject to create the group on
 * @grp:	The attribute group to create
 *
 * This function updates an attribute group.  Unlike
 * sysfs_create_group(), it will explicitly not warn or error if any
 * of the attribute files being created already exist.  Furthermore,
 * if the visibility of the files has changed through the is_visible()
 * callback, it will update the permissions and add or remove the
 * relevant files.
 *
 * The primary use for this function is to call it after making a change
 * that affects group visibility.
 *
 * Returns 0 on success or error.
 */
int sysfs_update_group(struct kobject *kobj,
		       const struct attribute_group *grp)
{
	return internal_create_group(kobj, 1, grp);
}



void sysfs_remove_group(struct kobject * kobj, 
			const struct attribute_group * grp)
{
	struct sysfs_dirent *dir_sd = kobj->sd;
	struct sysfs_dirent *sd;

	if (grp->name) {
		sd = sysfs_get_dirent(dir_sd, grp->name);
		if (!sd) {
			WARN(!sd, KERN_WARNING "sysfs group %p not found for "
				"kobject '%s'\n", grp, kobject_name(kobj));
			return;
		}
	} else
		sd = sysfs_get(dir_sd);

	remove_files(sd, kobj, grp);
	if (grp->name)
		sysfs_remove_subdir(sd);

	sysfs_put(sd);
}


EXPORT_SYMBOL_GPL(sysfs_create_group);
EXPORT_SYMBOL_GPL(sysfs_update_group);
EXPORT_SYMBOL_GPL(sysfs_remove_group);
