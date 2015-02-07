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
 * linux/arch/arm/plat-omap/debug-devices.c
 *
 * Copyright (C) 2005 Nokia Corporation
 * Modified from mach-omap2/board-h4.c
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/io.h>

#include <mach/hardware.h>

#include <mach/board.h>
#include <mach/gpio.h>


/* Many OMAP development platforms reuse the same "debug board"; these
 * platforms include H2, H3, H4, and Perseus2.
 */

static struct resource smc91x_resources[] = {
	[0] = {
		.flags  = IORESOURCE_MEM,
	},
	[1] = {
		.flags  = IORESOURCE_IRQ | IORESOURCE_IRQ_LOWEDGE,
	},
};

static struct platform_device smc91x_device = {
	.name		= "smc91x",
	.id		= -1,
	.num_resources	= ARRAY_SIZE(smc91x_resources),
	.resource	= smc91x_resources,
};

static struct resource led_resources[] = {
	[0] = {
		.flags	= IORESOURCE_MEM,
	},
};

static struct platform_device led_device = {
	.name		= "omap_dbg_led",
	.id		= -1,
	.num_resources	= ARRAY_SIZE(led_resources),
	.resource	= led_resources,
};

static struct platform_device *debug_devices[] __initdata = {
	&smc91x_device,
	&led_device,
	/* ps2 kbd + mouse ports */
	/* 4 extra uarts */
	/* 6 input dip switches */
	/* 8 output pins */
};

int __init debug_card_init(u32 addr, unsigned gpio)
{
	int	status;

	smc91x_resources[0].start = addr + 0x300;
	smc91x_resources[0].end   = addr + 0x30f;

	smc91x_resources[1].start = gpio_to_irq(gpio);
	smc91x_resources[1].end   = gpio_to_irq(gpio);

	status = gpio_request(gpio, "SMC91x irq");
	if (status < 0) {
		printk(KERN_ERR "GPIO%d unavailable for smc91x IRQ\n", gpio);
		return status;
	}
	gpio_direction_input(gpio);

	led_resources[0].start = addr;
	led_resources[0].end   = addr + SZ_4K - 1;

	return platform_add_devices(debug_devices, ARRAY_SIZE(debug_devices));
}
