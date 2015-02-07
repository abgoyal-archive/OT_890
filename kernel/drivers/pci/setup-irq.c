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
 *	drivers/pci/setup-irq.c
 *
 * Extruded from code written by
 *      Dave Rusling (david.rusling@reo.mts.dec.com)
 *      David Mosberger (davidm@cs.arizona.edu)
 *	David Miller (davem@redhat.com)
 *
 * Support routines for initializing a PCI subsystem.
 */


#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/pci.h>
#include <linux/errno.h>
#include <linux/ioport.h>
#include <linux/cache.h>


static void __init
pdev_fixup_irq(struct pci_dev *dev,
	       u8 (*swizzle)(struct pci_dev *, u8 *),
	       int (*map_irq)(struct pci_dev *, u8, u8))
{
	u8 pin, slot;
	int irq = 0;

	/* If this device is not on the primary bus, we need to figure out
	   which interrupt pin it will come in on.   We know which slot it
	   will come in on 'cos that slot is where the bridge is.   Each
	   time the interrupt line passes through a PCI-PCI bridge we must
	   apply the swizzle function.  */

	pci_read_config_byte(dev, PCI_INTERRUPT_PIN, &pin);
	/* Cope with illegal. */
	if (pin > 4)
		pin = 1;

	if (pin != 0) {
		/* Follow the chain of bridges, swizzling as we go.  */
		slot = (*swizzle)(dev, &pin);

		irq = (*map_irq)(dev, slot, pin);
		if (irq == -1)
			irq = 0;
	}
	dev->irq = irq;

	dev_dbg(&dev->dev, "fixup irq: got %d\n", dev->irq);

	/* Always tell the device, so the driver knows what is
	   the real IRQ to use; the device does not use it. */
	pcibios_update_irq(dev, irq);
}

void __init
pci_fixup_irqs(u8 (*swizzle)(struct pci_dev *, u8 *),
	       int (*map_irq)(struct pci_dev *, u8, u8))
{
	struct pci_dev *dev = NULL;
	while ((dev = pci_get_device(PCI_ANY_ID, PCI_ANY_ID, dev)) != NULL) {
		pdev_fixup_irq(dev, swizzle, map_irq);
	}
}
