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
 * Support for indirect PCI bridges.
 *
 * Copyright (C) 1998 Gabriel Paubert.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#include <linux/kernel.h>
#include <linux/pci.h>
#include <linux/delay.h>
#include <linux/string.h>
#include <linux/init.h>

#include <asm/io.h>
#include <asm/prom.h>
#include <asm/pci-bridge.h>
#include <asm/machdep.h>

static int
indirect_read_config(struct pci_bus *bus, unsigned int devfn, int offset,
		     int len, u32 *val)
{
	struct pci_controller *hose = bus->sysdata;
	volatile void __iomem *cfg_data;
	u8 cfg_type = 0;
	u32 bus_no, reg;

	if (hose->indirect_type & PPC_INDIRECT_TYPE_NO_PCIE_LINK) {
		if (bus->number != hose->first_busno)
			return PCIBIOS_DEVICE_NOT_FOUND;
		if (devfn != 0)
			return PCIBIOS_DEVICE_NOT_FOUND;
	}

	if (ppc_md.pci_exclude_device)
		if (ppc_md.pci_exclude_device(hose, bus->number, devfn))
			return PCIBIOS_DEVICE_NOT_FOUND;

	if (hose->indirect_type & PPC_INDIRECT_TYPE_SET_CFG_TYPE)
		if (bus->number != hose->first_busno)
			cfg_type = 1;

	bus_no = (bus->number == hose->first_busno) ?
			hose->self_busno : bus->number;

	if (hose->indirect_type & PPC_INDIRECT_TYPE_EXT_REG)
		reg = ((offset & 0xf00) << 16) | (offset & 0xfc);
	else
		reg = offset & 0xfc;

	if (hose->indirect_type & PPC_INDIRECT_TYPE_BIG_ENDIAN)
		out_be32(hose->cfg_addr, (0x80000000 | (bus_no << 16) |
			 (devfn << 8) | reg | cfg_type));
	else
		out_le32(hose->cfg_addr, (0x80000000 | (bus_no << 16) |
			 (devfn << 8) | reg | cfg_type));

	/*
	 * Note: the caller has already checked that offset is
	 * suitably aligned and that len is 1, 2 or 4.
	 */
	cfg_data = hose->cfg_data + (offset & 3);
	switch (len) {
	case 1:
		*val = in_8(cfg_data);
		break;
	case 2:
		*val = in_le16(cfg_data);
		break;
	default:
		*val = in_le32(cfg_data);
		break;
	}
	return PCIBIOS_SUCCESSFUL;
}

static int
indirect_write_config(struct pci_bus *bus, unsigned int devfn, int offset,
		      int len, u32 val)
{
	struct pci_controller *hose = bus->sysdata;
	volatile void __iomem *cfg_data;
	u8 cfg_type = 0;
	u32 bus_no, reg;

	if (hose->indirect_type & PPC_INDIRECT_TYPE_NO_PCIE_LINK) {
		if (bus->number != hose->first_busno)
			return PCIBIOS_DEVICE_NOT_FOUND;
		if (devfn != 0)
			return PCIBIOS_DEVICE_NOT_FOUND;
	}

	if (ppc_md.pci_exclude_device)
		if (ppc_md.pci_exclude_device(hose, bus->number, devfn))
			return PCIBIOS_DEVICE_NOT_FOUND;

	if (hose->indirect_type & PPC_INDIRECT_TYPE_SET_CFG_TYPE)
		if (bus->number != hose->first_busno)
			cfg_type = 1;

	bus_no = (bus->number == hose->first_busno) ?
			hose->self_busno : bus->number;

	if (hose->indirect_type & PPC_INDIRECT_TYPE_EXT_REG)
		reg = ((offset & 0xf00) << 16) | (offset & 0xfc);
	else
		reg = offset & 0xfc;

	if (hose->indirect_type & PPC_INDIRECT_TYPE_BIG_ENDIAN)
		out_be32(hose->cfg_addr, (0x80000000 | (bus_no << 16) |
			 (devfn << 8) | reg | cfg_type));
	else
		out_le32(hose->cfg_addr, (0x80000000 | (bus_no << 16) |
			 (devfn << 8) | reg | cfg_type));

	/* surpress setting of PCI_PRIMARY_BUS */
	if (hose->indirect_type & PPC_INDIRECT_TYPE_SURPRESS_PRIMARY_BUS)
		if ((offset == PCI_PRIMARY_BUS) &&
			(bus->number == hose->first_busno))
		val &= 0xffffff00;

	/* Workaround for PCI_28 Errata in 440EPx/GRx */
	if ((hose->indirect_type & PPC_INDIRECT_TYPE_BROKEN_MRM) &&
			offset == PCI_CACHE_LINE_SIZE) {
		val = 0;
	}

	/*
	 * Note: the caller has already checked that offset is
	 * suitably aligned and that len is 1, 2 or 4.
	 */
	cfg_data = hose->cfg_data + (offset & 3);
	switch (len) {
	case 1:
		out_8(cfg_data, val);
		break;
	case 2:
		out_le16(cfg_data, val);
		break;
	default:
		out_le32(cfg_data, val);
		break;
	}
	return PCIBIOS_SUCCESSFUL;
}

static struct pci_ops indirect_pci_ops =
{
	.read = indirect_read_config,
	.write = indirect_write_config,
};

void __init
setup_indirect_pci(struct pci_controller* hose,
		   resource_size_t cfg_addr,
		   resource_size_t cfg_data, u32 flags)
{
	resource_size_t base = cfg_addr & PAGE_MASK;
	void __iomem *mbase;

	mbase = ioremap(base, PAGE_SIZE);
	hose->cfg_addr = mbase + (cfg_addr & ~PAGE_MASK);
	if ((cfg_data & PAGE_MASK) != base)
		mbase = ioremap(cfg_data & PAGE_MASK, PAGE_SIZE);
	hose->cfg_data = mbase + (cfg_data & ~PAGE_MASK);
	hose->ops = &indirect_pci_ops;
	hose->indirect_type = flags;
}
