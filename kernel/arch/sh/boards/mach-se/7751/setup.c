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
 * linux/arch/sh/boards/se/7751/setup.c
 *
 * Copyright (C) 2000  Kazumoto Kojima
 *
 * Hitachi SolutionEngine Support.
 *
 * Modified for 7751 Solution Engine by
 * Ian da Silva and Jeremy Siegel, 2001.
 */
#include <linux/init.h>
#include <linux/platform_device.h>
#include <asm/machvec.h>
#include <mach-se/mach/se7751.h>
#include <asm/io.h>
#include <asm/heartbeat.h>

static unsigned char heartbeat_bit_pos[] = { 8, 9, 10, 11, 12, 13, 14, 15 };

static struct heartbeat_data heartbeat_data = {
	.bit_pos	= heartbeat_bit_pos,
	.nr_bits	= ARRAY_SIZE(heartbeat_bit_pos),
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

static struct platform_device *se7751_devices[] __initdata = {
	&heartbeat_device,
};

static int __init se7751_devices_setup(void)
{
	return platform_add_devices(se7751_devices, ARRAY_SIZE(se7751_devices));
}
__initcall(se7751_devices_setup);

/*
 * The Machine Vector
 */
static struct sh_machine_vector mv_7751se __initmv = {
	.mv_name		= "7751 SolutionEngine",
	.mv_nr_irqs		= 72,

	.mv_inb			= sh7751se_inb,
	.mv_inw			= sh7751se_inw,
	.mv_inl			= sh7751se_inl,
	.mv_outb		= sh7751se_outb,
	.mv_outw		= sh7751se_outw,
	.mv_outl		= sh7751se_outl,

	.mv_inb_p		= sh7751se_inb_p,
	.mv_inw_p		= sh7751se_inw,
	.mv_inl_p		= sh7751se_inl,
	.mv_outb_p		= sh7751se_outb_p,
	.mv_outw_p		= sh7751se_outw,
	.mv_outl_p		= sh7751se_outl,

	.mv_insl		= sh7751se_insl,
	.mv_outsl		= sh7751se_outsl,

	.mv_init_irq		= init_7751se_IRQ,
};
