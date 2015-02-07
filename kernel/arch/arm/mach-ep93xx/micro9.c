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
 *  linux/arch/arm/mach-ep93xx/micro9.c
 *
 * Copyright (C) 2006 Contec Steuerungstechnik & Automation GmbH
 *                   Manfred Gruber <manfred.gruber@contec.at>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/platform_device.h>
#include <linux/sched.h>
#include <linux/io.h>
#include <linux/i2c.h>
#include <linux/mtd/physmap.h>

#include <mach/hardware.h>

#include <asm/mach/arch.h>
#include <asm/mach-types.h>

static struct ep93xx_eth_data micro9_eth_data = {
	.phy_id		= 0x1f,
};

static void __init micro9_init(void)
{
	ep93xx_register_eth(&micro9_eth_data, 1);
}

/*
 * Micro9-H
 */
#ifdef CONFIG_MACH_MICRO9H
static struct physmap_flash_data micro9h_flash_data = {
	.width		= 4,
};

static struct resource micro9h_flash_resource = {
	.start		= EP93XX_CS1_PHYS_BASE,
	.end		= EP93XX_CS1_PHYS_BASE + SZ_64M - 1,
	.flags		= IORESOURCE_MEM,
};

static struct platform_device micro9h_flash = {
	.name		= "physmap-flash",
	.id		= 0,
	.dev		= {
		.platform_data	= &micro9h_flash_data,
	},
	.num_resources	= 1,
	.resource	= &micro9h_flash_resource,
};

static void __init micro9h_init(void)
{
	platform_device_register(&micro9h_flash);
}

static void __init micro9h_init_machine(void)
{
	ep93xx_init_devices();
	micro9_init();
	micro9h_init();
}

MACHINE_START(MICRO9, "Contec Hypercontrol Micro9-H")
	/* Maintainer: Manfred Gruber <manfred.gruber@contec.at> */
	.phys_io	= EP93XX_APB_PHYS_BASE,
	.io_pg_offst	= ((EP93XX_APB_VIRT_BASE) >> 18) & 0xfffc,
	.boot_params	= EP93XX_SDCE3_PHYS_BASE_SYNC + 0x100,
	.map_io		= ep93xx_map_io,
	.init_irq	= ep93xx_init_irq,
	.timer		= &ep93xx_timer,
	.init_machine	= micro9h_init_machine,
MACHINE_END
#endif

/*
 * Micro9-M
 */
#ifdef CONFIG_MACH_MICRO9M
static void __init micro9m_init_machine(void)
{
	ep93xx_init_devices();
	micro9_init();
}

MACHINE_START(MICRO9M, "Contec Hypercontrol Micro9-M")
	/* Maintainer: Manfred Gruber <manfred.gruber@contec.at> */
	.phys_io	= EP93XX_APB_PHYS_BASE,
	.io_pg_offst	= ((EP93XX_APB_VIRT_BASE) >> 18) & 0xfffc,
	.boot_params	= EP93XX_SDCE3_PHYS_BASE_SYNC + 0x100,
	.map_io		= ep93xx_map_io,
	.init_irq	= ep93xx_init_irq,
	.timer		= &ep93xx_timer,
	.init_machine	= micro9m_init_machine,
MACHINE_END
#endif

/*
 * Micro9-L
 */
#ifdef CONFIG_MACH_MICRO9L
static void __init micro9l_init_machine(void)
{
	ep93xx_init_devices();
	micro9_init();
}

MACHINE_START(MICRO9L, "Contec Hypercontrol Micro9-L")
	/* Maintainer: Manfred Gruber <manfred.gruber@contec.at> */
	.phys_io	= EP93XX_APB_PHYS_BASE,
	.io_pg_offst	= ((EP93XX_APB_VIRT_BASE) >> 18) & 0xfffc,
	.boot_params	= EP93XX_SDCE3_PHYS_BASE_SYNC + 0x100,
	.map_io		= ep93xx_map_io,
	.init_irq	= ep93xx_init_irq,
	.timer		= &ep93xx_timer,
	.init_machine	= micro9l_init_machine,
MACHINE_END
#endif

