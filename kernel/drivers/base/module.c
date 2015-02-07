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
 * module.c - module sysfs fun for drivers
 *
 * This file is released under the GPLv2
 *
 */
#include <linux/device.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/string.h>
#include "base.h"

static char *make_driver_name(struct device_driver *drv)
{
	char *driver_name;

	driver_name = kmalloc(strlen(drv->name) + strlen(drv->bus->name) + 2,
			      GFP_KERNEL);
	if (!driver_name)
		return NULL;

	sprintf(driver_name, "%s:%s", drv->bus->name, drv->name);
	return driver_name;
}

static void module_create_drivers_dir(struct module_kobject *mk)
{
	if (!mk || mk->drivers_dir)
		return;

	mk->drivers_dir = kobject_create_and_add("drivers", &mk->kobj);
}

void module_add_driver(struct module *mod, struct device_driver *drv)
{
	char *driver_name;
	int no_warn;
	struct module_kobject *mk = NULL;

	if (!drv)
		return;

	if (mod)
		mk = &mod->mkobj;
	else if (drv->mod_name) {
		struct kobject *mkobj;

		/* Lookup built-in module entry in /sys/modules */
		mkobj = kset_find_obj(module_kset, drv->mod_name);
		if (mkobj) {
			mk = container_of(mkobj, struct module_kobject, kobj);
			/* remember our module structure */
			drv->p->mkobj = mk;
			/* kset_find_obj took a reference */
			kobject_put(mkobj);
		}
	}

	if (!mk)
		return;

	/* Don't check return codes; these calls are idempotent */
	no_warn = sysfs_create_link(&drv->p->kobj, &mk->kobj, "module");
	driver_name = make_driver_name(drv);
	if (driver_name) {
		module_create_drivers_dir(mk);
		no_warn = sysfs_create_link(mk->drivers_dir, &drv->p->kobj,
					    driver_name);
		kfree(driver_name);
	}
}

void module_remove_driver(struct device_driver *drv)
{
	struct module_kobject *mk = NULL;
	char *driver_name;

	if (!drv)
		return;

	sysfs_remove_link(&drv->p->kobj, "module");

	if (drv->owner)
		mk = &drv->owner->mkobj;
	else if (drv->p->mkobj)
		mk = drv->p->mkobj;
	if (mk && mk->drivers_dir) {
		driver_name = make_driver_name(drv);
		if (driver_name) {
			sysfs_remove_link(mk->drivers_dir, driver_name);
			kfree(driver_name);
		}
	}
}
