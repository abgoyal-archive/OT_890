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
 *
 *  BRIEF MODULE DESCRIPTION
 *
 *  2.6 port, Embedded Alley Solutions, Inc
 *
 *  Based on:
 *  Author: source@mvista.com
 *
 *  This program is free software; you can distribute it and/or modify it
 *  under the terms of the GNU General Public License (Version 2) as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place - Suite 330, Boston MA 02111-1307, USA.
 */
#include <linux/types.h>
#include <linux/pci.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/vmalloc.h>
#include <linux/delay.h>

#include <asm/mach-pnx8550/pci.h>
#include <asm/mach-pnx8550/glb.h>

static inline void clear_status(void)
{
	unsigned long pci_stat;

	pci_stat = inl(PCI_BASE | PCI_GPPM_STATUS);
	outl(pci_stat, PCI_BASE | PCI_GPPM_ICLR);
}

static inline unsigned int
calc_cfg_addr(struct pci_bus *bus, unsigned int devfn, int where)
{
	unsigned int addr;

	addr = ((bus->number > 0) ? (((bus->number & 0xff) << PCI_CFG_BUS_SHIFT) | 1) : 0);
	addr |= ((devfn & 0xff) << PCI_CFG_FUNC_SHIFT) | (where & 0xfc);

	return addr;
}

static int
config_access(unsigned int pci_cmd, struct pci_bus *bus, unsigned int devfn, int where, unsigned int pci_mode, unsigned int *val)
{
	unsigned int flags;
	unsigned long loops = 0;
	unsigned long ioaddr = calc_cfg_addr(bus, devfn, where);

	local_irq_save(flags);
	/*Clear pending interrupt status */
	if (inl(PCI_BASE | PCI_GPPM_STATUS)) {
		clear_status();
		while (!(inl(PCI_BASE | PCI_GPPM_STATUS) == 0)) ;
	}

	outl(ioaddr, PCI_BASE | PCI_GPPM_ADDR);

	if ((pci_cmd == PCI_CMD_IOW) || (pci_cmd == PCI_CMD_CONFIG_WRITE))
		outl(*val, PCI_BASE | PCI_GPPM_WDAT);

	outl(INIT_PCI_CYCLE | pci_cmd | (pci_mode & PCI_BYTE_ENABLE_MASK),
	     PCI_BASE | PCI_GPPM_CTRL);

	loops =
	    ((loops_per_jiffy *
	      PCI_IO_JIFFIES_TIMEOUT) >> (PCI_IO_JIFFIES_SHIFT));
	while (1) {
		if (inl(PCI_BASE | PCI_GPPM_STATUS) & GPPM_DONE) {
			if ((pci_cmd == PCI_CMD_IOR) ||
			    (pci_cmd == PCI_CMD_CONFIG_READ))
				*val = inl(PCI_BASE | PCI_GPPM_RDAT);
			clear_status();
			local_irq_restore(flags);
			return PCIBIOS_SUCCESSFUL;
		} else if (inl(PCI_BASE | PCI_GPPM_STATUS) & GPPM_R_MABORT) {
			break;
		}

		loops--;
		if (loops == 0) {
			printk("%s : Arbiter Locked.\n", __func__);
		}
	}

	clear_status();
	if ((pci_cmd == PCI_CMD_IOR) || (pci_cmd == PCI_CMD_IOW)) {
		printk("%s timeout (GPPM_CTRL=%X) ioaddr %lX pci_cmd %X\n",
		       __func__, inl(PCI_BASE | PCI_GPPM_CTRL), ioaddr,
		       pci_cmd);
	}

	if ((pci_cmd == PCI_CMD_IOR) || (pci_cmd == PCI_CMD_CONFIG_READ))
		*val = 0xffffffff;
	local_irq_restore(flags);
	return PCIBIOS_DEVICE_NOT_FOUND;
}

/*
 * We can't address 8 and 16 bit words directly.  Instead we have to
 * read/write a 32bit word and mask/modify the data we actually want.
 */
static int
read_config_byte(struct pci_bus *bus, unsigned int devfn, int where, u8 * val)
{
	unsigned int data = 0;
	int err;

	if (bus == NULL)
		return -1;

	err = config_access(PCI_CMD_CONFIG_READ, bus, devfn, where, ~(1 << (where & 3)), &data);
	switch (where & 0x03) {
	case 0:
		*val = (unsigned char)(data & 0x000000ff);
		break;
	case 1:
		*val = (unsigned char)((data & 0x0000ff00) >> 8);
		break;
	case 2:
		*val = (unsigned char)((data & 0x00ff0000) >> 16);
		break;
	case 3:
		*val = (unsigned char)((data & 0xff000000) >> 24);
		break;
	}

	return err;
}

static int
read_config_word(struct pci_bus *bus, unsigned int devfn, int where, u16 * val)
{
	unsigned int data = 0;
	int err;

	if (bus == NULL)
		return -1;

	if (where & 0x01)
		return PCIBIOS_BAD_REGISTER_NUMBER;

	err = config_access(PCI_CMD_CONFIG_READ, bus, devfn, where, ~(3 << (where & 3)), &data);
	switch (where & 0x02) {
	case 0:
		*val = (unsigned short)(data & 0x0000ffff);
		break;
	case 2:
		*val = (unsigned short)((data & 0xffff0000) >> 16);
		break;
	}

	return err;
}

static int
read_config_dword(struct pci_bus *bus, unsigned int devfn, int where, u32 * val)
{
	int err;
	if (bus == NULL)
		return -1;

	if (where & 0x03)
		return PCIBIOS_BAD_REGISTER_NUMBER;

	err = config_access(PCI_CMD_CONFIG_READ, bus, devfn, where, 0, val);

	return err;
}

static int
write_config_byte(struct pci_bus *bus, unsigned int devfn, int where, u8 val)
{
	unsigned int data = (unsigned int)val;
	int err;

	if (bus == NULL)
		return -1;

	switch (where & 0x03) {
	case 1:
		data = (data << 8);
		break;
	case 2:
		data = (data << 16);
		break;
	case 3:
		data = (data << 24);
		break;
	default:
		break;
	}

	err = config_access(PCI_CMD_CONFIG_WRITE, bus, devfn, where, ~(1 << (where & 3)), &data);

	return err;
}

static int
write_config_word(struct pci_bus *bus, unsigned int devfn, int where, u16 val)
{
	unsigned int data = (unsigned int)val;
	int err;

	if (bus == NULL)
		return -1;

	if (where & 0x01)
		return PCIBIOS_BAD_REGISTER_NUMBER;

	switch (where & 0x02) {
	case 2:
		data = (data << 16);
		break;
	default:
		break;
	}
	err = config_access(PCI_CMD_CONFIG_WRITE, bus, devfn, where, ~(3 << (where & 3)), &data);

	return err;
}

static int
write_config_dword(struct pci_bus *bus, unsigned int devfn, int where, u32 val)
{
	int err;
	if (bus == NULL)
		return -1;

	if (where & 0x03)
		return PCIBIOS_BAD_REGISTER_NUMBER;

	err = config_access(PCI_CMD_CONFIG_WRITE, bus, devfn, where, 0, &val);

	return err;
}

static int config_read(struct pci_bus *bus, unsigned int devfn, int where, int size, u32 * val)
{
	switch (size) {
	case 1: {
			u8 _val;
			int rc = read_config_byte(bus, devfn, where, &_val);
			*val = _val;
			return rc;
		}
       case 2: {
			u16 _val;
			int rc = read_config_word(bus, devfn, where, &_val);
			*val = _val;
			return rc;
		}
	default:
		return read_config_dword(bus, devfn, where, val);
	}
}

static int config_write(struct pci_bus *bus, unsigned int devfn, int where, int size, u32 val)
{
	switch (size) {
	case 1:
		return write_config_byte(bus, devfn, where, (u8) val);
	case 2:
		return write_config_word(bus, devfn, where, (u16) val);
	default:
		return write_config_dword(bus, devfn, where, val);
	}
}

struct pci_ops pnx8550_pci_ops = {
	config_read,
	config_write
};
