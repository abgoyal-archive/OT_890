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
 * arch/arm/mach-ks8695/board-dsm320.c
 *
 * DSM-320 D-Link Wireless Media Player, board support.
 *
 * Copyright 2008 Simtec Electronics
 *		  Daniel Silverstone <dsilvers@simtec.co.uk>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/interrupt.h>
#include <linux/init.h>
#include <linux/platform_device.h>

#include <linux/mtd/mtd.h>
#include <linux/mtd/map.h>
#include <linux/mtd/physmap.h>
#include <linux/mtd/partitions.h>

#include <asm/mach-types.h>

#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <asm/mach/irq.h>

#include <mach/devices.h>
#include <mach/gpio.h>

#include "generic.h"

#ifdef CONFIG_PCI
static int dsm320_pci_map_irq(struct pci_dev *dev, u8 slot, u8 pin)
{
	switch (slot) {
	case 0:
		/* PCI-AHB bridge? */
		return KS8695_IRQ_EXTERN0;
	case 18:
		/* Mini PCI slot */
		return KS8695_IRQ_EXTERN2;
	case 20:
		/* RealMAGIC chip */
		return KS8695_IRQ_EXTERN0;
	}
	BUG();
}

static struct ks8695_pci_cfg __initdata dsm320_pci = {
	.mode		= KS8695_MODE_MINIPCI,
	.map_irq	= dsm320_pci_map_irq,
};

static void __init dsm320_register_pci(void)
{
	/* Initialise the GPIO lines for interrupt mode */
	/* RealMAGIC */
	ks8695_gpio_interrupt(KS8695_GPIO_0, IRQ_TYPE_LEVEL_LOW);
	/* MiniPCI Slot */
	ks8695_gpio_interrupt(KS8695_GPIO_2, IRQ_TYPE_LEVEL_LOW);

	ks8695_init_pci(&dsm320_pci);
}

#else
static inline void __init dsm320_register_pci(void) { }
#endif

static struct physmap_flash_data dsm320_nor_pdata = {
	.width		= 4,
	.nr_parts	= 0,
};

static struct resource dsm320_nor_resource[] = {
	[0] = {
		.start = SZ_32M, /* We expect the bootloader to map
				  * the flash here.
				  */
		.end   = SZ_32M + SZ_4M - 1,
		.flags = IORESOURCE_MEM,
	}
};

static struct platform_device dsm320_device_nor = {
	.name		= "physmap-flash",
	.id		= -1,
	.num_resources	= ARRAY_SIZE(dsm320_nor_resource),
	.resource	= dsm320_nor_resource,
	.dev		= {
		.platform_data = &dsm320_nor_pdata,
	},
};

void __init dsm320_register_nor(void)
{
	int ret;

	ret = platform_device_register(&dsm320_device_nor);
	if (ret < 0)
		printk(KERN_ERR "failed to register physmap-flash device\n");
}

static void __init dsm320_init(void)
{
	/* GPIO registration */
	ks8695_register_gpios();

	/* PCI registration */
	dsm320_register_pci();

	/* Network device */
	ks8695_add_device_lan();	/* eth0 = LAN */

	/* NOR devices */
	dsm320_register_nor();
}

MACHINE_START(DSM320, "D-Link DSM-320 Wireless Media Player")
	/* Maintainer: Simtec Electronics. */
	.phys_io	= KS8695_IO_PA,
	.io_pg_offst	= (KS8695_IO_VA >> 18) & 0xfffc,
	.boot_params	= KS8695_SDRAM_PA + 0x100,
	.map_io		= ks8695_map_io,
	.init_irq	= ks8695_init_irq,
	.init_machine	= dsm320_init,
	.timer		= &ks8695_timer,
MACHINE_END
