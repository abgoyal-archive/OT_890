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
 * Compaq Hot Plug Controller Driver
 *
 * Copyright (c) 1995,2001 Compaq Computer Corporation
 * Copyright (c) 2001,2003 Greg Kroah-Hartman (greg@kroah.com)
 * Copyright (c) 2001 IBM Corp.
 *
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, GOOD TITLE or
 * NON INFRINGEMENT.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * Send feedback to <greg@kroah.com>
 *
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/pci.h>
#include "shpchp.h"


/* A few routines that create sysfs entries for the hot plug controller */

static ssize_t show_ctrl (struct device *dev, struct device_attribute *attr, char *buf)
{
	struct pci_dev *pdev;
	char * out = buf;
	int index, busnr;
	struct resource *res;
	struct pci_bus *bus;

	pdev = container_of (dev, struct pci_dev, dev);
	bus = pdev->subordinate;

	out += sprintf(buf, "Free resources: memory\n");
	for (index = 0; index < PCI_BUS_NUM_RESOURCES; index++) {
		res = bus->resource[index];
		if (res && (res->flags & IORESOURCE_MEM) &&
				!(res->flags & IORESOURCE_PREFETCH)) {
			out += sprintf(out, "start = %8.8llx, "
					"length = %8.8llx\n",
					(unsigned long long)res->start,
					(unsigned long long)(res->end - res->start));
		}
	}
	out += sprintf(out, "Free resources: prefetchable memory\n");
	for (index = 0; index < PCI_BUS_NUM_RESOURCES; index++) {
		res = bus->resource[index];
		if (res && (res->flags & IORESOURCE_MEM) &&
			       (res->flags & IORESOURCE_PREFETCH)) {
			out += sprintf(out, "start = %8.8llx, "
					"length = %8.8llx\n",
					(unsigned long long)res->start,
					(unsigned long long)(res->end - res->start));
		}
	}
	out += sprintf(out, "Free resources: IO\n");
	for (index = 0; index < PCI_BUS_NUM_RESOURCES; index++) {
		res = bus->resource[index];
		if (res && (res->flags & IORESOURCE_IO)) {
			out += sprintf(out, "start = %8.8llx, "
					"length = %8.8llx\n",
					(unsigned long long)res->start,
					(unsigned long long)(res->end - res->start));
		}
	}
	out += sprintf(out, "Free resources: bus numbers\n");
	for (busnr = bus->secondary; busnr <= bus->subordinate; busnr++) {
		if (!pci_find_bus(pci_domain_nr(bus), busnr))
			break;
	}
	if (busnr < bus->subordinate)
		out += sprintf(out, "start = %8.8x, length = %8.8x\n",
				busnr, (bus->subordinate - busnr));

	return out - buf;
}
static DEVICE_ATTR (ctrl, S_IRUGO, show_ctrl, NULL);

int __must_check shpchp_create_ctrl_files (struct controller *ctrl)
{
	return device_create_file (&ctrl->pci_dev->dev, &dev_attr_ctrl);
}

void shpchp_remove_ctrl_files(struct controller *ctrl)
{
	device_remove_file(&ctrl->pci_dev->dev, &dev_attr_ctrl);
}
