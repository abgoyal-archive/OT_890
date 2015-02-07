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
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * SNI specific PCI support for RM200/RM300.
 *
 * Copyright (C) 1997 - 2000, 2003 Ralf Baechle <ralf@linux-mips.org>
 */
#include <linux/kernel.h>
#include <linux/pci.h>
#include <linux/types.h>
#include <asm/sni.h>

/*
 * It seems that on the RM200 only lower 3 bits of the 5 bit PCI device
 * address are decoded.  We therefore manually have to reject attempts at
 * reading outside this range.  Being on the paranoid side we only do this
 * test for bus 0 and hope forwarding and decoding work properly for any
 * subordinated busses.
 *
 * ASIC PCI only supports type 1 config cycles.
 */
static int set_config_address(unsigned int busno, unsigned int devfn, int reg)
{
	if ((devfn > 255) || (reg > 255))
		return PCIBIOS_BAD_REGISTER_NUMBER;

	if (busno == 0 && devfn >= PCI_DEVFN(8, 0))
		return PCIBIOS_DEVICE_NOT_FOUND;

	*(volatile u32 *)PCIMT_CONFIG_ADDRESS =
		 ((busno    & 0xff) << 16) |
	         ((devfn    & 0xff) <<  8) |
	          (reg      & 0xfc);

	return PCIBIOS_SUCCESSFUL;
}

static int pcimt_read(struct pci_bus *bus, unsigned int devfn, int reg,
		      int size, u32 * val)
{
	int res;

	if ((res = set_config_address(bus->number, devfn, reg)))
		return res;

	switch (size) {
	case 1:
		*val = inb(PCIMT_CONFIG_DATA + (reg & 3));
		break;
	case 2:
		*val = inw(PCIMT_CONFIG_DATA + (reg & 2));
		break;
	case 4:
		*val = inl(PCIMT_CONFIG_DATA);
		break;
	}

	return 0;
}

static int pcimt_write(struct pci_bus *bus, unsigned int devfn, int reg,
		       int size, u32 val)
{
	int res;

	if ((res = set_config_address(bus->number, devfn, reg)))
		return res;

	switch (size) {
	case 1:
		outb(val, PCIMT_CONFIG_DATA + (reg & 3));
		break;
	case 2:
		outw(val, PCIMT_CONFIG_DATA + (reg & 2));
		break;
	case 4:
		outl(val, PCIMT_CONFIG_DATA);
		break;
	}

	return 0;
}

struct pci_ops sni_pcimt_ops = {
	.read = pcimt_read,
	.write = pcimt_write,
};

static int pcit_set_config_address(unsigned int busno, unsigned int devfn, int reg)
{
	if ((devfn > 255) || (reg > 255) || (busno > 255))
		return PCIBIOS_BAD_REGISTER_NUMBER;

	outl((1 << 31) | ((busno & 0xff) << 16) | ((devfn & 0xff) << 8) | (reg & 0xfc), 0xcf8);
	return PCIBIOS_SUCCESSFUL;
}

static int pcit_read(struct pci_bus *bus, unsigned int devfn, int reg,
		      int size, u32 * val)
{
	int res;

	/*
	 * on bus 0 we need to check, whether there is a device answering
	 * for the devfn by doing a config write and checking the result. If
	 * we don't do it, we will get a data bus error
	 */
	if (bus->number == 0) {
		pcit_set_config_address(0, 0, 0x68);
		outl(inl(0xcfc) | 0xc0000000, 0xcfc);
		if ((res = pcit_set_config_address(0, devfn, 0)))
			return res;
		outl(0xffffffff, 0xcfc);
		pcit_set_config_address(0, 0, 0x68);
		if (inl(0xcfc) & 0x100000)
			return PCIBIOS_DEVICE_NOT_FOUND;
	}
	if ((res = pcit_set_config_address(bus->number, devfn, reg)))
		return res;

	switch (size) {
	case 1:
		*val = inb(PCIMT_CONFIG_DATA + (reg & 3));
		break;
	case 2:
		*val = inw(PCIMT_CONFIG_DATA + (reg & 2));
		break;
	case 4:
		*val = inl(PCIMT_CONFIG_DATA);
		break;
	}
	return 0;
}

static int pcit_write(struct pci_bus *bus, unsigned int devfn, int reg,
		       int size, u32 val)
{
	int res;

	if ((res = pcit_set_config_address(bus->number, devfn, reg)))
		return res;

	switch (size) {
	case 1:
		outb(val, PCIMT_CONFIG_DATA + (reg & 3));
		break;
	case 2:
		outw(val, PCIMT_CONFIG_DATA + (reg & 2));
		break;
	case 4:
		outl(val, PCIMT_CONFIG_DATA);
		break;
	}

	return 0;
}


struct pci_ops sni_pcit_ops = {
	.read = pcit_read,
	.write = pcit_write,
};
