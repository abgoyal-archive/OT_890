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
 * linux/drivers/firmware/memmap.c
 *  Copyright (C) 2008 SUSE LINUX Products GmbH
 *  by Bernhard Walle <bernhard.walle@gmx.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License v2.0 as published by
 * the Free Software Foundation
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/string.h>
#include <linux/firmware-map.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/bootmem.h>

/*
 * Data types ------------------------------------------------------------------
 */

/*
 * Firmware map entry. Because firmware memory maps are flat and not
 * hierarchical, it's ok to organise them in a linked list. No parent
 * information is necessary as for the resource tree.
 */
struct firmware_map_entry {
	resource_size_t		start;	/* start of the memory range */
	resource_size_t		end;	/* end of the memory range (incl.) */
	const char		*type;	/* type of the memory range */
	struct list_head	list;	/* entry for the linked list */
	struct kobject		kobj;   /* kobject for each entry */
};

/*
 * Forward declarations --------------------------------------------------------
 */
static ssize_t memmap_attr_show(struct kobject *kobj,
				struct attribute *attr, char *buf);
static ssize_t start_show(struct firmware_map_entry *entry, char *buf);
static ssize_t end_show(struct firmware_map_entry *entry, char *buf);
static ssize_t type_show(struct firmware_map_entry *entry, char *buf);

/*
 * Static data -----------------------------------------------------------------
 */

struct memmap_attribute {
	struct attribute attr;
	ssize_t (*show)(struct firmware_map_entry *entry, char *buf);
};

static struct memmap_attribute memmap_start_attr = __ATTR_RO(start);
static struct memmap_attribute memmap_end_attr   = __ATTR_RO(end);
static struct memmap_attribute memmap_type_attr  = __ATTR_RO(type);

/*
 * These are default attributes that are added for every memmap entry.
 */
static struct attribute *def_attrs[] = {
	&memmap_start_attr.attr,
	&memmap_end_attr.attr,
	&memmap_type_attr.attr,
	NULL
};

static struct sysfs_ops memmap_attr_ops = {
	.show = memmap_attr_show,
};

static struct kobj_type memmap_ktype = {
	.sysfs_ops	= &memmap_attr_ops,
	.default_attrs	= def_attrs,
};

/*
 * Registration functions ------------------------------------------------------
 */

/*
 * Firmware memory map entries. No locking is needed because the
 * firmware_map_add() and firmware_map_add_early() functions are called
 * in firmware initialisation code in one single thread of execution.
 */
static LIST_HEAD(map_entries);

/**
 * firmware_map_add_entry() - Does the real work to add a firmware memmap entry.
 * @start: Start of the memory range.
 * @end:   End of the memory range (inclusive).
 * @type:  Type of the memory range.
 * @entry: Pre-allocated (either kmalloc() or bootmem allocator), uninitialised
 *         entry.
 *
 * Common implementation of firmware_map_add() and firmware_map_add_early()
 * which expects a pre-allocated struct firmware_map_entry.
 **/
static int firmware_map_add_entry(resource_size_t start, resource_size_t end,
				  const char *type,
				  struct firmware_map_entry *entry)
{
	BUG_ON(start > end);

	entry->start = start;
	entry->end = end;
	entry->type = type;
	INIT_LIST_HEAD(&entry->list);
	kobject_init(&entry->kobj, &memmap_ktype);

	list_add_tail(&entry->list, &map_entries);

	return 0;
}

/**
 * firmware_map_add() - Adds a firmware mapping entry.
 * @start: Start of the memory range.
 * @end:   End of the memory range (inclusive).
 * @type:  Type of the memory range.
 *
 * This function uses kmalloc() for memory
 * allocation. Use firmware_map_add_early() if you want to use the bootmem
 * allocator.
 *
 * That function must be called before late_initcall.
 *
 * Returns 0 on success, or -ENOMEM if no memory could be allocated.
 **/
int firmware_map_add(resource_size_t start, resource_size_t end,
		     const char *type)
{
	struct firmware_map_entry *entry;

	entry = kmalloc(sizeof(struct firmware_map_entry), GFP_ATOMIC);
	if (!entry)
		return -ENOMEM;

	return firmware_map_add_entry(start, end, type, entry);
}

/**
 * firmware_map_add_early() - Adds a firmware mapping entry.
 * @start: Start of the memory range.
 * @end:   End of the memory range (inclusive).
 * @type:  Type of the memory range.
 *
 * Adds a firmware mapping entry. This function uses the bootmem allocator
 * for memory allocation. Use firmware_map_add() if you want to use kmalloc().
 *
 * That function must be called before late_initcall.
 *
 * Returns 0 on success, or -ENOMEM if no memory could be allocated.
 **/
int __init firmware_map_add_early(resource_size_t start, resource_size_t end,
				  const char *type)
{
	struct firmware_map_entry *entry;

	entry = alloc_bootmem_low(sizeof(struct firmware_map_entry));
	if (WARN_ON(!entry))
		return -ENOMEM;

	return firmware_map_add_entry(start, end, type, entry);
}

/*
 * Sysfs functions -------------------------------------------------------------
 */

static ssize_t start_show(struct firmware_map_entry *entry, char *buf)
{
	return snprintf(buf, PAGE_SIZE, "0x%llx\n",
		(unsigned long long)entry->start);
}

static ssize_t end_show(struct firmware_map_entry *entry, char *buf)
{
	return snprintf(buf, PAGE_SIZE, "0x%llx\n",
		(unsigned long long)entry->end);
}

static ssize_t type_show(struct firmware_map_entry *entry, char *buf)
{
	return snprintf(buf, PAGE_SIZE, "%s\n", entry->type);
}

#define to_memmap_attr(_attr) container_of(_attr, struct memmap_attribute, attr)
#define to_memmap_entry(obj) container_of(obj, struct firmware_map_entry, kobj)

static ssize_t memmap_attr_show(struct kobject *kobj,
				struct attribute *attr, char *buf)
{
	struct firmware_map_entry *entry = to_memmap_entry(kobj);
	struct memmap_attribute *memmap_attr = to_memmap_attr(attr);

	return memmap_attr->show(entry, buf);
}

/*
 * Initialises stuff and adds the entries in the map_entries list to
 * sysfs. Important is that firmware_map_add() and firmware_map_add_early()
 * must be called before late_initcall. That's just because that function
 * is called as late_initcall() function, which means that if you call
 * firmware_map_add() or firmware_map_add_early() afterwards, the entries
 * are not added to sysfs.
 */
static int __init memmap_init(void)
{
	int i = 0;
	struct firmware_map_entry *entry;
	struct kset *memmap_kset;

	memmap_kset = kset_create_and_add("memmap", NULL, firmware_kobj);
	if (WARN_ON(!memmap_kset))
		return -ENOMEM;

	list_for_each_entry(entry, &map_entries, list) {
		entry->kobj.kset = memmap_kset;
		if (kobject_add(&entry->kobj, NULL, "%d", i++))
			kobject_put(&entry->kobj);
	}

	return 0;
}
late_initcall(memmap_init);

