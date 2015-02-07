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
 *  Zorro Driver Services
 *
 *  Copyright (C) 2003 Geert Uytterhoeven
 *
 *  Loosely based on drivers/pci/pci-driver.c
 *
 *  This file is subject to the terms and conditions of the GNU General Public
 *  License.  See the file COPYING in the main directory of this archive
 *  for more details.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/zorro.h>


    /**
     *  zorro_match_device - Tell if a Zorro device structure has a matching
     *                       Zorro device id structure
     *  @ids: array of Zorro device id structures to search in
     *  @dev: the Zorro device structure to match against
     *
     *  Used by a driver to check whether a Zorro device present in the
     *  system is in its list of supported devices. Returns the matching
     *  zorro_device_id structure or %NULL if there is no match.
     */

const struct zorro_device_id *
zorro_match_device(const struct zorro_device_id *ids,
		   const struct zorro_dev *z)
{
	while (ids->id) {
		if (ids->id == ZORRO_WILDCARD || ids->id == z->id)
			return ids;
		ids++;
	}
	return NULL;
}


static int zorro_device_probe(struct device *dev)
{
	int error = 0;
	struct zorro_driver *drv = to_zorro_driver(dev->driver);
	struct zorro_dev *z = to_zorro_dev(dev);

	if (!z->driver && drv->probe) {
		const struct zorro_device_id *id;

		id = zorro_match_device(drv->id_table, z);
		if (id)
			error = drv->probe(z, id);
		if (error >= 0) {
			z->driver = drv;
			error = 0;
		}
	}
	return error;
}


static int zorro_device_remove(struct device *dev)
{
	struct zorro_dev *z = to_zorro_dev(dev);
	struct zorro_driver *drv = to_zorro_driver(dev->driver);

	if (drv) {
		if (drv->remove)
			drv->remove(z);
		z->driver = NULL;
	}
	return 0;
}


    /**
     *  zorro_register_driver - register a new Zorro driver
     *  @drv: the driver structure to register
     *
     *  Adds the driver structure to the list of registered drivers
     *  Returns zero or a negative error value.
     */

int zorro_register_driver(struct zorro_driver *drv)
{
	/* initialize common driver fields */
	drv->driver.name = drv->name;
	drv->driver.bus = &zorro_bus_type;

	/* register with core */
	return driver_register(&drv->driver);
}


    /**
     *  zorro_unregister_driver - unregister a zorro driver
     *  @drv: the driver structure to unregister
     *
     *  Deletes the driver structure from the list of registered Zorro drivers,
     *  gives it a chance to clean up by calling its remove() function for
     *  each device it was responsible for, and marks those devices as
     *  driverless.
     */

void zorro_unregister_driver(struct zorro_driver *drv)
{
	driver_unregister(&drv->driver);
}


    /**
     *  zorro_bus_match - Tell if a Zorro device structure has a matching Zorro
     *                    device id structure
     *  @ids: array of Zorro device id structures to search in
     *  @dev: the Zorro device structure to match against
     *
     *  Used by a driver to check whether a Zorro device present in the
     *  system is in its list of supported devices.Returns the matching
     *  zorro_device_id structure or %NULL if there is no match.
     */

static int zorro_bus_match(struct device *dev, struct device_driver *drv)
{
	struct zorro_dev *z = to_zorro_dev(dev);
	struct zorro_driver *zorro_drv = to_zorro_driver(drv);
	const struct zorro_device_id *ids = zorro_drv->id_table;

	if (!ids)
		return 0;

	while (ids->id) {
		if (ids->id == ZORRO_WILDCARD || ids->id == z->id)
			return 1;
		ids++;
	}
	return 0;
}


struct bus_type zorro_bus_type = {
	.name	= "zorro",
	.match	= zorro_bus_match,
	.probe	= zorro_device_probe,
	.remove	= zorro_device_remove,
};


static int __init zorro_driver_init(void)
{
	return bus_register(&zorro_bus_type);
}

postcore_initcall(zorro_driver_init);

EXPORT_SYMBOL(zorro_match_device);
EXPORT_SYMBOL(zorro_register_driver);
EXPORT_SYMBOL(zorro_unregister_driver);
EXPORT_SYMBOL(zorro_dev_driver);
EXPORT_SYMBOL(zorro_bus_type);
