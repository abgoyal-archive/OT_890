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
 * Hardware definitions for the Toshiba e330 PDAs
 *
 * Copyright (c) 2003 Ian Molton <spyro@f2s.com>
 *
 * This file is licensed under
 * the terms of the GNU General Public License version 2. This program
 * is licensed "as is" without any warranty of any kind, whether express
 * or implied.
 *
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/clk.h>
#include <linux/platform_device.h>
#include <linux/mfd/tc6387xb.h>

#include <asm/setup.h>
#include <asm/mach/arch.h>
#include <asm/mach-types.h>

#include <mach/mfp-pxa25x.h>
#include <mach/hardware.h>
#include <mach/pxa-regs.h>
#include <mach/eseries-gpio.h>
#include <mach/udc.h>

#include "generic.h"
#include "eseries.h"
#include "clock.h"

/* -------------------- e330 tc6387xb parameters -------------------- */

static struct tc6387xb_platform_data e330_tc6387xb_info = {
	.enable   = &eseries_tmio_enable,
	.disable  = &eseries_tmio_disable,
	.suspend  = &eseries_tmio_suspend,
	.resume   = &eseries_tmio_resume,
};

static struct platform_device e330_tc6387xb_device = {
	.name           = "tc6387xb",
	.id             = -1,
	.dev            = {
		.platform_data = &e330_tc6387xb_info,
	},
	.num_resources = 2,
	.resource      = eseries_tmio_resources,
};

/* --------------------------------------------------------------- */

static struct platform_device *devices[] __initdata = {
	&e330_tc6387xb_device,
};

static void __init e330_init(void)
{
	eseries_register_clks();
	eseries_get_tmio_gpios();
	platform_add_devices(devices, ARRAY_SIZE(devices));
	pxa_set_udc_info(&e7xx_udc_mach_info);
}

MACHINE_START(E330, "Toshiba e330")
	/* Maintainer: Ian Molton (spyro@f2s.com) */
	.phys_io	= 0x40000000,
	.io_pg_offst	= (io_p2v(0x40000000) >> 18) & 0xfffc,
	.boot_params	= 0xa0000100,
	.map_io		= pxa_map_io,
	.init_irq	= pxa25x_init_irq,
	.fixup		= eseries_fixup,
	.init_machine	= e330_init,
	.timer		= &pxa_timer,
MACHINE_END

