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

/* ASB2305 Arch-specific PCI declarations
 *
 * Copyright (C) 2007 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 * Derived from: arch/i386/kernel/pci-i386.h: (c) 1999 Martin Mares <mj@ucw.cz>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version
 * 2 of the Licence, or (at your option) any later version.
 */
#ifndef _PCI_ASB2305_H
#define _PCI_ASB2305_H

#undef DEBUG

#ifdef DEBUG
#define DBG(x...) printk(x)
#else
#define DBG(x...)
#endif

#define PCI_PROBE_BIOS 1
#define PCI_PROBE_CONF1 2
#define PCI_PROBE_CONF2 4
#define PCI_NO_CHECKS 0x400
#define PCI_ASSIGN_ROMS 0x1000
#define PCI_BIOS_IRQ_SCAN 0x2000

extern unsigned int pci_probe;

/* pci-asb2305.c */

extern unsigned int pcibios_max_latency;

extern void pcibios_resource_survey(void);
extern int pcibios_enable_resources(struct pci_dev *dev, int mask);

/* pci.c */

extern int pcibios_last_bus;
extern struct pci_bus *pci_root_bus;
extern struct pci_ops *pci_root_ops;

extern struct irq_routing_table *pcibios_get_irq_routing_table(void);
extern int pcibios_set_irq_routing(struct pci_dev *dev, int pin, int irq);

/* pci-irq.c */

struct irq_info {
	u8 bus, devfn;			/* Bus, device and function */
	struct {
		u8 link;		/* IRQ line ID, chipset dependent,
					 * 0=not routed */
		u16 bitmap;		/* Available IRQs */
	} __attribute__((packed)) irq[4];
	u8 slot;			/* Slot number, 0=onboard */
	u8 rfu;
} __attribute__((packed));

struct irq_routing_table {
	u32 signature;			/* PIRQ_SIGNATURE should be here */
	u16 version;			/* PIRQ_VERSION */
	u16 size;			/* Table size in bytes */
	u8 rtr_bus, rtr_devfn;		/* Where the interrupt router lies */
	u16 exclusive_irqs;		/* IRQs devoted exclusively to PCI usage */
	u16 rtr_vendor, rtr_device;	/* Vendor and device ID of interrupt router */
	u32 miniport_data;		/* Crap */
	u8 rfu[11];
	u8 checksum;			/* Modulo 256 checksum must give zero */
	struct irq_info slots[0];
} __attribute__((packed));

extern unsigned int pcibios_irq_mask;

extern void pcibios_irq_init(void);
extern void pcibios_fixup_irqs(void);
extern void pcibios_enable_irq(struct pci_dev *dev);

#endif /* PCI_ASB2305_H */
