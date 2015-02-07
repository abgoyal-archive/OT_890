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
 * arch/arm/mach-ns9xxx/processor-ns9360.c
 *
 * Copyright (C) 2007 by Digi International Inc.
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 */
#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/slab.h>

#include <asm/page.h>
#include <asm/mach/map.h>
#include <mach/processor-ns9360.h>
#include <mach/regs-sys-ns9360.h>

void ns9360_reset(char mode)
{
	u32 reg;

	reg = __raw_readl(SYS_PLL) >> 16;
	REGSET(reg, SYS_PLL, SWC, YES);
	__raw_writel(reg, SYS_PLL);
}

#define CRYSTAL 29491200 /* Hz */
unsigned long ns9360_systemclock(void)
{
	u32 pll = __raw_readl(SYS_PLL);
	return CRYSTAL * (REGGETIM(pll, SYS_PLL, ND) + 1)
		>> REGGETIM(pll, SYS_PLL, FS);
}

static struct map_desc ns9360_io_desc[] __initdata = {
	{ /* BBus */
		.virtual = io_p2v(0x90000000),
		.pfn = __phys_to_pfn(0x90000000),
		.length = 0x00700000,
		.type = MT_DEVICE,
	}, { /* AHB */
		.virtual = io_p2v(0xa0100000),
		.pfn = __phys_to_pfn(0xa0100000),
		.length = 0x00900000,
		.type = MT_DEVICE,
	},
};

void __init ns9360_map_io(void)
{
	iotable_init(ns9360_io_desc, ARRAY_SIZE(ns9360_io_desc));
}
