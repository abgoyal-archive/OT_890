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

/* pci-irq.c: PCI IRQ routing on the FRV motherboard
 *
 * Copyright (C) 2003 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 * derived from: arch/i386/kernel/pci-irq.c: (c) 1999--2000 Martin Mares <mj@suse.cz>
 */

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/pci.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/irq.h>

#include <asm/io.h>
#include <asm/smp.h>

#include "pci-frv.h"

/*
 *	DEVICE	DEVNO	INT#A	INT#B	INT#C	INT#D
 *	=======	=======	=======	=======	=======	=======
 *	MB86943	0	fpga.10	-	-	-
 *	RTL8029	16	fpga.12	-	-	-
 *	SLOT 1	19	fpga.6	fpga.5	fpga.4	fpga.3
 *	SLOT 2	18	fpga.5	fpga.4	fpga.3	fpga.6
 *	SLOT 3	17	fpga.4	fpga.3	fpga.6	fpga.5
 *
 */

static const uint8_t __initdata pci_bus0_irq_routing[32][4] = {
	[0 ] = { IRQ_FPGA_MB86943_PCI_INTA },
	[16] = { IRQ_FPGA_RTL8029_INTA },
	[17] = { IRQ_FPGA_PCI_INTC, IRQ_FPGA_PCI_INTD, IRQ_FPGA_PCI_INTA, IRQ_FPGA_PCI_INTB },
	[18] = { IRQ_FPGA_PCI_INTB, IRQ_FPGA_PCI_INTC, IRQ_FPGA_PCI_INTD, IRQ_FPGA_PCI_INTA },
	[19] = { IRQ_FPGA_PCI_INTA, IRQ_FPGA_PCI_INTB, IRQ_FPGA_PCI_INTC, IRQ_FPGA_PCI_INTD },
};

void __init pcibios_irq_init(void)
{
}

void __init pcibios_fixup_irqs(void)
{
	struct pci_dev *dev = NULL;
	uint8_t line, pin;

	for_each_pci_dev(dev) {
		pci_read_config_byte(dev, PCI_INTERRUPT_PIN, &pin);
		if (pin) {
			dev->irq = pci_bus0_irq_routing[PCI_SLOT(dev->devfn)][pin - 1];
			pci_write_config_byte(dev, PCI_INTERRUPT_LINE, dev->irq);
		}
		pci_read_config_byte(dev, PCI_INTERRUPT_LINE, &line);
	}
}

void __init pcibios_penalize_isa_irq(int irq)
{
}

void pcibios_enable_irq(struct pci_dev *dev)
{
	pci_write_config_byte(dev, PCI_INTERRUPT_LINE, dev->irq);
}
