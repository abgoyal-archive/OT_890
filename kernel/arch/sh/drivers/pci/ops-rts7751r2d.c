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
 * linux/arch/sh/drivers/pci/ops-rts7751r2d.c
 *
 * Author:  Ian DaSilva (idasilva@mvista.com)
 *
 * Highly leveraged from pci-bigsur.c, written by Dustin McIntire.
 *
 * May be copied or modified under the terms of the GNU General Public
 * License.  See linux/COPYING for more information.
 *
 * PCI initialization for the Renesas SH7751R RTS7751R2D board
 */
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/init.h>
#include <linux/pci.h>
#include <linux/io.h>
#include <mach/r2d.h>
#include "pci-sh4.h"

static u8 rts7751r2d_irq_tab[] __initdata = {
	IRQ_PCI_INTA,
	IRQ_PCI_INTB,
	IRQ_PCI_INTC,
	IRQ_PCI_INTD,
};

int __init pcibios_map_platform_irq(struct pci_dev *pdev, u8 slot, u8 pin)
{
	return rts7751r2d_irq_tab[slot];
}

static struct resource sh7751_io_resource = {
	.name	= "SH7751_IO",
	.start	= 0x4000,
	.end	= SH7751_PCI_IO_SIZE - 1,
	.flags	= IORESOURCE_IO
};

static struct resource sh7751_mem_resource = {
	.name	= "SH7751_mem",
	.start	= SH7751_PCI_MEMORY_BASE,
	.end	= SH7751_PCI_MEMORY_BASE + SH7751_PCI_MEM_SIZE - 1,
	.flags	= IORESOURCE_MEM
};

extern struct pci_ops sh7751_pci_ops;

struct pci_channel board_pci_channels[] = {
	{ &sh4_pci_ops, &sh7751_io_resource, &sh7751_mem_resource, 0, 0xff },
	{ NULL, NULL, NULL, 0, 0 },
};
EXPORT_SYMBOL(board_pci_channels);

static struct sh4_pci_address_map sh7751_pci_map = {
	.window0	= {
		.base	= SH7751_CS3_BASE_ADDR,
		.size	= 0x04000000,
	},

	.window1	= {
		.base	= 0x00000000,	/* Unused */
		.size	= 0x00000000,	/* Unused */
	},

	.flags	= SH4_PCIC_NO_RESET,
};

int __init pcibios_init_platform(void)
{
	__set_io_port_base(SH7751_PCI_IO_BASE);
	return sh7751_pcic_init(&sh7751_pci_map);
}

