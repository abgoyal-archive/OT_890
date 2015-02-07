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
 * Battery class driver for Apple PMU
 *
 *	Copyright © 2006  David Woodhouse <dwmw2@infradead.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/err.h>
#include <linux/power_supply.h>
#include <linux/adb.h>
#include <linux/pmu.h>

static struct pmu_battery_dev {
	struct power_supply bat;
	struct pmu_battery_info *pbi;
	char name[16];
	int propval;
} *pbats[PMU_MAX_BATTERIES];

#define to_pmu_battery_dev(x) container_of(x, struct pmu_battery_dev, bat)

/*********************************************************************
 *		Power
 *********************************************************************/

static int pmu_get_ac_prop(struct power_supply *psy,
			   enum power_supply_property psp,
			   union power_supply_propval *val)
{
	switch (psp) {
	case POWER_SUPPLY_PROP_ONLINE:
		val->intval = (!!(pmu_power_flags & PMU_PWR_AC_PRESENT)) ||
			      (pmu_battery_count == 0);
		break;
	default:
		return -EINVAL;
	}

	return 0;
}

static enum power_supply_property pmu_ac_props[] = {
	POWER_SUPPLY_PROP_ONLINE,
};

static struct power_supply pmu_ac = {
	.name = "pmu-ac",
	.type = POWER_SUPPLY_TYPE_MAINS,
	.properties = pmu_ac_props,
	.num_properties = ARRAY_SIZE(pmu_ac_props),
	.get_property = pmu_get_ac_prop,
};

/*********************************************************************
 *		Battery properties
 *********************************************************************/

static char *pmu_batt_types[] = {
	"Smart", "Comet", "Hooper", "Unknown"
};

static char *pmu_bat_get_model_name(struct pmu_battery_info *pbi)
{
	switch (pbi->flags & PMU_BATT_TYPE_MASK) {
	case PMU_BATT_TYPE_SMART:
		return pmu_batt_types[0];
	case PMU_BATT_TYPE_COMET:
		return pmu_batt_types[1];
	case PMU_BATT_TYPE_HOOPER:
		return pmu_batt_types[2];
	default: break;
	}
	return pmu_batt_types[3];
}

static int pmu_bat_get_property(struct power_supply *psy,
				enum power_supply_property psp,
				union power_supply_propval *val)
{
	struct pmu_battery_dev *pbat = to_pmu_battery_dev(psy);
	struct pmu_battery_info *pbi = pbat->pbi;

	switch (psp) {
	case POWER_SUPPLY_PROP_STATUS:
		if (pbi->flags & PMU_BATT_CHARGING)
			val->intval = POWER_SUPPLY_STATUS_CHARGING;
		else
			val->intval = POWER_SUPPLY_STATUS_DISCHARGING;
		break;
	case POWER_SUPPLY_PROP_PRESENT:
		val->intval = !!(pbi->flags & PMU_BATT_PRESENT);
		break;
	case POWER_SUPPLY_PROP_MODEL_NAME:
		val->strval = pmu_bat_get_model_name(pbi);
		break;
	case POWER_SUPPLY_PROP_ENERGY_AVG:
		val->intval = pbi->charge     * 1000; /* mWh -> µWh */
		break;
	case POWER_SUPPLY_PROP_ENERGY_FULL:
		val->intval = pbi->max_charge * 1000; /* mWh -> µWh */
		break;
	case POWER_SUPPLY_PROP_CURRENT_AVG:
		val->intval = pbi->amperage   * 1000; /* mA -> µA */
		break;
	case POWER_SUPPLY_PROP_VOLTAGE_AVG:
		val->intval = pbi->voltage    * 1000; /* mV -> µV */
		break;
	case POWER_SUPPLY_PROP_TIME_TO_EMPTY_AVG:
		val->intval = pbi->time_remaining;
		break;
	default:
		return -EINVAL;
	}

	return 0;
}

static enum power_supply_property pmu_bat_props[] = {
	POWER_SUPPLY_PROP_STATUS,
	POWER_SUPPLY_PROP_PRESENT,
	POWER_SUPPLY_PROP_MODEL_NAME,
	POWER_SUPPLY_PROP_ENERGY_AVG,
	POWER_SUPPLY_PROP_ENERGY_FULL,
	POWER_SUPPLY_PROP_CURRENT_AVG,
	POWER_SUPPLY_PROP_VOLTAGE_AVG,
	POWER_SUPPLY_PROP_TIME_TO_EMPTY_AVG,
};

/*********************************************************************
 *		Initialisation
 *********************************************************************/

static struct platform_device *bat_pdev;

static int __init pmu_bat_init(void)
{
	int ret;
	int i;

	bat_pdev = platform_device_register_simple("pmu-battery",
						   0, NULL, 0);
	if (IS_ERR(bat_pdev)) {
		ret = PTR_ERR(bat_pdev);
		goto pdev_register_failed;
	}

	ret = power_supply_register(&bat_pdev->dev, &pmu_ac);
	if (ret)
		goto ac_register_failed;

	for (i = 0; i < pmu_battery_count; i++) {
		struct pmu_battery_dev *pbat = kzalloc(sizeof(*pbat),
						       GFP_KERNEL);
		if (!pbat)
			break;

		sprintf(pbat->name, "PMU_battery_%d", i);
		pbat->bat.name = pbat->name;
		pbat->bat.properties = pmu_bat_props;
		pbat->bat.num_properties = ARRAY_SIZE(pmu_bat_props);
		pbat->bat.get_property = pmu_bat_get_property;
		pbat->pbi = &pmu_batteries[i];

		ret = power_supply_register(&bat_pdev->dev, &pbat->bat);
		if (ret) {
			kfree(pbat);
			goto battery_register_failed;
		}
		pbats[i] = pbat;
	}

	goto success;

battery_register_failed:
	while (i--) {
		if (!pbats[i])
			continue;
		power_supply_unregister(&pbats[i]->bat);
		kfree(pbats[i]);
	}
	power_supply_unregister(&pmu_ac);
ac_register_failed:
	platform_device_unregister(bat_pdev);
pdev_register_failed:
success:
	return ret;
}

static void __exit pmu_bat_exit(void)
{
	int i;

	for (i = 0; i < PMU_MAX_BATTERIES; i++) {
		if (!pbats[i])
			continue;
		power_supply_unregister(&pbats[i]->bat);
		kfree(pbats[i]);
	}
	power_supply_unregister(&pmu_ac);
	platform_device_unregister(bat_pdev);
}

module_init(pmu_bat_init);
module_exit(pmu_bat_exit);

MODULE_AUTHOR("David Woodhouse <dwmw2@infradead.org>");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("PMU battery driver");
