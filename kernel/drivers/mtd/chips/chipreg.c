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
 * Registration for chip drivers
 *
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kmod.h>
#include <linux/spinlock.h>
#include <linux/slab.h>
#include <linux/mtd/map.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/compatmac.h>

static DEFINE_SPINLOCK(chip_drvs_lock);
static LIST_HEAD(chip_drvs_list);

void register_mtd_chip_driver(struct mtd_chip_driver *drv)
{
	spin_lock(&chip_drvs_lock);
	list_add(&drv->list, &chip_drvs_list);
	spin_unlock(&chip_drvs_lock);
}

void unregister_mtd_chip_driver(struct mtd_chip_driver *drv)
{
	spin_lock(&chip_drvs_lock);
	list_del(&drv->list);
	spin_unlock(&chip_drvs_lock);
}

static struct mtd_chip_driver *get_mtd_chip_driver (const char *name)
{
	struct list_head *pos;
	struct mtd_chip_driver *ret = NULL, *this;

	spin_lock(&chip_drvs_lock);

	list_for_each(pos, &chip_drvs_list) {
		this = list_entry(pos, typeof(*this), list);

		if (!strcmp(this->name, name)) {
			ret = this;
			break;
		}
	}
	if (ret && !try_module_get(ret->module))
		ret = NULL;

	spin_unlock(&chip_drvs_lock);

	return ret;
}

	/* Hide all the horrid details, like some silly person taking
	   get_module_symbol() away from us, from the caller. */

struct mtd_info *do_map_probe(const char *name, struct map_info *map)
{
	struct mtd_chip_driver *drv;
	struct mtd_info *ret;

	drv = get_mtd_chip_driver(name);

	if (!drv && !request_module("%s", name))
		drv = get_mtd_chip_driver(name);

	if (!drv)
		return NULL;

	ret = drv->probe(map);

	/* We decrease the use count here. It may have been a
	   probe-only module, which is no longer required from this
	   point, having given us a handle on (and increased the use
	   count of) the actual driver code.
	*/
	module_put(drv->module);

	if (ret)
		return ret;

	return NULL;
}
/*
 * Destroy an MTD device which was created for a map device.
 * Make sure the MTD device is already unregistered before calling this
 */
void map_destroy(struct mtd_info *mtd)
{
	struct map_info *map = mtd->priv;

	if (map->fldrv->destroy)
		map->fldrv->destroy(mtd);

	module_put(map->fldrv->module);

	kfree(mtd);
}

EXPORT_SYMBOL(register_mtd_chip_driver);
EXPORT_SYMBOL(unregister_mtd_chip_driver);
EXPORT_SYMBOL(do_map_probe);
EXPORT_SYMBOL(map_destroy);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("David Woodhouse <dwmw2@infradead.org>");
MODULE_DESCRIPTION("Core routines for registering and invoking MTD chip drivers");
