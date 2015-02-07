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
 * Hardware definitions for the Toshiba eseries PDAs
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
#include <linux/mfd/t7l66xb.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/partitions.h>

#include <asm/setup.h>
#include <asm/mach/arch.h>
#include <asm/mach-types.h>

#include <mach/mfp-pxa25x.h>
#include <mach/pxa-regs.h>
#include <mach/hardware.h>
#include <mach/eseries-gpio.h>
#include <mach/pxafb.h>
#include <mach/udc.h>
#include <mach/irqs.h>

#include "generic.h"
#include "eseries.h"
#include "clock.h"

/* ------------------------ E400 LCD definitions ------------------------ */

static struct pxafb_mode_info e400_pxafb_mode_info = {
	.pixclock       = 140703,
	.xres           = 240,
	.yres           = 320,
	.bpp            = 16,
	.hsync_len      = 4,
	.left_margin    = 28,
	.right_margin   = 8,
	.vsync_len      = 3,
	.upper_margin   = 5,
	.lower_margin   = 6,
	.sync           = 0,
};

static struct pxafb_mach_info e400_pxafb_mach_info = {
	.modes          = &e400_pxafb_mode_info,
	.num_modes      = 1,
	.lcd_conn	= LCD_COLOR_TFT_16BPP,
	.lccr3          = 0,
	.pxafb_backlight_power  = NULL,
};

/* ------------------------ E400 MFP config ----------------------------- */

static unsigned long e400_pin_config[] __initdata = {
	/* Chip selects */
	GPIO15_nCS_1,   /* CS1 - Flash */
	GPIO80_nCS_4,   /* CS4 - TMIO */

	/* Clocks */
	GPIO12_32KHz,

	/* BTUART */
	GPIO42_BTUART_RXD,
	GPIO43_BTUART_TXD,
	GPIO44_BTUART_CTS,

	/* TMIO controller */
	GPIO19_GPIO, /* t7l66xb #PCLR */
	GPIO45_GPIO, /* t7l66xb #SUSPEND (NOT BTUART!) */

	/* wakeup */
	GPIO0_GPIO | WAKEUP_ON_EDGE_RISE,
};

/* ---------------------------------------------------------------------- */

static struct mtd_partition partition_a = {
	.name = "Internal NAND flash",
	.offset =  0,
	.size =  MTDPART_SIZ_FULL,
};

static uint8_t scan_ff_pattern[] = { 0xff, 0xff };

static struct nand_bbt_descr e400_t7l66xb_nand_bbt = {
	.options = 0,
	.offs = 4,
	.len = 2,
	.pattern = scan_ff_pattern
};

static struct tmio_nand_data e400_t7l66xb_nand_config = {
	.num_partitions = 1,
	.partition = &partition_a,
	.badblock_pattern = &e400_t7l66xb_nand_bbt,
};

static struct t7l66xb_platform_data e400_t7l66xb_info = {
	.irq_base 		= IRQ_BOARD_START,
	.enable                 = &eseries_tmio_enable,
	.suspend                = &eseries_tmio_suspend,
	.resume                 = &eseries_tmio_resume,

	.nand_data              = &e400_t7l66xb_nand_config,
};

static struct platform_device e400_t7l66xb_device = {
	.name           = "t7l66xb",
	.id             = -1,
	.dev            = {
		.platform_data = &e400_t7l66xb_info,
	},
	.num_resources = 2,
	.resource      = eseries_tmio_resources,
};

/* ---------------------------------------------------------- */

static struct platform_device *devices[] __initdata = {
	&e400_t7l66xb_device,
};

static void __init e400_init(void)
{
	pxa2xx_mfp_config(ARRAY_AND_SIZE(e400_pin_config));
	/* Fixme - e400 may have a switched clock */
	eseries_register_clks();
	eseries_get_tmio_gpios();
	set_pxa_fb_info(&e400_pxafb_mach_info);
	platform_add_devices(devices, ARRAY_SIZE(devices));
	pxa_set_udc_info(&e7xx_udc_mach_info);
}

MACHINE_START(E400, "Toshiba e400")
	/* Maintainer: Ian Molton (spyro@f2s.com) */
	.phys_io	= 0x40000000,
	.io_pg_offst	= (io_p2v(0x40000000) >> 18) & 0xfffc,
	.boot_params	= 0xa0000100,
	.map_io		= pxa_map_io,
	.init_irq	= pxa25x_init_irq,
	.fixup		= eseries_fixup,
	.init_machine	= e400_init,
	.timer		= &pxa_timer,
MACHINE_END

