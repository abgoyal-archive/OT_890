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
 * This file provides autodetection for ISA PnP IDE interfaces.
 * It was tested with "ESS ES1868 Plug and Play AudioDrive" IDE interface.
 *
 * Copyright (C) 2000 Andrey Panin <pazke@donpac.ru>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * You should have received a copy of the GNU General Public License
 * (for example /usr/src/linux/COPYING); if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <linux/init.h>
#include <linux/pnp.h>
#include <linux/ide.h>

#define DRV_NAME "ide-pnp"

/* Add your devices here :)) */
static struct pnp_device_id idepnp_devices[] = {
	/* Generic ESDI/IDE/ATA compatible hard disk controller */
	{.id = "PNP0600", .driver_data = 0},
	{.id = ""}
};

static int idepnp_probe(struct pnp_dev *dev, const struct pnp_device_id *dev_id)
{
	struct ide_host *host;
	unsigned long base, ctl;
	int rc;
	hw_regs_t hw, *hws[] = { &hw, NULL, NULL, NULL };

	printk(KERN_INFO DRV_NAME ": generic PnP IDE interface\n");

	if (!(pnp_port_valid(dev, 0) && pnp_port_valid(dev, 1) && pnp_irq_valid(dev, 0)))
		return -1;

	base = pnp_port_start(dev, 0);
	ctl = pnp_port_start(dev, 1);

	if (!request_region(base, 8, DRV_NAME)) {
		printk(KERN_ERR "%s: I/O resource 0x%lX-0x%lX not free.\n",
				DRV_NAME, base, base + 7);
		return -EBUSY;
	}

	if (!request_region(ctl, 1, DRV_NAME)) {
		printk(KERN_ERR "%s: I/O resource 0x%lX not free.\n",
				DRV_NAME, ctl);
		release_region(base, 8);
		return -EBUSY;
	}

	memset(&hw, 0, sizeof(hw));
	ide_std_init_ports(&hw, base, ctl);
	hw.irq = pnp_irq(dev, 0);
	hw.chipset = ide_generic;

	rc = ide_host_add(NULL, hws, &host);
	if (rc)
		goto out;

	pnp_set_drvdata(dev, host);

	return 0;
out:
	release_region(ctl, 1);
	release_region(base, 8);

	return rc;
}

static void idepnp_remove(struct pnp_dev *dev)
{
	struct ide_host *host = pnp_get_drvdata(dev);

	ide_host_remove(host);

	release_region(pnp_port_start(dev, 1), 1);
	release_region(pnp_port_start(dev, 0), 8);
}

static struct pnp_driver idepnp_driver = {
	.name		= "ide",
	.id_table	= idepnp_devices,
	.probe		= idepnp_probe,
	.remove		= idepnp_remove,
};

static int __init pnpide_init(void)
{
	return pnp_register_driver(&idepnp_driver);
}

static void __exit pnpide_exit(void)
{
	pnp_unregister_driver(&idepnp_driver);
}

module_init(pnpide_init);
module_exit(pnpide_exit);

MODULE_LICENSE("GPL");
