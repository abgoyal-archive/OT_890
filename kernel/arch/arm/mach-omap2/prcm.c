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
 * linux/arch/arm/mach-omap2/prcm.c
 *
 * OMAP 24xx Power Reset and Clock Management (PRCM) functions
 *
 * Copyright (C) 2005 Nokia Corporation
 *
 * Written by Tony Lindgren <tony.lindgren@nokia.com>
 *
 * Some pieces of code Copyright (C) 2005 Texas Instruments, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/module.h>
#include <linux/init.h>
#include <linux/clk.h>
#include <linux/io.h>

#include <mach/common.h>
#include <mach/prcm.h>

#include "clock.h"
#include "prm.h"
#include "prm-regbits-24xx.h"

static void __iomem *prm_base;
static void __iomem *cm_base;

u32 omap_prcm_get_reset_sources(void)
{
	/* XXX This presumably needs modification for 34XX */
	return prm_read_mod_reg(WKUP_MOD, RM_RSTST) & 0x7f;
}
EXPORT_SYMBOL(omap_prcm_get_reset_sources);

/* Resets clock rates and reboots the system. Only called from system.h */
void omap_prcm_arch_reset(char mode)
{
	s16 prcm_offs;
	omap2_clk_prepare_for_reboot();

	if (cpu_is_omap24xx())
		prcm_offs = WKUP_MOD;
	else if (cpu_is_omap34xx())
		prcm_offs = OMAP3430_GR_MOD;
	else
		WARN_ON(1);

	prm_set_mod_reg_bits(OMAP_RST_DPLL3, prcm_offs, RM_RSTCTRL);
}

static inline u32 __omap_prcm_read(void __iomem *base, s16 module, u16 reg)
{
	BUG_ON(!base);
	return __raw_readl(base + module + reg);
}

static inline void __omap_prcm_write(u32 value, void __iomem *base,
						s16 module, u16 reg)
{
	BUG_ON(!base);
	__raw_writel(value, base + module + reg);
}

/* Read a register in a PRM module */
u32 prm_read_mod_reg(s16 module, u16 idx)
{
	return __omap_prcm_read(prm_base, module, idx);
}
EXPORT_SYMBOL(prm_read_mod_reg);

/* Write into a register in a PRM module */
void prm_write_mod_reg(u32 val, s16 module, u16 idx)
{
	__omap_prcm_write(val, prm_base, module, idx);
}
EXPORT_SYMBOL(prm_write_mod_reg);

/* Read-modify-write a register in a PRM module. Caller must lock */
u32 prm_rmw_mod_reg_bits(u32 mask, u32 bits, s16 module, s16 idx)
{
	u32 v;

	v = prm_read_mod_reg(module, idx);
	v &= ~mask;
	v |= bits;
	prm_write_mod_reg(v, module, idx);

	return v;
}
EXPORT_SYMBOL(prm_rmw_mod_reg_bits);

/* Read a register in a CM module */
u32 cm_read_mod_reg(s16 module, u16 idx)
{
	return __omap_prcm_read(cm_base, module, idx);
}
EXPORT_SYMBOL(cm_read_mod_reg);

/* Write into a register in a CM module */
void cm_write_mod_reg(u32 val, s16 module, u16 idx)
{
	__omap_prcm_write(val, cm_base, module, idx);
}
EXPORT_SYMBOL(cm_write_mod_reg);

/* Read-modify-write a register in a CM module. Caller must lock */
u32 cm_rmw_mod_reg_bits(u32 mask, u32 bits, s16 module, s16 idx)
{
	u32 v;

	v = cm_read_mod_reg(module, idx);
	v &= ~mask;
	v |= bits;
	cm_write_mod_reg(v, module, idx);

	return v;
}
EXPORT_SYMBOL(cm_rmw_mod_reg_bits);

void __init omap2_set_globals_prcm(struct omap_globals *omap2_globals)
{
	prm_base = omap2_globals->prm;
	cm_base = omap2_globals->cm;
}
