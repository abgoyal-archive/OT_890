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
 *  linux/arch/arm/mach-pxa/mp900.c
 *
 *  Support for the NEC MobilePro900/C platform
 *
 *  Based on mach-pxa/gumstix.c
 *
 *  2007, 2008 Kristoffer Ericson <kristoffer.ericson@gmail.com>
 *  2007, 2008 Michael Petchkovsky <mkpetch@internode.on.net>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 */

#include <linux/init.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/types.h>
#include <linux/usb/isp116x.h>

#include <mach/hardware.h>
#include <mach/pxa-regs.h>
#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include "generic.h"

static void isp116x_pfm_delay(struct device *dev, int delay)
{

	/* 400Mhz PXA2 = 2.5ns / instruction */

	int cyc = delay / 10;

	/* 4 Instructions = 4 x 2.5ns = 10ns */
	__asm__ volatile ("0:\n"
		"subs %0, %1, #1\n"
		"bge 0b\n"
		:"=r" (cyc)
		:"0"(cyc)
	);
}

static struct isp116x_platform_data isp116x_pfm_data = {
	.remote_wakeup_enable = 1,
	.delay = isp116x_pfm_delay,
};

static struct resource isp116x_pfm_resources[] = {
	[0] =	{
		.start	= 0x0d000000,
		.end	= 0x0d000000 + 1,
		.flags	= IORESOURCE_MEM,
		},
	[1] =	{
		.start  = 0x0d000000 + 4,
		.end	= 0x0d000000 + 5,
		.flags  = IORESOURCE_MEM,
		},
	[2] =	{
		.start	= 61,
		.end	= 61,
		.flags	= IORESOURCE_IRQ,
		},
};

static struct platform_device mp900c_dummy_device = {
	.name		= "mp900c_dummy",
	.id		= -1,
};

static struct platform_device mp900c_usb = {
	.name		= "isp116x-hcd",
	.num_resources	= ARRAY_SIZE(isp116x_pfm_resources),
	.resource	= isp116x_pfm_resources,
	.dev.platform_data = &isp116x_pfm_data,
};

static struct platform_device *devices[] __initdata = {
	&mp900c_dummy_device,
	&mp900c_usb,
};

static void __init mp900c_init(void)
{
	printk(KERN_INFO "MobilePro 900/C machine init\n");
	platform_add_devices(devices, ARRAY_SIZE(devices));
}

/* Maintainer - Michael Petchkovsky <mkpetch@internode.on.net> */
MACHINE_START(NEC_MP900, "MobilePro900/C")
	.phys_io	= 0x40000000,
	.boot_params	= 0xa0220100,
	.io_pg_offst	= (io_p2v(0x40000000) >> 18) & 0xfffc,
	.timer		= &pxa_timer,
	.map_io		= pxa_map_io,
	.init_irq	= pxa25x_init_irq,
	.init_machine	= mp900c_init,
MACHINE_END

