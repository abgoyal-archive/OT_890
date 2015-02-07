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
 * Sample kobject implementation
 *
 * Copyright (C) 2004-2007 Greg Kroah-Hartman <greg@kroah.com>
 * Copyright (C) 2007 Novell Inc.
 *
 * Released under the GPL version 2 only.
 *
 */
#include <linux/kobject.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/module.h>
#include <linux/init.h>

/*
 * This module shows how to create a simple subdirectory in sysfs called
 * /sys/kernel/kobject-example  In that directory, 3 files are created:
 * "foo", "baz", and "bar".  If an integer is written to these files, it can be
 * later read out of it.
 */

static int foo;
static int baz;
static int bar;

/*
 * The "foo" file where a static variable is read from and written to.
 */
static ssize_t foo_show(struct kobject *kobj, struct kobj_attribute *attr,
			char *buf)
{
	return sprintf(buf, "%d\n", foo);
}

static ssize_t foo_store(struct kobject *kobj, struct kobj_attribute *attr,
			 const char *buf, size_t count)
{
	sscanf(buf, "%du", &foo);
	return count;
}

static struct kobj_attribute foo_attribute =
	__ATTR(foo, 0666, foo_show, foo_store);

/*
 * More complex function where we determine which varible is being accessed by
 * looking at the attribute for the "baz" and "bar" files.
 */
static ssize_t b_show(struct kobject *kobj, struct kobj_attribute *attr,
		      char *buf)
{
	int var;

	if (strcmp(attr->attr.name, "baz") == 0)
		var = baz;
	else
		var = bar;
	return sprintf(buf, "%d\n", var);
}

static ssize_t b_store(struct kobject *kobj, struct kobj_attribute *attr,
		       const char *buf, size_t count)
{
	int var;

	sscanf(buf, "%du", &var);
	if (strcmp(attr->attr.name, "baz") == 0)
		baz = var;
	else
		bar = var;
	return count;
}

static struct kobj_attribute baz_attribute =
	__ATTR(baz, 0666, b_show, b_store);
static struct kobj_attribute bar_attribute =
	__ATTR(bar, 0666, b_show, b_store);


/*
 * Create a group of attributes so that we can create and destory them all
 * at once.
 */
static struct attribute *attrs[] = {
	&foo_attribute.attr,
	&baz_attribute.attr,
	&bar_attribute.attr,
	NULL,	/* need to NULL terminate the list of attributes */
};

/*
 * An unnamed attribute group will put all of the attributes directly in
 * the kobject directory.  If we specify a name, a subdirectory will be
 * created for the attributes with the directory being the name of the
 * attribute group.
 */
static struct attribute_group attr_group = {
	.attrs = attrs,
};

static struct kobject *example_kobj;

static int __init example_init(void)
{
	int retval;

	/*
	 * Create a simple kobject with the name of "kobject_example",
	 * located under /sys/kernel/
	 *
	 * As this is a simple directory, no uevent will be sent to
	 * userspace.  That is why this function should not be used for
	 * any type of dynamic kobjects, where the name and number are
	 * not known ahead of time.
	 */
	example_kobj = kobject_create_and_add("kobject_example", kernel_kobj);
	if (!example_kobj)
		return -ENOMEM;

	/* Create the files associated with this kobject */
	retval = sysfs_create_group(example_kobj, &attr_group);
	if (retval)
		kobject_put(example_kobj);

	return retval;
}

static void __exit example_exit(void)
{
	kobject_put(example_kobj);
}

module_init(example_init);
module_exit(example_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Greg Kroah-Hartman <greg@kroah.com>");
