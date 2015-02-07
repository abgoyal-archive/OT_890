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
 *  Universal power supply monitor class
 *
 *  Copyright © 2007  Anton Vorontsov <cbou@mail.ru>
 *  Copyright © 2004  Szabolcs Gyurko
 *  Copyright © 2003  Ian Molton <spyro@f2s.com>
 *
 *  Modified: 2004, Oct     Szabolcs Gyurko
 *
 *  You may use this code as per GPL version 2
 */

#include <linux/module.h>
#include <linux/types.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/power_supply.h>
#include "power_supply.h"

struct class *power_supply_class;

static int __power_supply_changed_work(struct device *dev, void *data)
{
	struct power_supply *psy = (struct power_supply *)data;
	struct power_supply *pst = dev_get_drvdata(dev);
	int i;

	for (i = 0; i < psy->num_supplicants; i++)
		if (!strcmp(psy->supplied_to[i], pst->name)) {
			if (pst->external_power_changed)
				pst->external_power_changed(pst);
		}
	return 0;
}

static void power_supply_changed_work(struct work_struct *work)
{
	struct power_supply *psy = container_of(work, struct power_supply,
						changed_work);

	dev_dbg(psy->dev, "%s\n", __func__);

	class_for_each_device(power_supply_class, NULL, psy,
			      __power_supply_changed_work);

	power_supply_update_leds(psy);

	kobject_uevent(&psy->dev->kobj, KOBJ_CHANGE);
}

void power_supply_changed(struct power_supply *psy)
{
	dev_dbg(psy->dev, "%s\n", __func__);

	schedule_work(&psy->changed_work);
}

static int __power_supply_am_i_supplied(struct device *dev, void *data)
{
	union power_supply_propval ret = {0,};
	struct power_supply *psy = (struct power_supply *)data;
	struct power_supply *epsy = dev_get_drvdata(dev);
	int i;

	for (i = 0; i < epsy->num_supplicants; i++) {
		if (!strcmp(epsy->supplied_to[i], psy->name)) {
			if (epsy->get_property(epsy,
				  POWER_SUPPLY_PROP_ONLINE, &ret))
				continue;
			if (ret.intval)
				return ret.intval;
		}
	}
	return 0;
}

int power_supply_am_i_supplied(struct power_supply *psy)
{
	int error;

	error = class_for_each_device(power_supply_class, NULL, psy,
				      __power_supply_am_i_supplied);

	dev_dbg(psy->dev, "%s %d\n", __func__, error);

	return error;
}

static int __power_supply_is_system_supplied(struct device *dev, void *data)
{
	union power_supply_propval ret = {0,};
	struct power_supply *psy = dev_get_drvdata(dev);

	if (psy->type != POWER_SUPPLY_TYPE_BATTERY) {
		if (psy->get_property(psy, POWER_SUPPLY_PROP_ONLINE, &ret))
			return 0;
		if (ret.intval)
			return ret.intval;
	}
	return 0;
}

int power_supply_is_system_supplied(void)
{
	int error;

	error = class_for_each_device(power_supply_class, NULL, NULL,
				      __power_supply_is_system_supplied);

	return error;
}

int power_supply_register(struct device *parent, struct power_supply *psy)
{
	int rc = 0;

	psy->dev = device_create(power_supply_class, parent, 0, psy,
				 "%s", psy->name);
	if (IS_ERR(psy->dev)) {
		rc = PTR_ERR(psy->dev);
		goto dev_create_failed;
	}

	INIT_WORK(&psy->changed_work, power_supply_changed_work);

	rc = power_supply_create_attrs(psy);
	if (rc)
		goto create_attrs_failed;

	rc = power_supply_create_triggers(psy);
	if (rc)
		goto create_triggers_failed;

	power_supply_changed(psy);

	goto success;

create_triggers_failed:
	power_supply_remove_attrs(psy);
create_attrs_failed:
	device_unregister(psy->dev);
dev_create_failed:
success:
	return rc;
}

void power_supply_unregister(struct power_supply *psy)
{
	flush_scheduled_work();
	power_supply_remove_triggers(psy);
	power_supply_remove_attrs(psy);
	device_unregister(psy->dev);
}

static int __init power_supply_class_init(void)
{
	power_supply_class = class_create(THIS_MODULE, "power_supply");

	if (IS_ERR(power_supply_class))
		return PTR_ERR(power_supply_class);

	power_supply_class->dev_uevent = power_supply_uevent;

	return 0;
}

static void __exit power_supply_class_exit(void)
{
	class_destroy(power_supply_class);
}

EXPORT_SYMBOL_GPL(power_supply_changed);
EXPORT_SYMBOL_GPL(power_supply_am_i_supplied);
EXPORT_SYMBOL_GPL(power_supply_is_system_supplied);
EXPORT_SYMBOL_GPL(power_supply_register);
EXPORT_SYMBOL_GPL(power_supply_unregister);

/* exported for the APM Power driver, APM emulation */
EXPORT_SYMBOL_GPL(power_supply_class);

subsys_initcall(power_supply_class_init);
module_exit(power_supply_class_exit);

MODULE_DESCRIPTION("Universal power supply monitor class");
MODULE_AUTHOR("Ian Molton <spyro@f2s.com>, "
	      "Szabolcs Gyurko, "
	      "Anton Vorontsov <cbou@mail.ru>");
MODULE_LICENSE("GPL");
