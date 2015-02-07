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
 * Board-specific setup code for the ATEVKLCD10X addon board to the ATNGW100
 * Network Gateway
 *
 * Copyright (C) 2008 Atmel Corporation
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 */

#include <linux/init.h>
#include <linux/linkage.h>
#include <linux/fb.h>
#include <linux/platform_device.h>

#include <video/atmel_lcdc.h>

#include <asm/setup.h>

#include <mach/at32ap700x.h>
#include <mach/board.h>

static struct ac97c_platform_data __initdata ac97c0_data = {
	.dma_rx_periph_id	= 3,
	.dma_tx_periph_id	= 4,
	.dma_controller_id	= 0,
	.reset_pin		= GPIO_PIN_PB(19),
};

#ifdef CONFIG_BOARD_ATNGW100_EVKLCD10X_VGA
static struct fb_videomode __initdata tcg057vglad_modes[] = {
	{
		.name		= "640x480 @ 60",
		.refresh	= 60,
		.xres		= 640,		.yres		= 480,
		.pixclock	= KHZ2PICOS(25180),

		.left_margin	= 64,		.right_margin	= 31,
		.upper_margin	= 34,		.lower_margin	= 2,
		.hsync_len	= 96,		.vsync_len	= 4,

		.sync		= 0,
		.vmode		= FB_VMODE_NONINTERLACED,
	},
};

static struct fb_monspecs __initdata atevklcd10x_default_monspecs = {
	.manufacturer		= "KYO",
	.monitor		= "TCG057VGLAD",
	.modedb			= tcg057vglad_modes,
	.modedb_len		= ARRAY_SIZE(tcg057vglad_modes),
	.hfmin			= 19948,
	.hfmax			= 31478,
	.vfmin			= 50,
	.vfmax			= 67,
	.dclkmax		= 28330000,
};

static struct atmel_lcdfb_info __initdata atevklcd10x_lcdc_data = {
	.default_bpp		= 16,
	.default_dmacon		= ATMEL_LCDC_DMAEN | ATMEL_LCDC_DMA2DEN,
	.default_lcdcon2	= (ATMEL_LCDC_DISTYPE_TFT
				   | ATMEL_LCDC_CLKMOD_ALWAYSACTIVE
				   | ATMEL_LCDC_MEMOR_BIG),
	.default_monspecs	= &atevklcd10x_default_monspecs,
	.guard_time		= 2,
};
#elif CONFIG_BOARD_ATNGW100_EVKLCD10X_QVGA
static struct fb_videomode __initdata tcg057qvlad_modes[] = {
	{
		.name		= "320x240 @ 60",
		.refresh	= 60,
		.xres		= 320,		.yres		= 240,
		.pixclock	= KHZ2PICOS(6300),

		.left_margin	= 52,		.right_margin	= 28,
		.upper_margin	= 7,		.lower_margin	= 2,
		.hsync_len	= 96,		.vsync_len	= 4,

		.sync		= 0,
		.vmode		= FB_VMODE_NONINTERLACED,
	},
};

static struct fb_monspecs __initdata atevklcd10x_default_monspecs = {
	.manufacturer		= "KYO",
	.monitor		= "TCG057QVLAD",
	.modedb			= tcg057qvlad_modes,
	.modedb_len		= ARRAY_SIZE(tcg057qvlad_modes),
	.hfmin			= 19948,
	.hfmax			= 31478,
	.vfmin			= 50,
	.vfmax			= 67,
	.dclkmax		= 7000000,
};

static struct atmel_lcdfb_info __initdata atevklcd10x_lcdc_data = {
	.default_bpp		= 16,
	.default_dmacon		= ATMEL_LCDC_DMAEN | ATMEL_LCDC_DMA2DEN,
	.default_lcdcon2	= (ATMEL_LCDC_DISTYPE_TFT
				   | ATMEL_LCDC_CLKMOD_ALWAYSACTIVE
				   | ATMEL_LCDC_MEMOR_BIG),
	.default_monspecs	= &atevklcd10x_default_monspecs,
	.guard_time		= 2,
};
#elif CONFIG_BOARD_ATNGW100_EVKLCD10X_POW_QVGA
static struct fb_videomode __initdata ph320240t_modes[] = {
	{
		.name		= "320x240 @ 60",
		.refresh	= 60,
		.xres		= 320,		.yres		= 240,
		.pixclock	= KHZ2PICOS(6300),

		.left_margin	= 38,		.right_margin	= 20,
		.upper_margin	= 15,		.lower_margin	= 5,
		.hsync_len	= 30,		.vsync_len	= 3,

		.sync		= 0,
		.vmode		= FB_VMODE_NONINTERLACED,
	},
};

static struct fb_monspecs __initdata atevklcd10x_default_monspecs = {
	.manufacturer		= "POW",
	.monitor		= "PH320240T",
	.modedb			= ph320240t_modes,
	.modedb_len		= ARRAY_SIZE(ph320240t_modes),
	.hfmin			= 14400,
	.hfmax			= 21600,
	.vfmin			= 50,
	.vfmax			= 90,
	.dclkmax		= 6400000,
};

static struct atmel_lcdfb_info __initdata atevklcd10x_lcdc_data = {
	.default_bpp		= 16,
	.default_dmacon		= ATMEL_LCDC_DMAEN | ATMEL_LCDC_DMA2DEN,
	.default_lcdcon2	= (ATMEL_LCDC_DISTYPE_TFT
				   | ATMEL_LCDC_CLKMOD_ALWAYSACTIVE
				   | ATMEL_LCDC_MEMOR_BIG),
	.default_monspecs	= &atevklcd10x_default_monspecs,
	.guard_time		= 2,
};
#endif

static int __init atevklcd10x_init(void)
{
	at32_add_device_ac97c(0, &ac97c0_data);

	at32_add_device_lcdc(0, &atevklcd10x_lcdc_data,
			fbmem_start, fbmem_size, 1);
	return 0;
}
postcore_initcall(atevklcd10x_init);
