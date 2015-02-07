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
 * support for probing IDE PCI devices in the PCI bus order
 *
 * Copyright (c) 1998-2000  Andre Hedrick <andre@linux-ide.org>
 * Copyright (c) 1995-1998  Mark Lord
 *
 * May be copied or modified under the terms of the GNU General Public License
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/ide.h>

/*
 *	Module interfaces
 */

static int pre_init = 1;		/* Before first ordered IDE scan */
static LIST_HEAD(ide_pci_drivers);

/*
 *	__ide_pci_register_driver	-	attach IDE driver
 *	@driver: pci driver
 *	@module: owner module of the driver
 *
 *	Registers a driver with the IDE layer. The IDE layer arranges that
 *	boot time setup is done in the expected device order and then
 *	hands the controllers off to the core PCI code to do the rest of
 *	the work.
 *
 *	Returns are the same as for pci_register_driver
 */

int __ide_pci_register_driver(struct pci_driver *driver, struct module *module,
			      const char *mod_name)
{
	if (!pre_init)
		return __pci_register_driver(driver, module, mod_name);
	driver->driver.owner = module;
	list_add_tail(&driver->node, &ide_pci_drivers);
	return 0;
}
EXPORT_SYMBOL_GPL(__ide_pci_register_driver);

/**
 *	ide_scan_pcidev		-	find an IDE driver for a device
 *	@dev: PCI device to check
 *
 *	Look for an IDE driver to handle the device we are considering.
 *	This is only used during boot up to get the ordering correct. After
 *	boot up the pci layer takes over the job.
 */

static int __init ide_scan_pcidev(struct pci_dev *dev)
{
	struct list_head *l;
	struct pci_driver *d;

	list_for_each(l, &ide_pci_drivers) {
		d = list_entry(l, struct pci_driver, node);
		if (d->id_table) {
			const struct pci_device_id *id =
				pci_match_id(d->id_table, dev);

			if (id != NULL && d->probe(dev, id) >= 0) {
				dev->driver = d;
				pci_dev_get(dev);
				return 1;
			}
		}
	}
	return 0;
}

/**
 *	ide_scan_pcibus		-	perform the initial IDE driver scan
 *
 *	Perform the initial bus rather than driver ordered scan of the
 *	PCI drivers. After this all IDE pci handling becomes standard
 *	module ordering not traditionally ordered.
 */

static int __init ide_scan_pcibus(void)
{
	struct pci_dev *dev = NULL;
	struct pci_driver *d;
	struct list_head *l, *n;

	pre_init = 0;
	while ((dev = pci_get_device(PCI_ANY_ID, PCI_ANY_ID, dev)))
		ide_scan_pcidev(dev);

	/*
	 *	Hand the drivers over to the PCI layer now we
	 *	are post init.
	 */

	list_for_each_safe(l, n, &ide_pci_drivers) {
		list_del(l);
		d = list_entry(l, struct pci_driver, node);
		if (__pci_register_driver(d, d->driver.owner,
					  d->driver.mod_name))
			printk(KERN_ERR "%s: failed to register %s driver\n",
					__func__, d->driver.mod_name);
	}

	return 0;
}

module_init(ide_scan_pcibus);
