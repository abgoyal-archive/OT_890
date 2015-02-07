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
 * Hardware definitions for HP iPAQ h5xxx Handheld Computers
 *
 * Copyright 2000-2003  Hewlett-Packard Company.
 * Copyright 2002       Jamey Hicks <jamey.hicks@hp.com>
 * Copyright 2004-2005  Phil Blundell <pb@handhelds.org>
 * Copyright 2007-2008  Anton Vorontsov <cbouatmailru@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * COMPAQ COMPUTER CORPORATION MAKES NO WARRANTIES, EXPRESSED OR IMPLIED,
 * AS TO THE USEFULNESS OR CORRECTNESS OF THIS CODE OR ITS
 * FITNESS FOR ANY PARTICULAR PURPOSE.
 *
 * Author: Jamey Hicks.
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/partitions.h>
#include <linux/mtd/physmap.h>
#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <mach/h5000.h>
#include <mach/pxa-regs.h>
#include <mach/pxa2xx-regs.h>
#include <mach/mfp-pxa25x.h>
#include <mach/udc.h>
#include "generic.h"

/*
 * Flash
 */

static struct mtd_partition h5000_flash0_partitions[] = {
	{
		.name = "bootldr",
		.size = 0x00040000,
		.offset = 0,
		.mask_flags = MTD_WRITEABLE,
	},
	{
		.name = "root",
		.size = MTDPART_SIZ_FULL,
		.offset = MTDPART_OFS_APPEND,
	},
};

static struct mtd_partition h5000_flash1_partitions[] = {
	{
		.name = "second root",
		.size = SZ_16M - 0x00040000,
		.offset = 0,
	},
	{
		.name = "asset",
		.size = MTDPART_SIZ_FULL,
		.offset = MTDPART_OFS_APPEND,
		.mask_flags = MTD_WRITEABLE,
	},
};

static struct physmap_flash_data h5000_flash0_data = {
	.width = 4,
	.parts = h5000_flash0_partitions,
	.nr_parts = ARRAY_SIZE(h5000_flash0_partitions),
};

static struct physmap_flash_data h5000_flash1_data = {
	.width = 4,
	.parts = h5000_flash1_partitions,
	.nr_parts = ARRAY_SIZE(h5000_flash1_partitions),
};

static struct resource h5000_flash0_resources = {
	.start = PXA_CS0_PHYS,
	.end = PXA_CS0_PHYS + SZ_32M - 1,
	.flags = IORESOURCE_MEM | IORESOURCE_MEM_32BIT,
};

static struct resource h5000_flash1_resources = {
	.start = PXA_CS0_PHYS + SZ_32M,
	.end = PXA_CS0_PHYS + SZ_32M + SZ_16M - 1,
	.flags = IORESOURCE_MEM | IORESOURCE_MEM_32BIT,
};

static struct platform_device h5000_flash[] = {
	{
		.name = "physmap-flash",
		.id = 0,
		.resource = &h5000_flash0_resources,
		.num_resources = 1,
		.dev = {
			.platform_data = &h5000_flash0_data,
		},
	},
	{
		.name = "physmap-flash",
		.id = 1,
		.resource = &h5000_flash1_resources,
		.num_resources = 1,
		.dev = {
			.platform_data = &h5000_flash1_data,
		},
	},
};

/*
 * USB Device Controller
 */

static struct pxa2xx_udc_mach_info h5000_udc_mach_info __initdata = {
	.gpio_pullup = H5000_GPIO_USB_PULLUP,
};

/*
 * GPIO setup
 */

static unsigned long h5000_pin_config[] __initdata = {
	/* Crystal and Clock Signals */
	GPIO12_32KHz,

	/* SDRAM and Static Memory I/O Signals */
	GPIO15_nCS_1,
	GPIO78_nCS_2,
	GPIO79_nCS_3,
	GPIO80_nCS_4,

	/* FFUART */
	GPIO34_FFUART_RXD,
	GPIO35_FFUART_CTS,
	GPIO36_FFUART_DCD,
	GPIO37_FFUART_DSR,
	GPIO38_FFUART_RI,
	GPIO39_FFUART_TXD,
	GPIO40_FFUART_DTR,
	GPIO41_FFUART_RTS,

	/* BTUART */
	GPIO42_BTUART_RXD,
	GPIO43_BTUART_TXD,
	GPIO44_BTUART_CTS,
	GPIO45_BTUART_RTS,

	/* SSP1 */
	GPIO23_SSP1_SCLK,
	GPIO25_SSP1_TXD,
	GPIO26_SSP1_RXD,
};

/*
 * Localbus setup:
 * CS0: Flash;
 * CS1: MediaQ chip, select 16-bit bus and vlio;
 * CS5: SAMCOP.
 */

static void fix_msc(void)
{
	MSC0 = 0x129c24f2;
	MSC1 = 0x7ff424fa;
	MSC2 = 0x7ff47ff4;

	MDREFR |= 0x02080000;
}

/*
 * Platform devices
 */

static struct platform_device *devices[] __initdata = {
	&h5000_flash[0],
	&h5000_flash[1],
};

static void __init h5000_init(void)
{
	fix_msc();

	pxa2xx_mfp_config(ARRAY_AND_SIZE(h5000_pin_config));
	pxa_set_udc_info(&h5000_udc_mach_info);
	platform_add_devices(ARRAY_AND_SIZE(devices));
}

MACHINE_START(H5400, "HP iPAQ H5000")
	.phys_io = 0x40000000,
	.io_pg_offst = (io_p2v(0x40000000) >> 18) & 0xfffc,
	.boot_params = 0xa0000100,
	.map_io = pxa_map_io,
	.init_irq = pxa25x_init_irq,
	.timer = &pxa_timer,
	.init_machine = h5000_init,
MACHINE_END
