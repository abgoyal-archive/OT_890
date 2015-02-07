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
 *  arch/mips/pci/fixup-emma2rh.c
 *      This file defines the PCI configration.
 *
 *  Copyright (C) NEC Electronics Corporation 2004-2006
 *
 *  This file is based on the arch/mips/ddb5xxx/ddb5477/pci.c
 *
 *	Copyright 2001 MontaVista Software Inc.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/pci.h>

#include <asm/bootinfo.h>

#include <asm/emma/emma2rh.h>

#define EMMA2RH_PCI_HOST_SLOT 0x09
#define EMMA2RH_USB_SLOT 0x03
#define PCI_DEVICE_ID_NEC_EMMA2RH      0x014b /* EMMA2RH PCI Host */

/*
 * we fix up irqs based on the slot number.
 * The first entry is at AD:11.
 * Fortunately this works because, although we have two pci buses,
 * they all have different slot numbers (except for rockhopper slot 20
 * which is handled below).
 *
 */

#define	MAX_SLOT_NUM 10
static unsigned char irq_map[][5] __initdata = {
	[3] = {0, MARKEINS_PCI_IRQ_INTB, MARKEINS_PCI_IRQ_INTC,
	       MARKEINS_PCI_IRQ_INTD, 0,},
	[4] = {0, MARKEINS_PCI_IRQ_INTA, 0, 0, 0,},
	[5] = {0, 0, 0, 0, 0,},
	[6] = {0, MARKEINS_PCI_IRQ_INTC, MARKEINS_PCI_IRQ_INTD,
	       MARKEINS_PCI_IRQ_INTA, MARKEINS_PCI_IRQ_INTB,},
};

static void __devinit nec_usb_controller_fixup(struct pci_dev *dev)
{
	if (PCI_SLOT(dev->devfn) == EMMA2RH_USB_SLOT)
		/* on board USB controller configuration */
		pci_write_config_dword(dev, 0xe4, 1 << 5);
}

DECLARE_PCI_FIXUP_FINAL(PCI_VENDOR_ID_NEC, PCI_DEVICE_ID_NEC_USB,
			nec_usb_controller_fixup);

/*
 * Prevent the PCI layer from seeing the resources allocated to this device
 * if it is the host bridge by marking it as such.  These resources are of
 * no consequence to the PCI layer (they are handled elsewhere).
 */
static void __devinit emma2rh_pci_host_fixup(struct pci_dev *dev)
{
	int i;

	if (PCI_SLOT(dev->devfn) == EMMA2RH_PCI_HOST_SLOT) {
		dev->class &= 0xff;
		dev->class |= PCI_CLASS_BRIDGE_HOST << 8;
		for (i = 0; i < PCI_NUM_RESOURCES; i++) {
			dev->resource[i].start = 0;
			dev->resource[i].end = 0;
			dev->resource[i].flags = 0;
		}
	}
}

DECLARE_PCI_FIXUP_HEADER(PCI_VENDOR_ID_NEC, PCI_DEVICE_ID_NEC_EMMA2RH,
			 emma2rh_pci_host_fixup);

int __init pcibios_map_irq(const struct pci_dev *dev, u8 slot, u8 pin)
{
	return irq_map[slot][pin];
}

/* Do platform specific device initialization at pci_enable_device() time */
int pcibios_plat_dev_init(struct pci_dev *dev)
{
	return 0;
}
