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
 * arch/sh/boards/renesas/x3proto/setup.c
 *
 * Renesas SH-X3 Prototype Board Support.
 *
 * Copyright (C) 2007 - 2008 Paul Mundt
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/kernel.h>
#include <linux/io.h>
#include <linux/smc91x.h>
#include <linux/irq.h>
#include <asm/ilsel.h>

static struct resource heartbeat_resources[] = {
	[0] = {
		.start	= 0xb8140020,
		.end	= 0xb8140020,
		.flags	= IORESOURCE_MEM,
	},
};

static struct platform_device heartbeat_device = {
	.name		= "heartbeat",
	.id		= -1,
	.num_resources	= ARRAY_SIZE(heartbeat_resources),
	.resource	= heartbeat_resources,
};

static struct smc91x_platdata smc91x_info = {
	.flags	= SMC91X_USE_16BIT | SMC91X_NOWAIT,
};

static struct resource smc91x_resources[] = {
	[0] = {
		.start		= 0x18000300,
		.end		= 0x18000300 + 0x10 - 1,
		.flags		= IORESOURCE_MEM,
	},
	[1] = {
		/* Filled in by ilsel */
		.flags		= IORESOURCE_IRQ,
	},
};

static struct platform_device smc91x_device = {
	.name		= "smc91x",
	.id		= -1,
	.resource	= smc91x_resources,
	.num_resources	= ARRAY_SIZE(smc91x_resources),
	.dev	= {
		.platform_data = &smc91x_info,
	},
};

static struct resource r8a66597_usb_host_resources[] = {
	[0] = {
		.name	= "r8a66597_hcd",
		.start	= 0x18040000,
		.end	= 0x18080000 - 1,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.name	= "r8a66597_hcd",
		/* Filled in by ilsel */
		.flags	= IORESOURCE_IRQ,
	},
};

static struct platform_device r8a66597_usb_host_device = {
	.name		= "r8a66597_hcd",
	.id		= -1,
	.dev = {
		.dma_mask		= NULL,		/* don't use dma */
		.coherent_dma_mask	= 0xffffffff,
	},
	.num_resources	= ARRAY_SIZE(r8a66597_usb_host_resources),
	.resource	= r8a66597_usb_host_resources,
};

static struct resource m66592_usb_peripheral_resources[] = {
	[0] = {
		.name	= "m66592_udc",
		.start	= 0x18080000,
		.end	= 0x180c0000 - 1,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.name	= "m66592_udc",
		/* Filled in by ilsel */
		.flags	= IORESOURCE_IRQ,
	},
};

static struct platform_device m66592_usb_peripheral_device = {
	.name		= "m66592_udc",
	.id		= -1,
	.dev = {
		.dma_mask		= NULL,		/* don't use dma */
		.coherent_dma_mask	= 0xffffffff,
	},
	.num_resources	= ARRAY_SIZE(m66592_usb_peripheral_resources),
	.resource	= m66592_usb_peripheral_resources,
};

static struct platform_device *x3proto_devices[] __initdata = {
	&heartbeat_device,
	&smc91x_device,
	&r8a66597_usb_host_device,
	&m66592_usb_peripheral_device,
};

static int __init x3proto_devices_setup(void)
{
	r8a66597_usb_host_resources[1].start =
		r8a66597_usb_host_resources[1].end = ilsel_enable(ILSEL_USBH_I);

	m66592_usb_peripheral_resources[1].start =
		m66592_usb_peripheral_resources[1].end = ilsel_enable(ILSEL_USBP_I);

	smc91x_resources[1].start =
		smc91x_resources[1].end = ilsel_enable(ILSEL_LAN);

	return platform_add_devices(x3proto_devices,
				    ARRAY_SIZE(x3proto_devices));
}
device_initcall(x3proto_devices_setup);

static void __init x3proto_init_irq(void)
{
	plat_irq_setup_pins(IRQ_MODE_IRL3210);

	/* Set ICR0.LVLMODE */
	ctrl_outl(ctrl_inl(0xfe410000) | (1 << 21), 0xfe410000);
}

static struct sh_machine_vector mv_x3proto __initmv = {
	.mv_name		= "x3proto",
	.mv_init_irq		= x3proto_init_irq,
};
