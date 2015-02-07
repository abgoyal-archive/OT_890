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
 * File:	portdrv_bus.c
 * Purpose:	PCI Express Port Bus Driver's Bus Overloading Functions
 *
 * Copyright (C) 2004 Intel
 * Copyright (C) Tom Long Nguyen (tom.l.nguyen@intel.com)
 */

#include <linux/module.h>
#include <linux/pci.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/pm.h>

#include <linux/pcieport_if.h>
#include "portdrv.h"

static int pcie_port_bus_match(struct device *dev, struct device_driver *drv);

struct bus_type pcie_port_bus_type = {
	.name 		= "pci_express",
	.match 		= pcie_port_bus_match,
};
EXPORT_SYMBOL_GPL(pcie_port_bus_type);

static int pcie_port_bus_match(struct device *dev, struct device_driver *drv)
{
	struct pcie_device *pciedev;
	struct pcie_port_service_driver *driver;

	if (drv->bus != &pcie_port_bus_type || dev->bus != &pcie_port_bus_type)
		return 0;
	
	pciedev = to_pcie_device(dev);
	driver = to_service_driver(drv);
	if (   (driver->id_table->vendor != PCI_ANY_ID && 
		driver->id_table->vendor != pciedev->id.vendor) ||
	       (driver->id_table->device != PCI_ANY_ID &&
		driver->id_table->device != pciedev->id.device) ||	
	       (driver->id_table->port_type != PCIE_ANY_PORT &&
		driver->id_table->port_type != pciedev->id.port_type) ||
		driver->id_table->service_type != pciedev->id.service_type )
		return 0;

	return 1;
}

int pcie_port_bus_register(void)
{
	return bus_register(&pcie_port_bus_type);
}

void pcie_port_bus_unregister(void)
{
	bus_unregister(&pcie_port_bus_type);
}
