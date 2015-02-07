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
 * system.c - a driver for reserving pnp system resources
 *
 * Some code is based on pnpbios_core.c
 * Copyright 2002 Adam Belay <ambx1@neo.rr.com>
 * (c) Copyright 2007 Hewlett-Packard Development Company, L.P.
 *	Bjorn Helgaas <bjorn.helgaas@hp.com>
 */

#include <linux/pnp.h>
#include <linux/device.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/ioport.h>

static const struct pnp_device_id pnp_dev_table[] = {
	/* General ID for reserving resources */
	{"PNP0c02", 0},
	/* memory controller */
	{"PNP0c01", 0},
	{"", 0}
};

static void reserve_range(struct pnp_dev *dev, resource_size_t start,
			  resource_size_t end, int port)
{
	char *regionid;
	const char *pnpid = dev_name(&dev->dev);
	struct resource *res;

	regionid = kmalloc(16, GFP_KERNEL);
	if (!regionid)
		return;

	snprintf(regionid, 16, "pnp %s", pnpid);
	if (port)
		res = request_region(start, end - start + 1, regionid);
	else
		res = request_mem_region(start, end - start + 1, regionid);
	if (res)
		res->flags &= ~IORESOURCE_BUSY;
	else
		kfree(regionid);

	/*
	 * Failures at this point are usually harmless. pci quirks for
	 * example do reserve stuff they know about too, so we may well
	 * have double reservations.
	 */
	dev_info(&dev->dev, "%s range 0x%llx-0x%llx %s reserved\n",
		port ? "ioport" : "iomem",
		(unsigned long long) start, (unsigned long long) end,
		res ? "has been" : "could not be");
}

static void reserve_resources_of_dev(struct pnp_dev *dev)
{
	struct resource *res;
	int i;

	for (i = 0; (res = pnp_get_resource(dev, IORESOURCE_IO, i)); i++) {
		if (res->flags & IORESOURCE_DISABLED)
			continue;
		if (res->start == 0)
			continue;	/* disabled */
		if (res->start < 0x100)
			/*
			 * Below 0x100 is only standard PC hardware
			 * (pics, kbd, timer, dma, ...)
			 * We should not get resource conflicts there,
			 * and the kernel reserves these anyway
			 * (see arch/i386/kernel/setup.c).
			 * So, do nothing
			 */
			continue;
		if (res->end < res->start)
			continue;	/* invalid */

		reserve_range(dev, res->start, res->end, 1);
	}

	for (i = 0; (res = pnp_get_resource(dev, IORESOURCE_MEM, i)); i++) {
		if (res->flags & IORESOURCE_DISABLED)
			continue;

		reserve_range(dev, res->start, res->end, 0);
	}
}

static int system_pnp_probe(struct pnp_dev *dev,
			    const struct pnp_device_id *dev_id)
{
	reserve_resources_of_dev(dev);
	return 0;
}

static struct pnp_driver system_pnp_driver = {
	.name     = "system",
	.id_table = pnp_dev_table,
	.flags    = PNP_DRIVER_RES_DO_NOT_CHANGE,
	.probe    = system_pnp_probe,
};

static int __init pnp_system_init(void)
{
	return pnp_register_driver(&system_pnp_driver);
}

/**
 * Reserve motherboard resources after PCI claim BARs,
 * but before PCI assign resources for uninitialized PCI devices
 */
fs_initcall(pnp_system_init);
