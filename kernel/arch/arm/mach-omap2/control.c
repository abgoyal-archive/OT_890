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
 * OMAP2/3 System Control Module register access
 *
 * Copyright (C) 2007 Texas Instruments, Inc.
 * Copyright (C) 2007 Nokia Corporation
 *
 * Written by Paul Walmsley
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#undef DEBUG

#include <linux/kernel.h>
#include <linux/io.h>

#include <mach/common.h>
#include <mach/control.h>

static void __iomem *omap2_ctrl_base;

#define OMAP_CTRL_REGADDR(reg)		(omap2_ctrl_base + (reg))

void __init omap2_set_globals_control(struct omap_globals *omap2_globals)
{
	omap2_ctrl_base = omap2_globals->ctrl;
}

void __iomem *omap_ctrl_base_get(void)
{
	return omap2_ctrl_base;
}

u8 omap_ctrl_readb(u16 offset)
{
	return __raw_readb(OMAP_CTRL_REGADDR(offset));
}

u16 omap_ctrl_readw(u16 offset)
{
	return __raw_readw(OMAP_CTRL_REGADDR(offset));
}

u32 omap_ctrl_readl(u16 offset)
{
	return __raw_readl(OMAP_CTRL_REGADDR(offset));
}

void omap_ctrl_writeb(u8 val, u16 offset)
{
	__raw_writeb(val, OMAP_CTRL_REGADDR(offset));
}

void omap_ctrl_writew(u16 val, u16 offset)
{
	__raw_writew(val, OMAP_CTRL_REGADDR(offset));
}

void omap_ctrl_writel(u32 val, u16 offset)
{
	__raw_writel(val, OMAP_CTRL_REGADDR(offset));
}

