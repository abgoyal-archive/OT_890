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

/* linux/arch/arm/plat-s3c/dev-fb.c
 *
 * Copyright 2008 Simtec Electronics
 *	Ben Dooks <ben@simtec.co.uk>
 *	http://armlinux.simtec.co.uk/
 *
 * S3C series device definition for framebuffer device
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/platform_device.h>
#include <linux/fb.h>

#include <mach/irqs.h>
#include <mach/map.h>
#include <mach/regs-fb.h>

#include <plat/fb.h>
#include <plat/devs.h>
#include <plat/cpu.h>

static struct resource s3c_fb_resource[] = {
	[0] = {
		.start = S3C_PA_FB,
		.end   = S3C_PA_FB + SZ_16K - 1,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = IRQ_LCD_VSYNC,
		.end   = IRQ_LCD_VSYNC,
		.flags = IORESOURCE_IRQ,
	},
	[2] = {
		.start = IRQ_LCD_FIFO,
		.end   = IRQ_LCD_FIFO,
		.flags = IORESOURCE_IRQ,
	},
	[3] = {
		.start = IRQ_LCD_SYSTEM,
		.end   = IRQ_LCD_SYSTEM,
		.flags = IORESOURCE_IRQ,
	},
};

struct platform_device s3c_device_fb = {
	.name		  = "s3c-fb",
	.id		  = -1,
	.num_resources	  = ARRAY_SIZE(s3c_fb_resource),
	.resource	  = s3c_fb_resource,
	.dev.dma_mask	  = &s3c_device_fb.dev.coherent_dma_mask,
	.dev.coherent_dma_mask = 0xffffffffUL,
};

void __init s3c_fb_set_platdata(struct s3c_fb_platdata *pd)
{
	struct s3c_fb_platdata *npd;

	if (!pd) {
		printk(KERN_ERR "%s: no platform data\n", __func__);
		return;
	}

	npd = kmemdup(pd, sizeof(struct s3c_fb_platdata), GFP_KERNEL);
	if (!npd)
		printk(KERN_ERR "%s: no memory for platform data\n", __func__);

	s3c_device_fb.dev.platform_data = npd;
}
