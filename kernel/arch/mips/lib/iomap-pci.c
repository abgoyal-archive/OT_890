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
 * Implement the default iomap interfaces
 *
 * (C) Copyright 2004 Linus Torvalds
 * (C) Copyright 2006 Ralf Baechle <ralf@linux-mips.org>
 * (C) Copyright 2007 MIPS Technologies, Inc.
 *     written by Ralf Baechle <ralf@linux-mips.org>
 */
#include <linux/pci.h>
#include <linux/module.h>
#include <asm/io.h>

static void __iomem *ioport_map_pci(struct pci_dev *dev,
                                     unsigned long port, unsigned int nr)
{
	struct pci_controller *ctrl = dev->bus->sysdata;
	unsigned long base = ctrl->io_map_base;

	/* This will eventually become a BUG_ON but for now be gentle */
	if (unlikely(!ctrl->io_map_base)) {
		struct pci_bus *bus = dev->bus;
		char name[8];

		while (bus->parent)
			bus = bus->parent;

		ctrl->io_map_base = base = mips_io_port_base;

		sprintf(name, "%04x:%02x", pci_domain_nr(bus), bus->number);
		printk(KERN_WARNING "io_map_base of root PCI bus %s unset.  "
		       "Trying to continue but you better\nfix this issue or "
		       "report it to linux-mips@linux-mips.org or your "
		       "vendor.\n", name);
#ifdef CONFIG_PCI_DOMAINS
		panic("To avoid data corruption io_map_base MUST be set with "
		      "multiple PCI domains.");
#endif
	}

	return (void __iomem *) (ctrl->io_map_base + port);
}

/*
 * Create a virtual mapping cookie for a PCI BAR (memory or IO)
 */
void __iomem *pci_iomap(struct pci_dev *dev, int bar, unsigned long maxlen)
{
	resource_size_t start = pci_resource_start(dev, bar);
	resource_size_t len = pci_resource_len(dev, bar);
	unsigned long flags = pci_resource_flags(dev, bar);

	if (!len || !start)
		return NULL;
	if (maxlen && len > maxlen)
		len = maxlen;
	if (flags & IORESOURCE_IO)
		return ioport_map_pci(dev, start, len);
	if (flags & IORESOURCE_MEM) {
		if (flags & IORESOURCE_CACHEABLE)
			return ioremap(start, len);
		return ioremap_nocache(start, len);
	}
	/* What? */
	return NULL;
}

EXPORT_SYMBOL(pci_iomap);

void pci_iounmap(struct pci_dev *dev, void __iomem * addr)
{
	iounmap(addr);
}

EXPORT_SYMBOL(pci_iounmap);
