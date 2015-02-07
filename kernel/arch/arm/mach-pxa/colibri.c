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
 *  linux/arch/arm/mach-pxa/colibri.c
 *
 *  Support for Toradex PXA27x based Colibri module
 *  Daniel Mack <daniel@caiaq.de>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/sysdev.h>
#include <linux/interrupt.h>
#include <linux/bitops.h>
#include <linux/ioport.h>
#include <linux/delay.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/partitions.h>
#include <linux/mtd/physmap.h>
#include <asm/mach-types.h>
#include <mach/hardware.h>
#include <asm/irq.h>
#include <asm/sizes.h>
#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <asm/mach/irq.h>
#include <asm/mach/flash.h>
#include <mach/pxa-regs.h>
#include <mach/mfp-pxa27x.h>
#include <mach/colibri.h>

#include "generic.h"
#include "devices.h"

static unsigned long colibri_pin_config[] __initdata = {
	GPIO78_nCS_2,	/* Ethernet CS */
	GPIO114_GPIO,	/* Ethernet IRQ */
};

/*
 * Flash
 */
static struct mtd_partition colibri_partitions[] = {
	{
		.name =		"Bootloader",
		.offset =	0x00000000,
		.size =		0x00040000,
		.mask_flags =	MTD_WRITEABLE  /* force read-only */
	}, {
		.name =		"Kernel",
		.offset =	0x00040000,
		.size =		0x00400000,
		.mask_flags =	0
	}, {
		.name =		"Rootfs",
		.offset =	0x00440000,
		.size =		MTDPART_SIZ_FULL,
		.mask_flags =	0
	}
};

static struct physmap_flash_data colibri_flash_data[] = {
	{
		.width		= 4,			/* bankwidth in bytes */
		.parts		= colibri_partitions,
		.nr_parts	= ARRAY_SIZE(colibri_partitions)
	}
};

static struct resource flash_resource = {
	.start	= PXA_CS0_PHYS,
	.end	= PXA_CS0_PHYS + SZ_32M - 1,
	.flags	= IORESOURCE_MEM,
};

static struct platform_device flash_device = {
	.name	= "physmap-flash",
	.id	= 0,
	.dev 	= {
		.platform_data = colibri_flash_data,
	},
	.resource = &flash_resource,
	.num_resources = 1,
};

/*
 * DM9000 Ethernet
 */
static struct resource dm9000_resources[] = {
	[0] = {
		.start	= COLIBRI_ETH_PHYS,
		.end	= COLIBRI_ETH_PHYS + 3,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= COLIBRI_ETH_PHYS + 4,
		.end	= COLIBRI_ETH_PHYS + 4 + 500,
		.flags	= IORESOURCE_MEM,
	},
	[2] = {
		.start	= COLIBRI_ETH_IRQ,
		.end	= COLIBRI_ETH_IRQ,
		.flags	= IORESOURCE_IRQ | IRQF_TRIGGER_RISING,
	},
};

static struct platform_device dm9000_device = {
	.name		= "dm9000",
	.id		= -1,
	.num_resources	= ARRAY_SIZE(dm9000_resources),
	.resource	= dm9000_resources,
};

static struct platform_device *colibri_devices[] __initdata = {
	&flash_device,
	&dm9000_device,
};

static void __init colibri_init(void)
{
	pxa2xx_mfp_config(ARRAY_AND_SIZE(colibri_pin_config));

	platform_add_devices(colibri_devices, ARRAY_SIZE(colibri_devices));
}

MACHINE_START(COLIBRI, "Toradex Colibri PXA27x")
	.phys_io	= 0x40000000,
	.io_pg_offst	= (io_p2v(0x40000000) >> 18) & 0xfffc,
	.boot_params	= COLIBRI_SDRAM_BASE + 0x100,
	.init_machine	= colibri_init,
	.map_io		= pxa_map_io,
	.init_irq	= pxa27x_init_irq,
	.timer		= &pxa_timer,
MACHINE_END
