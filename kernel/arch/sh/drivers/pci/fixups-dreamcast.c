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
 * arch/sh/drivers/pci/fixups-dreamcast.c
 *
 * PCI fixups for the Sega Dreamcast
 *
 * Copyright (C) 2001, 2002  M. R. Brown
 * Copyright (C) 2002, 2003, 2006  Paul Mundt
 *
 * This file originally bore the message (with enclosed-$):
 *	Id: pci.c,v 1.3 2003/05/04 19:29:46 lethal Exp
 *	Dreamcast PCI: Supports SEGA Broadband Adaptor only.
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */

#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/param.h>
#include <linux/interrupt.h>
#include <linux/init.h>
#include <linux/irq.h>
#include <linux/pci.h>
#include <linux/dma-mapping.h>

#include <asm/io.h>
#include <asm/irq.h>
#include <mach/pci.h>

static void __init gapspci_fixup_resources(struct pci_dev *dev)
{
	struct pci_channel *p = board_pci_channels;

	printk(KERN_NOTICE "PCI: Fixing up device %s\n", pci_name(dev));

	switch (dev->device) {
	case PCI_DEVICE_ID_SEGA_BBA:
		/*
		 * We also assume that dev->devfn == 0
		 */
		dev->resource[1].start	= p->io_resource->start  + 0x100;
		dev->resource[1].end	= dev->resource[1].start + 0x200 - 1;
		/*
		 * Redirect dma memory allocations to special memory window.
		 */
		BUG_ON(!dma_declare_coherent_memory(&dev->dev,
						GAPSPCI_DMA_BASE,
						GAPSPCI_DMA_BASE,
						GAPSPCI_DMA_SIZE,
						DMA_MEMORY_MAP |
						DMA_MEMORY_EXCLUSIVE));
		break;
	default:
		printk("PCI: Failed resource fixup\n");
	}
}
DECLARE_PCI_FIXUP_HEADER(PCI_ANY_ID, PCI_ANY_ID, gapspci_fixup_resources);

int __init pcibios_map_platform_irq(struct pci_dev *dev, u8 slot, u8 pin)
{
	/*
	 * The interrupt routing semantics here are quite trivial.
	 *
	 * We basically only support one interrupt, so we only bother
	 * updating a device's interrupt line with this single shared
	 * interrupt. Keeps routing quite simple, doesn't it?
	 */
	return GAPSPCI_IRQ;
}
