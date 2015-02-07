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
 * Copyright (C) 2000, 2001 Keith M Wesolowski
 */
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/pci.h>
#include <linux/types.h>
#include <asm/pci.h>
#include <asm/ip32/mace.h>

#if 0
# define DPRINTK(args...) printk(args);
#else
# define DPRINTK(args...)
#endif

/*
 * O2 has up to 5 PCI devices connected into the MACE bridge.  The device
 * map looks like this:
 *
 * 0  aic7xxx 0
 * 1  aic7xxx 1
 * 2  expansion slot
 * 3  N/C
 * 4  N/C
 */

static inline int mkaddr(struct pci_bus *bus, unsigned int devfn,
	unsigned int reg)
{
	return ((bus->number & 0xff) << 16) |
		((devfn & 0xff) << 8) |
		(reg & 0xfc);
}


static int
mace_pci_read_config(struct pci_bus *bus, unsigned int devfn,
		     int reg, int size, u32 *val)
{
	u32 control = mace->pci.control;

	/* disable master aborts interrupts during config read */
	mace->pci.control = control & ~MACEPCI_CONTROL_MAR_INT;
	mace->pci.config_addr = mkaddr(bus, devfn, reg);
	switch (size) {
	case 1:
		*val = mace->pci.config_data.b[(reg & 3) ^ 3];
		break;
	case 2:
		*val = mace->pci.config_data.w[((reg >> 1) & 1) ^ 1];
		break;
	case 4:
		*val = mace->pci.config_data.l;
		break;
	}
	/* ack possible master abort */
	mace->pci.error &= ~MACEPCI_ERROR_MASTER_ABORT;
	mace->pci.control = control;
	/*
	 * someone forgot to set the ultra bit for the onboard
	 * scsi chips; we fake it here
	 */
	if (bus->number == 0 && reg == 0x40 && size == 4 &&
	    (devfn == (1 << 3) || devfn == (2 << 3)))
		*val |= 0x1000;

	DPRINTK("read%d: reg=%08x,val=%02x\n", size * 8, reg, *val);

	return PCIBIOS_SUCCESSFUL;
}

static int
mace_pci_write_config(struct pci_bus *bus, unsigned int devfn,
		      int reg, int size, u32 val)
{
	mace->pci.config_addr = mkaddr(bus, devfn, reg);
	switch (size) {
	case 1:
		mace->pci.config_data.b[(reg & 3) ^ 3] = val;
		break;
	case 2:
		mace->pci.config_data.w[((reg >> 1) & 1) ^ 1] = val;
		break;
	case 4:
		mace->pci.config_data.l = val;
		break;
	}

	DPRINTK("write%d: reg=%08x,val=%02x\n", size * 8, reg, val);

	return PCIBIOS_SUCCESSFUL;
}

struct pci_ops mace_pci_ops = {
	.read = mace_pci_read_config,
	.write = mace_pci_write_config,
};
