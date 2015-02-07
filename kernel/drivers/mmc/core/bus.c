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
 *  linux/drivers/mmc/core/bus.c
 *
 *  Copyright (C) 2003 Russell King, All Rights Reserved.
 *  Copyright (C) 2007 Pierre Ossman
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  MMC card bus driver model
 */

#include <linux/device.h>
#include <linux/err.h>

#include <linux/mmc/card.h>
#include <linux/mmc/host.h>

#include "core.h"
#include "sdio_cis.h"
#include "bus.h"

#define dev_to_mmc_card(d)	container_of(d, struct mmc_card, dev)
#define to_mmc_driver(d)	container_of(d, struct mmc_driver, drv)

static ssize_t mmc_type_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct mmc_card *card = dev_to_mmc_card(dev);

	switch (card->type) {
	case MMC_TYPE_MMC:
		return sprintf(buf, "MMC\n");
	case MMC_TYPE_SD:
		return sprintf(buf, "SD\n");
	case MMC_TYPE_SDIO:
		return sprintf(buf, "SDIO\n");
	default:
		return -EFAULT;
	}
}

static struct device_attribute mmc_dev_attrs[] = {
	__ATTR(type, S_IRUGO, mmc_type_show, NULL),
	__ATTR_NULL,
};

/*
 * This currently matches any MMC driver to any MMC card - drivers
 * themselves make the decision whether to drive this card in their
 * probe method.
 */
static int mmc_bus_match(struct device *dev, struct device_driver *drv)
{
	return 1;
}

static int
mmc_bus_uevent(struct device *dev, struct kobj_uevent_env *env)
{
	struct mmc_card *card = dev_to_mmc_card(dev);
	const char *type;
	int retval = 0;

	switch (card->type) {
	case MMC_TYPE_MMC:
		type = "MMC";
		break;
	case MMC_TYPE_SD:
		type = "SD";
		break;
	case MMC_TYPE_SDIO:
		type = "SDIO";
		break;
	default:
		type = NULL;
	}

	if (type) {
		retval = add_uevent_var(env, "MMC_TYPE=%s", type);
		if (retval)
			return retval;
	}

	retval = add_uevent_var(env, "MMC_NAME=%s", mmc_card_name(card));
	if (retval)
		return retval;

	/*
	 * Request the mmc_block device.  Note: that this is a direct request
	 * for the module it carries no information as to what is inserted.
	 */
	retval = add_uevent_var(env, "MODALIAS=mmc:block");

	return retval;
}

static int mmc_bus_probe(struct device *dev)
{
	struct mmc_driver *drv = to_mmc_driver(dev->driver);
	struct mmc_card *card = dev_to_mmc_card(dev);

	return drv->probe(card);
}

static int mmc_bus_remove(struct device *dev)
{
	struct mmc_driver *drv = to_mmc_driver(dev->driver);
	struct mmc_card *card = dev_to_mmc_card(dev);

	drv->remove(card);

	return 0;
}

static int mmc_bus_suspend(struct device *dev, pm_message_t state)
{
	struct mmc_driver *drv = to_mmc_driver(dev->driver);
	struct mmc_card *card = dev_to_mmc_card(dev);
	int ret = 0;

	if (dev->driver && drv->suspend)
		ret = drv->suspend(card, state);
	return ret;
}

static int mmc_bus_resume(struct device *dev)
{
	struct mmc_driver *drv = to_mmc_driver(dev->driver);
	struct mmc_card *card = dev_to_mmc_card(dev);
	int ret = 0;

	if (dev->driver && drv->resume)
		ret = drv->resume(card);
	return ret;
}

static struct bus_type mmc_bus_type = {
	.name		= "mmc",
	.dev_attrs	= mmc_dev_attrs,
	.match		= mmc_bus_match,
	.uevent		= mmc_bus_uevent,
	.probe		= mmc_bus_probe,
	.remove		= mmc_bus_remove,
	.suspend	= mmc_bus_suspend,
	.resume		= mmc_bus_resume,
};

int mmc_register_bus(void)
{
	return bus_register(&mmc_bus_type);
}

void mmc_unregister_bus(void)
{
	bus_unregister(&mmc_bus_type);
}

/**
 *	mmc_register_driver - register a media driver
 *	@drv: MMC media driver
 */
int mmc_register_driver(struct mmc_driver *drv)
{
	drv->drv.bus = &mmc_bus_type;
	return driver_register(&drv->drv);
}

EXPORT_SYMBOL(mmc_register_driver);

/**
 *	mmc_unregister_driver - unregister a media driver
 *	@drv: MMC media driver
 */
void mmc_unregister_driver(struct mmc_driver *drv)
{
	drv->drv.bus = &mmc_bus_type;
	driver_unregister(&drv->drv);
}

EXPORT_SYMBOL(mmc_unregister_driver);

static void mmc_release_card(struct device *dev)
{
	struct mmc_card *card = dev_to_mmc_card(dev);

	sdio_free_common_cis(card);

	if (card->info)
		kfree(card->info);

	kfree(card);
}

/*
 * Allocate and initialise a new MMC card structure.
 */
struct mmc_card *mmc_alloc_card(struct mmc_host *host, struct device_type *type)
{
	struct mmc_card *card;

	card = kzalloc(sizeof(struct mmc_card), GFP_KERNEL);
	if (!card)
		return ERR_PTR(-ENOMEM);

	card->host = host;

	device_initialize(&card->dev);

	card->dev.parent = mmc_classdev(host);
	card->dev.bus = &mmc_bus_type;
	card->dev.release = mmc_release_card;
	card->dev.type = type;

	return card;
}

/*
 * Register a new MMC card with the driver model.
 */
int mmc_add_card(struct mmc_card *card)
{
	int ret;
	const char *type;

	dev_set_name(&card->dev, "%s:%04x", mmc_hostname(card->host), card->rca);

	switch (card->type) {
	case MMC_TYPE_MMC:
		type = "MMC";
		break;
	case MMC_TYPE_SD:
		type = "SD";
		if (mmc_card_blockaddr(card))
			type = "SDHC";
		break;
	case MMC_TYPE_SDIO:
		type = "SDIO";
		break;
	default:
		type = "?";
		break;
	}

	if (mmc_host_is_spi(card->host)) {
		printk(KERN_INFO "%s: new %s%s card on SPI\n",
			mmc_hostname(card->host),
			mmc_card_highspeed(card) ? "high speed " : "",
			type);
	} else {
		printk(KERN_INFO "%s: new %s%s card at address %04x\n",
			mmc_hostname(card->host),
			mmc_card_highspeed(card) ? "high speed " : "",
			type, card->rca);
	}

	ret = device_add(&card->dev);
	if (ret)
		return ret;

#ifdef CONFIG_DEBUG_FS
	mmc_add_card_debugfs(card);
#endif

	mmc_card_set_present(card);

	return 0;
}

/*
 * Unregister a new MMC card with the driver model, and
 * (eventually) free it.
 */
void mmc_remove_card(struct mmc_card *card)
{
#ifdef CONFIG_DEBUG_FS
	mmc_remove_card_debugfs(card);
#endif

	if (mmc_card_present(card)) {
		if (mmc_host_is_spi(card->host)) {
			printk(KERN_INFO "%s: SPI card removed\n",
				mmc_hostname(card->host));
		} else {
			printk(KERN_INFO "%s: card %04x removed\n",
				mmc_hostname(card->host), card->rca);
		}
		device_del(&card->dev);
	}

	put_device(&card->dev);
}

