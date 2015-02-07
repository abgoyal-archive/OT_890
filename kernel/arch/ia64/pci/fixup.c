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
 * Exceptions for specific devices. Usually work-arounds for fatal design flaws.
 * Derived from fixup.c of i386 tree.
 */

#include <linux/pci.h>
#include <linux/init.h>

#include <asm/machvec.h>

/*
 * Fixup to mark boot BIOS video selected by BIOS before it changes
 *
 * From information provided by "Jon Smirl" <jonsmirl@gmail.com>
 *
 * The standard boot ROM sequence for an x86 machine uses the BIOS
 * to select an initial video card for boot display. This boot video
 * card will have it's BIOS copied to C0000 in system RAM.
 * IORESOURCE_ROM_SHADOW is used to associate the boot video
 * card with this copy. On laptops this copy has to be used since
 * the main ROM may be compressed or combined with another image.
 * See pci_map_rom() for use of this flag. IORESOURCE_ROM_SHADOW
 * is marked here since the boot video device will be the only enabled
 * video device at this point.
 */

static void __devinit pci_fixup_video(struct pci_dev *pdev)
{
	struct pci_dev *bridge;
	struct pci_bus *bus;
	u16 config;

	if ((strcmp(platform_name, "dig") != 0)
	    && (strcmp(platform_name, "hpzx1")  != 0))
		return;
	/* Maybe, this machine supports legacy memory map. */

	if ((pdev->class >> 8) != PCI_CLASS_DISPLAY_VGA)
		return;

	/* Is VGA routed to us? */
	bus = pdev->bus;
	while (bus) {
		bridge = bus->self;

		/*
		 * From information provided by
		 * "David Miller" <davem@davemloft.net>
		 * The bridge control register is valid for PCI header
		 * type BRIDGE, or CARDBUS. Host to PCI controllers use
		 * PCI header type NORMAL.
		 */
		if (bridge
		    &&((bridge->hdr_type == PCI_HEADER_TYPE_BRIDGE)
		       ||(bridge->hdr_type == PCI_HEADER_TYPE_CARDBUS))) {
			pci_read_config_word(bridge, PCI_BRIDGE_CONTROL,
						&config);
			if (!(config & PCI_BRIDGE_CTL_VGA))
				return;
		}
		bus = bus->parent;
	}
	pci_read_config_word(pdev, PCI_COMMAND, &config);
	if (config & (PCI_COMMAND_IO | PCI_COMMAND_MEMORY)) {
		pdev->resource[PCI_ROM_RESOURCE].flags |= IORESOURCE_ROM_SHADOW;
		dev_printk(KERN_DEBUG, &pdev->dev, "Boot video device\n");
	}
}
DECLARE_PCI_FIXUP_HEADER(PCI_ANY_ID, PCI_ANY_ID, pci_fixup_video);
