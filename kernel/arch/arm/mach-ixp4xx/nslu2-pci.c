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
 * arch/arm/mach-ixp4xx/nslu2-pci.c
 *
 * NSLU2 board-level PCI initialization
 *
 * based on ixdp425-pci.c:
 *	Copyright (C) 2002 Intel Corporation.
 *	Copyright (C) 2003-2004 MontaVista Software, Inc.
 *
 * Maintainer: http://www.nslu2-linux.org/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#include <linux/pci.h>
#include <linux/init.h>
#include <linux/irq.h>

#include <asm/mach/pci.h>
#include <asm/mach-types.h>

void __init nslu2_pci_preinit(void)
{
	set_irq_type(IRQ_NSLU2_PCI_INTA, IRQ_TYPE_LEVEL_LOW);
	set_irq_type(IRQ_NSLU2_PCI_INTB, IRQ_TYPE_LEVEL_LOW);
	set_irq_type(IRQ_NSLU2_PCI_INTC, IRQ_TYPE_LEVEL_LOW);

	ixp4xx_pci_preinit();
}

static int __init nslu2_map_irq(struct pci_dev *dev, u8 slot, u8 pin)
{
	static int pci_irq_table[NSLU2_PCI_IRQ_LINES] = {
		IRQ_NSLU2_PCI_INTA,
		IRQ_NSLU2_PCI_INTB,
		IRQ_NSLU2_PCI_INTC,
	};

	int irq = -1;

	if (slot >= 1 && slot <= NSLU2_PCI_MAX_DEV &&
		pin >= 1 && pin <= NSLU2_PCI_IRQ_LINES) {
			irq = pci_irq_table[(slot + pin - 2) % NSLU2_PCI_IRQ_LINES];
	}

	return irq;
}

struct hw_pci __initdata nslu2_pci = {
	.nr_controllers = 1,
	.preinit	= nslu2_pci_preinit,
	.swizzle	= pci_std_swizzle,
	.setup		= ixp4xx_setup,
	.scan		= ixp4xx_scan_bus,
	.map_irq	= nslu2_map_irq,
};

int __init nslu2_pci_init(void) /* monkey see, monkey do */
{
	if (machine_is_nslu2())
		pci_common_init(&nslu2_pci);

	return 0;
}

subsys_initcall(nslu2_pci_init);
