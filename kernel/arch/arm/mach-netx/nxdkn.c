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
 * arch/arm/mach-netx/nxdkn.c
 *
 * Copyright (c) 2005 Sascha Hauer <s.hauer@pengutronix.de>, Pengutronix
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
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

#include <linux/dma-mapping.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/mtd/plat-ram.h>
#include <linux/platform_device.h>
#include <linux/amba/bus.h>
#include <linux/amba/clcd.h>

#include <mach/hardware.h>
#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <mach/netx-regs.h>
#include <mach/eth.h>

#include "generic.h"

static struct netxeth_platform_data eth0_platform_data = {
	.xcno = 0,
};

static struct platform_device nxdkn_eth0_device = {
	.name		= "netx-eth",
	.id		= 0,
	.num_resources	= 0,
	.resource	= NULL,
	.dev = {
		.platform_data = &eth0_platform_data,
	}
};

static struct netxeth_platform_data eth1_platform_data = {
	.xcno = 1,
};

static struct platform_device nxdkn_eth1_device = {
	.name		= "netx-eth",
	.id		= 1,
	.num_resources	= 0,
	.resource	= NULL,
	.dev = {
		.platform_data = &eth1_platform_data,
	}
};

static struct resource netx_uart0_resources[] = {
	[0] = {
		.start	= 0x00100A00,
		.end	= 0x00100A3F,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= (NETX_IRQ_UART0),
		.end	= (NETX_IRQ_UART0),
		.flags	= IORESOURCE_IRQ,
	},
};

static struct platform_device netx_uart0_device = {
	.name		= "netx-uart",
	.id		= 0,
	.num_resources	= ARRAY_SIZE(netx_uart0_resources),
	.resource	= netx_uart0_resources,
};

static struct platform_device *devices[] __initdata = {
	&nxdkn_eth0_device,
	&nxdkn_eth1_device,
	&netx_uart0_device,
};

static void __init nxdkn_init(void)
{
	platform_add_devices(devices, ARRAY_SIZE(devices));
}

MACHINE_START(NXDKN, "Hilscher nxdkn")
	.phys_io	= 0x00100000,
	.io_pg_offst	= (io_p2v(0x00100000) >> 18) & 0xfffc,
	.boot_params	= 0x80000100,
	.map_io		= netx_map_io,
	.init_irq	= netx_init_irq,
	.timer		= &netx_timer,
	.init_machine	= nxdkn_init,
MACHINE_END
