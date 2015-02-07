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
 * linux/arch/arm/mach-w90x900/mach-w90p910evb.c
 *
 * Based on mach-s3c2410/mach-smdk2410.c by Jonas Dietsche
 *
 * Copyright (C) 2008 Nuvoton technology corporation
 * All rights reserved.
 *
 * Wan ZongShun <mcuos.com@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 */

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/interrupt.h>
#include <linux/list.h>
#include <linux/timer.h>
#include <linux/init.h>
#include <linux/platform_device.h>

#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <asm/mach/irq.h>
#include <asm/mach-types.h>

#include <mach/regs-serial.h>
#include <mach/map.h>

#include "cpu.h"

static struct map_desc w90p910_iodesc[] __initdata = {
};

static struct w90x900_uartcfg w90p910_uartcfgs[] = {
	W90X900_UARTCFG(0, 0, 0, 0, 0),
	W90X900_UARTCFG(1, 0, 0, 0, 0),
	W90X900_UARTCFG(2, 0, 0, 0, 0),
	W90X900_UARTCFG(3, 0, 0, 0, 0),
	W90X900_UARTCFG(4, 0, 0, 0, 0),
};

/*Here should be your evb resourse,such as LCD*/

static struct platform_device *w90p910evb_dev[] __initdata = {
};

static void __init w90p910evb_map_io(void)
{
	w90p910_map_io(w90p910_iodesc, ARRAY_SIZE(w90p910_iodesc));
	w90p910_init_clocks(0);
	w90p910_init_uarts(w90p910_uartcfgs, ARRAY_SIZE(w90p910_uartcfgs));
}

static void __init w90p910evb_init(void)
{
	platform_add_devices(w90p910evb_dev, ARRAY_SIZE(w90p910evb_dev));
}

MACHINE_START(W90P910EVB, "W90P910EVB")
	/* Maintainer: Wan ZongShun */
	.phys_io	= W90X900_PA_UART,
	.io_pg_offst	= (((u32)W90X900_VA_UART) >> 18) & 0xfffc,
	.boot_params	= 0,
	.map_io		= w90p910evb_map_io,
	.init_irq	= w90x900_init_irq,
	.init_machine	= w90p910evb_init,
	.timer		= &w90x900_timer,
MACHINE_END
