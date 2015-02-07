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
 * linux/arch/arm/mach-omap1/board-generic.c
 *
 * Modified from board-innovator1510.c
 *
 * Code for generic OMAP board. Should work on many OMAP systems where
 * the device drivers take care of all the necessary hardware initialization.
 * Do not put any board specific code to this file; create a new machine
 * type if you need custom low-level initializations.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>

#include <mach/hardware.h>
#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <asm/mach/map.h>

#include <mach/gpio.h>
#include <mach/mux.h>
#include <mach/usb.h>
#include <mach/board.h>
#include <mach/common.h>

static void __init omap_generic_init_irq(void)
{
	omap1_init_common_hw();
	omap_init_irq();
}

/* assume no Mini-AB port */

#ifdef CONFIG_ARCH_OMAP15XX
static struct omap_usb_config generic1510_usb_config __initdata = {
	.register_host	= 1,
	.register_dev	= 1,
	.hmc_mode	= 16,
	.pins[0]	= 3,
};
#endif

#if defined(CONFIG_ARCH_OMAP16XX)
static struct omap_usb_config generic1610_usb_config __initdata = {
#ifdef CONFIG_USB_OTG
	.otg		= 1,
#endif
	.register_host	= 1,
	.register_dev	= 1,
	.hmc_mode	= 16,
	.pins[0]	= 6,
};
#endif

static struct omap_uart_config generic_uart_config __initdata = {
	.enabled_uarts = ((1 << 0) | (1 << 1) | (1 << 2)),
};

static struct omap_board_config_kernel generic_config[] __initdata = {
	{ OMAP_TAG_USB,		NULL },
	{ OMAP_TAG_UART,	&generic_uart_config },
};

static void __init omap_generic_init(void)
{
#ifdef CONFIG_ARCH_OMAP15XX
	if (cpu_is_omap15xx()) {
		generic_config[0].data = &generic1510_usb_config;
	}
#endif
#if defined(CONFIG_ARCH_OMAP16XX)
	if (!cpu_is_omap1510()) {
		generic_config[0].data = &generic1610_usb_config;
	}
#endif

	omap_board_config = generic_config;
	omap_board_config_size = ARRAY_SIZE(generic_config);
	omap_serial_init();
	omap_register_i2c_bus(1, 100, NULL, 0);
}

static void __init omap_generic_map_io(void)
{
	omap1_map_common_io();
}

MACHINE_START(OMAP_GENERIC, "Generic OMAP1510/1610/1710")
	/* Maintainer: Tony Lindgren <tony@atomide.com> */
	.phys_io	= 0xfff00000,
	.io_pg_offst	= ((0xfef00000) >> 18) & 0xfffc,
	.boot_params	= 0x10000100,
	.map_io		= omap_generic_map_io,
	.init_irq	= omap_generic_init_irq,
	.init_machine	= omap_generic_init,
	.timer		= &omap_timer,
MACHINE_END
