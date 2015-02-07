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
 * Renesas Technology Europe RSK+ 7203 Support.
 *
 * Copyright (C) 2008 Paul Mundt
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */
#include <linux/init.h>
#include <linux/types.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/partitions.h>
#include <linux/mtd/physmap.h>
#include <linux/mtd/map.h>
#include <linux/smsc911x.h>
#include <linux/gpio.h>
#include <linux/leds.h>
#include <asm/machvec.h>
#include <asm/io.h>
#include <cpu/sh7203.h>

static struct smsc911x_platform_config smsc911x_config = {
	.phy_interface	= PHY_INTERFACE_MODE_MII,
	.irq_polarity	= SMSC911X_IRQ_POLARITY_ACTIVE_LOW,
	.irq_type	= SMSC911X_IRQ_TYPE_OPEN_DRAIN,
	.flags		= SMSC911X_USE_16BIT,
};

static struct resource smsc911x_resources[] = {
	[0] = {
		.start		= 0x24000000,
		.end		= 0x24000000 + 0x100,
		.flags		= IORESOURCE_MEM,
	},
	[1] = {
		.start		= 64,
		.end		= 64,
		.flags		= IORESOURCE_IRQ,
	},
};

static struct platform_device smsc911x_device = {
	.name		= "smsc911x",
	.id		= -1,
	.num_resources	= ARRAY_SIZE(smsc911x_resources),
	.resource	= smsc911x_resources,
	.dev		= {
		.platform_data = &smsc911x_config,
	},
};

static struct gpio_led rsk7203_gpio_leds[] = {
	{
		.name			= "green",
		.gpio			= GPIO_PE10,
		.active_low		= 1,
	}, {
		.name			= "orange",
		.default_trigger	= "nand-disk",
		.gpio			= GPIO_PE12,
		.active_low		= 1,
	}, {
		.name			= "red:timer",
		.default_trigger	= "timer",
		.gpio			= GPIO_PC14,
		.active_low		= 1,
	}, {
		.name			= "red:heartbeat",
		.default_trigger	= "heartbeat",
		.gpio			= GPIO_PE11,
		.active_low		= 1,
	},
};

static struct gpio_led_platform_data rsk7203_gpio_leds_info = {
	.leds		= rsk7203_gpio_leds,
	.num_leds	= ARRAY_SIZE(rsk7203_gpio_leds),
};

static struct platform_device led_device = {
	.name		= "leds-gpio",
	.id		= -1,
	.dev		= {
		.platform_data	= &rsk7203_gpio_leds_info,
	},
};

static struct platform_device *rsk7203_devices[] __initdata = {
	&smsc911x_device,
	&led_device,
};

static int __init rsk7203_devices_setup(void)
{
	/* Select pins for SCIF0 */
	gpio_request(GPIO_FN_TXD0, NULL);
	gpio_request(GPIO_FN_RXD0, NULL);

	return platform_add_devices(rsk7203_devices,
				    ARRAY_SIZE(rsk7203_devices));
}
device_initcall(rsk7203_devices_setup);
