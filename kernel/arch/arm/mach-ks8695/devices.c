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
 * arch/arm/mach-ks8695/devices.c
 *
 * Copyright (C) 2006 Andrew Victor
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation.
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

#include <asm/mach/arch.h>
#include <asm/mach/map.h>

#include <linux/platform_device.h>

#include <mach/irqs.h>
#include <mach/regs-wan.h>
#include <mach/regs-lan.h>
#include <mach/regs-hpna.h>
#include <mach/regs-switch.h>
#include <mach/regs-misc.h>


/* --------------------------------------------------------------------
 *  Ethernet
 * -------------------------------------------------------------------- */

static u64 eth_dmamask = 0xffffffffUL;

static struct resource ks8695_wan_resources[] = {
	[0] = {
		.start	= KS8695_WAN_PA,
		.end	= KS8695_WAN_PA + 0x00ff,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.name	= "WAN RX",
		.start	= KS8695_IRQ_WAN_RX_STATUS,
		.end	= KS8695_IRQ_WAN_RX_STATUS,
		.flags	= IORESOURCE_IRQ,
	},
	[2] = {
		.name	= "WAN TX",
		.start	= KS8695_IRQ_WAN_TX_STATUS,
		.end	= KS8695_IRQ_WAN_TX_STATUS,
		.flags	= IORESOURCE_IRQ,
	},
	[3] = {
		.name	= "WAN Link",
		.start	= KS8695_IRQ_WAN_LINK,
		.end	= KS8695_IRQ_WAN_LINK,
		.flags	= IORESOURCE_IRQ,
	},
	[4] = {
		.name	= "WAN PHY",
		.start	= KS8695_MISC_PA,
		.end	= KS8695_MISC_PA + 0x1f,
		.flags	= IORESOURCE_MEM,
	},
};

static struct platform_device ks8695_wan_device = {
	.name		= "ks8695_ether",
	.id		= 0,
	.dev		= {
				.dma_mask		= &eth_dmamask,
				.coherent_dma_mask	= 0xffffffff,
	},
	.resource	= ks8695_wan_resources,
	.num_resources	= ARRAY_SIZE(ks8695_wan_resources),
};


static struct resource ks8695_lan_resources[] = {
	[0] = {
		.start	= KS8695_LAN_PA,
		.end	= KS8695_LAN_PA + 0x00ff,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.name	= "LAN RX",
		.start	= KS8695_IRQ_LAN_RX_STATUS,
		.end	= KS8695_IRQ_LAN_RX_STATUS,
		.flags	= IORESOURCE_IRQ,
	},
	[2] = {
		.name	= "LAN TX",
		.start	= KS8695_IRQ_LAN_TX_STATUS,
		.end	= KS8695_IRQ_LAN_TX_STATUS,
		.flags	= IORESOURCE_IRQ,
	},
	[3] = {
		.name	= "LAN SWITCH",
		.start	= KS8695_SWITCH_PA,
		.end	= KS8695_SWITCH_PA + 0x4f,
		.flags	= IORESOURCE_MEM,
	},
};

static struct platform_device ks8695_lan_device = {
	.name		= "ks8695_ether",
	.id		= 1,
	.dev		= {
				.dma_mask		= &eth_dmamask,
				.coherent_dma_mask	= 0xffffffff,
	},
	.resource	= ks8695_lan_resources,
	.num_resources	= ARRAY_SIZE(ks8695_lan_resources),
};


static struct resource ks8695_hpna_resources[] = {
	[0] = {
		.start	= KS8695_HPNA_PA,
		.end	= KS8695_HPNA_PA + 0x00ff,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.name	= "HPNA RX",
		.start	= KS8695_IRQ_HPNA_RX_STATUS,
		.end	= KS8695_IRQ_HPNA_RX_STATUS,
		.flags	= IORESOURCE_IRQ,
	},
	[2] = {
		.name	= "HPNA TX",
		.start	= KS8695_IRQ_HPNA_TX_STATUS,
		.end	= KS8695_IRQ_HPNA_TX_STATUS,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct platform_device ks8695_hpna_device = {
	.name		= "ks8695_ether",
	.id		= 2,
	.dev		= {
				.dma_mask		= &eth_dmamask,
				.coherent_dma_mask	= 0xffffffff,
	},
	.resource	= ks8695_hpna_resources,
	.num_resources	= ARRAY_SIZE(ks8695_hpna_resources),
};

void __init ks8695_add_device_wan(void)
{
	platform_device_register(&ks8695_wan_device);
}

void __init ks8695_add_device_lan(void)
{
	platform_device_register(&ks8695_lan_device);
}

void __init ks8696_add_device_hpna(void)
{
	platform_device_register(&ks8695_hpna_device);
}


/* --------------------------------------------------------------------
 *  Watchdog
 * -------------------------------------------------------------------- */

static struct platform_device ks8695_wdt_device = {
	.name		= "ks8695_wdt",
	.id		= -1,
	.num_resources	= 0,
};

static void __init ks8695_add_device_watchdog(void)
{
	platform_device_register(&ks8695_wdt_device);
}


/* --------------------------------------------------------------------
 *  LEDs
 * -------------------------------------------------------------------- */

#if defined(CONFIG_LEDS)
short ks8695_leds_cpu = -1;
short ks8695_leds_timer = -1;

void __init ks8695_init_leds(u8 cpu_led, u8 timer_led)
{
	/* Enable GPIO to access the LEDs */
	gpio_direction_output(cpu_led, 1);
	gpio_direction_output(timer_led, 1);

	ks8695_leds_cpu	  = cpu_led;
	ks8695_leds_timer = timer_led;
}
#else
void __init ks8695_init_leds(u8 cpu_led, u8 timer_led) {}
#endif

/* -------------------------------------------------------------------- */

/*
 * These devices are always present and don't need any board-specific
 * setup.
 */
static int __init ks8695_add_standard_devices(void)
{
	ks8695_add_device_watchdog();
	return 0;
}

arch_initcall(ks8695_add_standard_devices);
