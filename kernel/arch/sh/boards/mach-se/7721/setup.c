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
 * linux/arch/sh/boards/se/7721/setup.c
 *
 * Copyright (C) 2008 Renesas Solutions Corp.
 *
 * Hitachi UL SolutionEngine 7721 Support.
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 */
#include <linux/init.h>
#include <linux/platform_device.h>
#include <mach-se/mach/se7721.h>
#include <mach-se/mach/mrshpc.h>
#include <asm/machvec.h>
#include <asm/io.h>
#include <asm/heartbeat.h>

static unsigned char heartbeat_bit_pos[] = { 8, 9, 10, 11, 12, 13, 14, 15 };

static struct heartbeat_data heartbeat_data = {
	.bit_pos	= heartbeat_bit_pos,
	.nr_bits	= ARRAY_SIZE(heartbeat_bit_pos),
	.regsize	= 16,
};

static struct resource heartbeat_resources[] = {
	[0] = {
		.start	= PA_LED,
		.end	= PA_LED,
		.flags	= IORESOURCE_MEM,
	},
};

static struct platform_device heartbeat_device = {
	.name		= "heartbeat",
	.id		= -1,
	.dev	= {
		.platform_data	= &heartbeat_data,
	},
	.num_resources	= ARRAY_SIZE(heartbeat_resources),
	.resource	= heartbeat_resources,
};

static struct resource cf_ide_resources[] = {
	[0] = {
		.start	= PA_MRSHPC_IO + 0x1f0,
		.end	= PA_MRSHPC_IO + 0x1f0 + 8 ,
		.flags	= IORESOURCE_IO,
	},
	[1] = {
		.start	= PA_MRSHPC_IO + 0x1f0 + 0x206,
		.end	= PA_MRSHPC_IO + 0x1f0 + 8 + 0x206 + 8,
		.flags	= IORESOURCE_IO,
	},
	[2] = {
		.start  = MRSHPC_IRQ0,
		.flags  = IORESOURCE_IRQ,
	},
};

static struct platform_device cf_ide_device = {
	.name		= "pata_platform",
	.id		= -1,
	.num_resources	= ARRAY_SIZE(cf_ide_resources),
	.resource	= cf_ide_resources,
};

static struct platform_device *se7721_devices[] __initdata = {
	&cf_ide_device,
	&heartbeat_device
};

static int __init se7721_devices_setup(void)
{
	mrshpc_setup_windows();
	return platform_add_devices(se7721_devices, ARRAY_SIZE(se7721_devices));
}
device_initcall(se7721_devices_setup);

static void __init se7721_setup(char **cmdline_p)
{
	/* for USB */
	ctrl_outw(0x0000, 0xA405010C);	/* PGCR */
	ctrl_outw(0x0000, 0xA405010E);	/* PHCR */
	ctrl_outw(0x00AA, 0xA4050118);	/* PPCR */
	ctrl_outw(0x0000, 0xA4050124);	/* PSELA */
}

/*
 * The Machine Vector
 */
struct sh_machine_vector mv_se7721 __initmv = {
	.mv_name		= "Solution Engine 7721",
	.mv_setup		= se7721_setup,
	.mv_nr_irqs		= 109,
	.mv_init_irq		= init_se7721_IRQ,
};
