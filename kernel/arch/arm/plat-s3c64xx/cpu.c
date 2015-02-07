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

/* linux/arch/arm/plat-s3c64xx/cpu.c
 *
 * Copyright 2008 Openmoko, Inc.
 * Copyright 2008 Simtec Electronics
 *	Ben Dooks <ben@simtec.co.uk>
 *	http://armlinux.simtec.co.uk/
 *
 * S3C64XX CPU Support
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/serial_core.h>
#include <linux/platform_device.h>
#include <linux/io.h>

#include <mach/hardware.h>
#include <mach/map.h>

#include <asm/mach/arch.h>
#include <asm/mach/map.h>

#include <plat/regs-serial.h>

#include <plat/cpu.h>
#include <plat/devs.h>
#include <plat/clock.h>

#include <plat/s3c6400.h>
#include <plat/s3c6410.h>

/* table of supported CPUs */

static const char name_s3c6400[] = "S3C6400";
static const char name_s3c6410[] = "S3C6410";

static struct cpu_table cpu_ids[] __initdata = {
	{
		.idcode		= 0x36400000,
		.idmask		= 0xfffff000,
		.map_io		= s3c6400_map_io,
		.init_clocks	= s3c6400_init_clocks,
		.init_uarts	= s3c6400_init_uarts,
		.init		= s3c6400_init,
		.name		= name_s3c6400,
	}, {
		.idcode		= 0x36410100,
		.idmask		= 0xffffff00,
		.map_io		= s3c6410_map_io,
		.init_clocks	= s3c6410_init_clocks,
		.init_uarts	= s3c6410_init_uarts,
		.init		= s3c6410_init,
		.name		= name_s3c6410,
	},
};

/* minimal IO mapping */

/* see notes on uart map in arch/arm/mach-s3c6400/include/mach/debug-macro.S */
#define UART_OFFS (S3C_PA_UART & 0xfffff)

static struct map_desc s3c_iodesc[] __initdata = {
	{
		.virtual	= (unsigned long)S3C_VA_SYS,
		.pfn		= __phys_to_pfn(S3C64XX_PA_SYSCON),
		.length		= SZ_4K,
		.type		= MT_DEVICE,
	}, {
		.virtual	= (unsigned long)(S3C_VA_UART + UART_OFFS),
		.pfn		= __phys_to_pfn(S3C_PA_UART),
		.length		= SZ_4K,
		.type		= MT_DEVICE,
	}, {
		.virtual	= (unsigned long)S3C_VA_VIC0,
		.pfn		= __phys_to_pfn(S3C64XX_PA_VIC0),
		.length		= SZ_16K,
		.type		= MT_DEVICE,
	}, {
		.virtual	= (unsigned long)S3C_VA_VIC1,
		.pfn		= __phys_to_pfn(S3C64XX_PA_VIC1),
		.length		= SZ_16K,
		.type		= MT_DEVICE,
	}, {
		.virtual	= (unsigned long)S3C_VA_TIMER,
		.pfn		= __phys_to_pfn(S3C_PA_TIMER),
		.length		= SZ_16K,
		.type		= MT_DEVICE,
	}, {
		.virtual	= (unsigned long)S3C64XX_VA_GPIO,
		.pfn		= __phys_to_pfn(S3C64XX_PA_GPIO),
		.length		= SZ_4K,
		.type		= MT_DEVICE,
	},
};

/* read cpu identification code */

void __init s3c64xx_init_io(struct map_desc *mach_desc, int size)
{
	unsigned long idcode;

	/* initialise the io descriptors we need for initialisation */
	iotable_init(s3c_iodesc, ARRAY_SIZE(s3c_iodesc));
	iotable_init(mach_desc, size);

	idcode = __raw_readl(S3C_VA_SYS + 0x118);
	s3c_init_cpu(idcode, cpu_ids, ARRAY_SIZE(cpu_ids));
}
