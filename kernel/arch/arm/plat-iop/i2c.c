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
 * arch/arm/plat-iop/i2c.c
 *
 * Author: Nicolas Pitre <nico@cam.org>
 * Copyright (C) 2001 MontaVista Software, Inc.
 * Copyright (C) 2004 Intel Corporation.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/mm.h>
#include <linux/init.h>
#include <linux/major.h>
#include <linux/fs.h>
#include <linux/platform_device.h>
#include <linux/serial.h>
#include <linux/tty.h>
#include <linux/serial_core.h>
#include <linux/io.h>
#include <asm/pgtable.h>
#include <asm/page.h>
#include <asm/mach/map.h>
#include <asm/setup.h>
#include <asm/system.h>
#include <asm/memory.h>
#include <mach/hardware.h>
#include <asm/hardware/iop3xx.h>
#include <asm/mach/arch.h>

#ifdef CONFIG_ARCH_IOP32X
#define IRQ_IOP3XX_I2C_0	IRQ_IOP32X_I2C_0
#define IRQ_IOP3XX_I2C_1	IRQ_IOP32X_I2C_1
#endif
#ifdef CONFIG_ARCH_IOP33X
#define IRQ_IOP3XX_I2C_0	IRQ_IOP33X_I2C_0
#define IRQ_IOP3XX_I2C_1	IRQ_IOP33X_I2C_1
#endif

static struct resource iop3xx_i2c0_resources[] = {
	[0] = {
		.start	= 0xfffff680,
		.end	= 0xfffff697,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= IRQ_IOP3XX_I2C_0,
		.end	= IRQ_IOP3XX_I2C_0,
		.flags	= IORESOURCE_IRQ,
	},
};

struct platform_device iop3xx_i2c0_device = {
	.name		= "IOP3xx-I2C",
	.id		= 0,
	.num_resources	= 2,
	.resource	= iop3xx_i2c0_resources,
};


static struct resource iop3xx_i2c1_resources[] = {
	[0] = {
		.start	= 0xfffff6a0,
		.end	= 0xfffff6b7,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= IRQ_IOP3XX_I2C_1,
		.end	= IRQ_IOP3XX_I2C_1,
		.flags	= IORESOURCE_IRQ,
	}
};

struct platform_device iop3xx_i2c1_device = {
	.name		= "IOP3xx-I2C",
	.id		= 1,
	.num_resources	= 2,
	.resource	= iop3xx_i2c1_resources,
};
