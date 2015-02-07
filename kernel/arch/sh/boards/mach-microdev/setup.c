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
 * arch/sh/boards/superh/microdev/setup.c
 *
 * Copyright (C) 2003 Sean McGoogan (Sean.McGoogan@superh.com)
 * Copyright (C) 2003, 2004 SuperH, Inc.
 * Copyright (C) 2004, 2005 Paul Mundt
 *
 * SuperH SH4-202 MicroDev board support.
 *
 * May be copied or modified under the terms of the GNU General Public
 * License.  See linux/COPYING for more information.
 */
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/ioport.h>
#include <video/s1d13xxxfb.h>
#include <mach/microdev.h>
#include <asm/io.h>
#include <asm/machvec.h>
#include <asm/sizes.h>

static struct resource smc91x_resources[] = {
	[0] = {
		.start		= 0x300,
		.end		= 0x300 + SZ_4K - 1,
		.flags		= IORESOURCE_MEM,
	},
	[1] = {
		.start		= MICRODEV_LINUX_IRQ_ETHERNET,
		.end		= MICRODEV_LINUX_IRQ_ETHERNET,
		.flags		= IORESOURCE_IRQ,
	},
};

static struct platform_device smc91x_device = {
	.name		= "smc91x",
	.id		= -1,
	.num_resources	= ARRAY_SIZE(smc91x_resources),
	.resource	= smc91x_resources,
};

static struct s1d13xxxfb_regval s1d13806_initregs[] = {
	{ S1DREG_MISC,			0x00 },
	{ S1DREG_COM_DISP_MODE,		0x00 },
	{ S1DREG_GPIO_CNF0,		0x00 },
	{ S1DREG_GPIO_CNF1,		0x00 },
	{ S1DREG_GPIO_CTL0,		0x00 },
	{ S1DREG_GPIO_CTL1,		0x00 },
	{ S1DREG_CLK_CNF,		0x02 },
	{ S1DREG_LCD_CLK_CNF,		0x01 },
	{ S1DREG_CRT_CLK_CNF,		0x03 },
	{ S1DREG_MPLUG_CLK_CNF,		0x03 },
	{ S1DREG_CPU2MEM_WST_SEL,	0x02 },
	{ S1DREG_SDRAM_REF_RATE,	0x03 },
	{ S1DREG_SDRAM_TC0,		0x00 },
	{ S1DREG_SDRAM_TC1,		0x01 },
	{ S1DREG_MEM_CNF,		0x80 },
	{ S1DREG_PANEL_TYPE,		0x25 },
	{ S1DREG_MOD_RATE,		0x00 },
	{ S1DREG_LCD_DISP_HWIDTH,	0x63 },
	{ S1DREG_LCD_NDISP_HPER,	0x1e },
	{ S1DREG_TFT_FPLINE_START,	0x06 },
	{ S1DREG_TFT_FPLINE_PWIDTH,	0x03 },
	{ S1DREG_LCD_DISP_VHEIGHT0,	0x57 },
	{ S1DREG_LCD_DISP_VHEIGHT1,	0x02 },
	{ S1DREG_LCD_NDISP_VPER,	0x00 },
	{ S1DREG_TFT_FPFRAME_START,	0x0a },
	{ S1DREG_TFT_FPFRAME_PWIDTH,	0x81 },
	{ S1DREG_LCD_DISP_MODE,		0x03 },
	{ S1DREG_LCD_MISC,		0x00 },
	{ S1DREG_LCD_DISP_START0,	0x00 },
	{ S1DREG_LCD_DISP_START1,	0x00 },
	{ S1DREG_LCD_DISP_START2,	0x00 },
	{ S1DREG_LCD_MEM_OFF0,		0x90 },
	{ S1DREG_LCD_MEM_OFF1,		0x01 },
	{ S1DREG_LCD_PIX_PAN,		0x00 },
	{ S1DREG_LCD_DISP_FIFO_HTC,	0x00 },
	{ S1DREG_LCD_DISP_FIFO_LTC,	0x00 },
	{ S1DREG_CRT_DISP_HWIDTH,	0x63 },
	{ S1DREG_CRT_NDISP_HPER,	0x1f },
	{ S1DREG_CRT_HRTC_START,	0x04 },
	{ S1DREG_CRT_HRTC_PWIDTH,	0x8f },
	{ S1DREG_CRT_DISP_VHEIGHT0,	0x57 },
	{ S1DREG_CRT_DISP_VHEIGHT1,	0x02 },
	{ S1DREG_CRT_NDISP_VPER,	0x1b },
	{ S1DREG_CRT_VRTC_START,	0x00 },
	{ S1DREG_CRT_VRTC_PWIDTH,	0x83 },
	{ S1DREG_TV_OUT_CTL,		0x10 },
	{ S1DREG_CRT_DISP_MODE,		0x05 },
	{ S1DREG_CRT_DISP_START0,	0x00 },
	{ S1DREG_CRT_DISP_START1,	0x00 },
	{ S1DREG_CRT_DISP_START2,	0x00 },
	{ S1DREG_CRT_MEM_OFF0,		0x20 },
	{ S1DREG_CRT_MEM_OFF1,		0x03 },
	{ S1DREG_CRT_PIX_PAN,		0x00 },
	{ S1DREG_CRT_DISP_FIFO_HTC,	0x00 },
	{ S1DREG_CRT_DISP_FIFO_LTC,	0x00 },
	{ S1DREG_LCD_CUR_CTL,		0x00 },
	{ S1DREG_LCD_CUR_START,		0x01 },
	{ S1DREG_LCD_CUR_XPOS0,		0x00 },
	{ S1DREG_LCD_CUR_XPOS1,		0x00 },
	{ S1DREG_LCD_CUR_YPOS0,		0x00 },
	{ S1DREG_LCD_CUR_YPOS1,		0x00 },
	{ S1DREG_LCD_CUR_BCTL0,		0x00 },
	{ S1DREG_LCD_CUR_GCTL0,		0x00 },
	{ S1DREG_LCD_CUR_RCTL0,		0x00 },
	{ S1DREG_LCD_CUR_BCTL1,		0x1f },
	{ S1DREG_LCD_CUR_GCTL1,		0x3f },
	{ S1DREG_LCD_CUR_RCTL1,		0x1f },
	{ S1DREG_LCD_CUR_FIFO_HTC,	0x00 },
	{ S1DREG_CRT_CUR_CTL,		0x00 },
	{ S1DREG_CRT_CUR_START,		0x01 },
	{ S1DREG_CRT_CUR_XPOS0,		0x00 },
	{ S1DREG_CRT_CUR_XPOS1,		0x00 },
	{ S1DREG_CRT_CUR_YPOS0,		0x00 },
	{ S1DREG_CRT_CUR_YPOS1,		0x00 },
	{ S1DREG_CRT_CUR_BCTL0,		0x00 },
	{ S1DREG_CRT_CUR_GCTL0,		0x00 },
	{ S1DREG_CRT_CUR_RCTL0,		0x00 },
	{ S1DREG_CRT_CUR_BCTL1,		0x1f },
	{ S1DREG_CRT_CUR_GCTL1,		0x3f },
	{ S1DREG_CRT_CUR_RCTL1,		0x1f },
	{ S1DREG_CRT_CUR_FIFO_HTC,	0x00 },
	{ S1DREG_BBLT_CTL0,		0x00 },
	{ S1DREG_BBLT_CTL1,		0x00 },
	{ S1DREG_BBLT_CC_EXP,		0x00 },
	{ S1DREG_BBLT_OP,		0x00 },
	{ S1DREG_BBLT_SRC_START0,	0x00 },
	{ S1DREG_BBLT_SRC_START1,	0x00 },
	{ S1DREG_BBLT_SRC_START2,	0x00 },
	{ S1DREG_BBLT_DST_START0,	0x00 },
	{ S1DREG_BBLT_DST_START1,	0x00 },
	{ S1DREG_BBLT_DST_START2,	0x00 },
	{ S1DREG_BBLT_MEM_OFF0,		0x00 },
	{ S1DREG_BBLT_MEM_OFF1,		0x00 },
	{ S1DREG_BBLT_WIDTH0,		0x00 },
	{ S1DREG_BBLT_WIDTH1,		0x00 },
	{ S1DREG_BBLT_HEIGHT0,		0x00 },
	{ S1DREG_BBLT_HEIGHT1,		0x00 },
	{ S1DREG_BBLT_BGC0,		0x00 },
	{ S1DREG_BBLT_BGC1,		0x00 },
	{ S1DREG_BBLT_FGC0,		0x00 },
	{ S1DREG_BBLT_FGC1,		0x00 },
	{ S1DREG_LKUP_MODE,		0x00 },
	{ S1DREG_LKUP_ADDR,		0x00 },
	{ S1DREG_PS_CNF,		0x10 },
	{ S1DREG_PS_STATUS,		0x00 },
	{ S1DREG_CPU2MEM_WDOGT,		0x00 },
	{ S1DREG_COM_DISP_MODE,		0x02 },
};

static struct s1d13xxxfb_pdata s1d13806_platform_data = {
	.initregs	= s1d13806_initregs,
	.initregssize	= ARRAY_SIZE(s1d13806_initregs),
};

static struct resource s1d13806_resources[] = {
	[0] = {
		.start		= 0x07200000,
		.end		= 0x07200000 + SZ_2M - 1,
		.flags		= IORESOURCE_MEM,
	},
	[1] = {
		.start		= 0x07000000,
		.end		= 0x07000000 + SZ_2M - 1,
		.flags		= IORESOURCE_MEM,
	},
};

static struct platform_device s1d13806_device = {
	.name		= "s1d13806fb",
	.id		= -1,
	.num_resources	= ARRAY_SIZE(s1d13806_resources),
	.resource	= s1d13806_resources,

	.dev = {
		.platform_data	= &s1d13806_platform_data,
	},
};

static struct platform_device *microdev_devices[] __initdata = {
	&smc91x_device,
	&s1d13806_device,
};

static int __init microdev_devices_setup(void)
{
	return platform_add_devices(microdev_devices, ARRAY_SIZE(microdev_devices));
}
device_initcall(microdev_devices_setup);

/*
 * The Machine Vector
 */
static struct sh_machine_vector mv_sh4202_microdev __initmv = {
	.mv_name		= "SH4-202 MicroDev",
	.mv_nr_irqs		= 72,

	.mv_inb			= microdev_inb,
	.mv_inw			= microdev_inw,
	.mv_inl			= microdev_inl,
	.mv_outb		= microdev_outb,
	.mv_outw		= microdev_outw,
	.mv_outl		= microdev_outl,

	.mv_inb_p		= microdev_inb_p,
	.mv_inw_p		= microdev_inw_p,
	.mv_inl_p		= microdev_inl_p,
	.mv_outb_p		= microdev_outb_p,
	.mv_outw_p		= microdev_outw_p,
	.mv_outl_p		= microdev_outl_p,

	.mv_insb		= microdev_insb,
	.mv_insw		= microdev_insw,
	.mv_insl		= microdev_insl,
	.mv_outsb		= microdev_outsb,
	.mv_outsw		= microdev_outsw,
	.mv_outsl		= microdev_outsl,

	.mv_init_irq		= init_microdev_irq,
};
