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
 * SCC (Super Companion Chip) UHC setup
 *
 * (C) Copyright 2006-2007 TOSHIBA CORPORATION
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <linux/kernel.h>
#include <linux/pci.h>

#include <asm/delay.h>
#include <asm/io.h>
#include <asm/machdep.h>

#include "celleb_scc.h"

#define UHC_RESET_WAIT_MAX 10000

static inline int uhc_clkctrl_ready(u32 val)
{
	const u32 mask = SCC_UHC_USBCEN | SCC_UHC_USBCEN;
	return((val & mask) == mask);
}

/*
 * UHC(usb host controller) enable function.
 * affect to both of OHCI and EHCI core module.
 */
static void enable_scc_uhc(struct pci_dev *dev)
{
	void __iomem *uhc_base;
	u32 __iomem *uhc_clkctrl;
	u32 __iomem *uhc_ecmode;
	u32 val = 0;
	int i;

	if (!machine_is(celleb_beat) &&
	    !machine_is(celleb_native))
		return;

	uhc_base = ioremap(pci_resource_start(dev, 0),
			   pci_resource_len(dev, 0));
	if (!uhc_base) {
		printk(KERN_ERR "failed to map UHC register base.\n");
		return;
	}
	uhc_clkctrl = uhc_base + SCC_UHC_CKRCTRL;
	uhc_ecmode  = uhc_base + SCC_UHC_ECMODE;

	/* setup for normal mode */
	val |= SCC_UHC_F48MCKLEN;
	out_be32(uhc_clkctrl, val);
	val |= SCC_UHC_PHY_SUSPEND_SEL;
	out_be32(uhc_clkctrl, val);
	udelay(10);
	val |= SCC_UHC_PHYEN;
	out_be32(uhc_clkctrl, val);
	udelay(50);

	/* disable reset */
	val |= SCC_UHC_HCLKEN;
	out_be32(uhc_clkctrl, val);
	val |= (SCC_UHC_USBCEN | SCC_UHC_USBEN);
	out_be32(uhc_clkctrl, val);
	i = 0;
	while (!uhc_clkctrl_ready(in_be32(uhc_clkctrl))) {
		udelay(10);
		if (i++ > UHC_RESET_WAIT_MAX) {
			printk(KERN_ERR "Failed to disable UHC reset %x\n",
			       in_be32(uhc_clkctrl));
			break;
		}
	}

	/* Endian Conversion Mode for Master ALL area */
	out_be32(uhc_ecmode, SCC_UHC_ECMODE_BY_BYTE);

	iounmap(uhc_base);
}

DECLARE_PCI_FIXUP_HEADER(PCI_VENDOR_ID_TOSHIBA_2,
		 PCI_DEVICE_ID_TOSHIBA_SCC_USB, enable_scc_uhc);
