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
 * DSM-G600 board-level PCI initialization
 *
 * Copyright (C) 2006 Tower Technologies
 * Author: Alessandro Zummo <a.zummo@towertech.it>
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

void __init dsmg600_pci_preinit(void)
{
	set_irq_type(IRQ_DSMG600_PCI_INTA, IRQ_TYPE_LEVEL_LOW);
	set_irq_type(IRQ_DSMG600_PCI_INTB, IRQ_TYPE_LEVEL_LOW);
	set_irq_type(IRQ_DSMG600_PCI_INTC, IRQ_TYPE_LEVEL_LOW);
	set_irq_type(IRQ_DSMG600_PCI_INTD, IRQ_TYPE_LEVEL_LOW);
	set_irq_type(IRQ_DSMG600_PCI_INTE, IRQ_TYPE_LEVEL_LOW);
	set_irq_type(IRQ_DSMG600_PCI_INTF, IRQ_TYPE_LEVEL_LOW);

	ixp4xx_pci_preinit();
}

static int __init dsmg600_map_irq(struct pci_dev *dev, u8 slot, u8 pin)
{
	static int pci_irq_table[DSMG600_PCI_MAX_DEV][DSMG600_PCI_IRQ_LINES] =
	{
		{ IRQ_DSMG600_PCI_INTE, -1, -1 },
		{ IRQ_DSMG600_PCI_INTA, -1, -1 },
		{ IRQ_DSMG600_PCI_INTB, IRQ_DSMG600_PCI_INTC, IRQ_DSMG600_PCI_INTD },
		{ IRQ_DSMG600_PCI_INTF, -1, -1 },
	};

	int irq = -1;

	if (slot >= 1 && slot <= DSMG600_PCI_MAX_DEV &&
		pin >= 1 && pin <= DSMG600_PCI_IRQ_LINES)
		irq = pci_irq_table[slot-1][pin-1];

	return irq;
}

struct hw_pci __initdata dsmg600_pci = {
	.nr_controllers = 1,
	.preinit	= dsmg600_pci_preinit,
	.swizzle	= pci_std_swizzle,
	.setup		= ixp4xx_setup,
	.scan		= ixp4xx_scan_bus,
	.map_irq	= dsmg600_map_irq,
};

int __init dsmg600_pci_init(void)
{
	if (machine_is_dsmg600())
		pci_common_init(&dsmg600_pci);

	return 0;
}

subsys_initcall(dsmg600_pci_init);
