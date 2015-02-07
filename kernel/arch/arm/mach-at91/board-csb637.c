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
 * linux/arch/arm/mach-at91/board-csb637.c
 *
 *  Copyright (C) 2005 SAN People
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <linux/types.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/mtd/physmap.h>

#include <asm/setup.h>
#include <asm/mach-types.h>
#include <asm/irq.h>

#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <asm/mach/irq.h>

#include <mach/hardware.h>
#include <mach/board.h>
#include <mach/gpio.h>

#include "generic.h"


static void __init csb637_map_io(void)
{
	/* Initialize processor: 3.6864 MHz crystal */
	at91rm9200_initialize(3686400, AT91RM9200_BGA);

	/* DBGU on ttyS0. (Rx & Tx only) */
	at91_register_uart(0, 0, 0);

	/* make console=ttyS0 (ie, DBGU) the default */
	at91_set_serial_console(0);
}

static void __init csb637_init_irq(void)
{
	at91rm9200_init_interrupts(NULL);
}

static struct at91_eth_data __initdata csb637_eth_data = {
	.phy_irq_pin	= AT91_PIN_PC0,
	.is_rmii	= 0,
};

static struct at91_usbh_data __initdata csb637_usbh_data = {
	.ports		= 2,
};

static struct at91_udc_data __initdata csb637_udc_data = {
	.vbus_pin     = AT91_PIN_PB28,
	.pullup_pin   = AT91_PIN_PB1,
};

#define CSB_FLASH_BASE	AT91_CHIPSELECT_0
#define CSB_FLASH_SIZE	SZ_16M

static struct mtd_partition csb_flash_partitions[] = {
	{
		.name		= "uMON flash",
		.offset		= 0,
		.size		= MTDPART_SIZ_FULL,
		.mask_flags	= MTD_WRITEABLE,	/* read only */
	}
};

static struct physmap_flash_data csb_flash_data = {
	.width		= 2,
	.parts		= csb_flash_partitions,
	.nr_parts	= ARRAY_SIZE(csb_flash_partitions),
};

static struct resource csb_flash_resources[] = {
	{
		.start	= CSB_FLASH_BASE,
		.end	= CSB_FLASH_BASE + CSB_FLASH_SIZE - 1,
		.flags	= IORESOURCE_MEM,
	}
};

static struct platform_device csb_flash = {
	.name		= "physmap-flash",
	.id		= 0,
	.dev		= {
				.platform_data = &csb_flash_data,
			},
	.resource	= csb_flash_resources,
	.num_resources	= ARRAY_SIZE(csb_flash_resources),
};

static struct gpio_led csb_leds[] = {
	{	/* "d1", red */
		.name			= "d1",
		.gpio			= AT91_PIN_PB2,
		.active_low		= 1,
		.default_trigger	= "heartbeat",
	},
};

static void __init csb637_board_init(void)
{
	/* LED(s) */
	at91_gpio_leds(csb_leds, ARRAY_SIZE(csb_leds));
	/* Serial */
	at91_add_device_serial();
	/* Ethernet */
	at91_add_device_eth(&csb637_eth_data);
	/* USB Host */
	at91_add_device_usbh(&csb637_usbh_data);
	/* USB Device */
	at91_add_device_udc(&csb637_udc_data);
	/* I2C */
	at91_add_device_i2c(NULL, 0);
	/* SPI */
	at91_add_device_spi(NULL, 0);
	/* NOR flash */
	platform_device_register(&csb_flash);
}

MACHINE_START(CSB637, "Cogent CSB637")
	/* Maintainer: Bill Gatliff */
	.phys_io	= AT91_BASE_SYS,
	.io_pg_offst	= (AT91_VA_BASE_SYS >> 18) & 0xfffc,
	.boot_params	= AT91_SDRAM_BASE + 0x100,
	.timer		= &at91rm9200_timer,
	.map_io		= csb637_map_io,
	.init_irq	= csb637_init_irq,
	.init_machine	= csb637_board_init,
MACHINE_END
