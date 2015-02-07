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
 * Support functions for the SH5 PCI hardware.
 *
 * Copyright (C) 2001 David J. Mckay (david.mckay@st.com)
 * Copyright (C) 2003, 2004 Paul Mundt
 * Copyright (C) 2004 Richard Curnow
 *
 * May be copied or modified under the terms of the GNU General Public
 * License.  See linux/COPYING for more information.
 */
#include <linux/kernel.h>
#include <linux/rwsem.h>
#include <linux/smp.h>
#include <linux/interrupt.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/pci.h>
#include <linux/delay.h>
#include <linux/types.h>
#include <linux/irq.h>
#include <asm/pci.h>
#include <asm/io.h>
#include "pci-sh5.h"

static void __init pci_fixup_ide_bases(struct pci_dev *d)
{
	int i;

	/*
	 * PCI IDE controllers use non-standard I/O port decoding, respect it.
	 */
	if ((d->class >> 8) != PCI_CLASS_STORAGE_IDE)
		return;
	printk("PCI: IDE base address fixup for %s\n", pci_name(d));
	for(i=0; i<4; i++) {
		struct resource *r = &d->resource[i];
		if ((r->start & ~0x80) == 0x374) {
			r->start |= 2;
			r->end = r->start;
		}
	}
}
DECLARE_PCI_FIXUP_HEADER(PCI_ANY_ID, PCI_ANY_ID, pci_fixup_ide_bases);

char * __devinit pcibios_setup(char *str)
{
	return str;
}

static int sh5pci_read(struct pci_bus *bus, unsigned int devfn, int where,
			int size, u32 *val)
{
	SH5PCI_WRITE(PAR, CONFIG_CMD(bus, devfn, where));

	switch (size) {
		case 1:
			*val = (u8)SH5PCI_READ_BYTE(PDR + (where & 3));
			break;
		case 2:
			*val = (u16)SH5PCI_READ_SHORT(PDR + (where & 2));
			break;
		case 4:
			*val = SH5PCI_READ(PDR);
			break;
	}

	return PCIBIOS_SUCCESSFUL;
}

static int sh5pci_write(struct pci_bus *bus, unsigned int devfn, int where,
			 int size, u32 val)
{
	SH5PCI_WRITE(PAR, CONFIG_CMD(bus, devfn, where));

	switch (size) {
		case 1:
			SH5PCI_WRITE_BYTE(PDR + (where & 3), (u8)val);
			break;
		case 2:
			SH5PCI_WRITE_SHORT(PDR + (where & 2), (u16)val);
			break;
		case 4:
			SH5PCI_WRITE(PDR, val);
			break;
	}

	return PCIBIOS_SUCCESSFUL;
}

struct pci_ops sh5_pci_ops = {
	.read		= sh5pci_read,
	.write		= sh5pci_write,
};
