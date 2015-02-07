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
 *  linux/arch/arm/mach-omap2/clock.h
 *
 *  Copyright (C) 2005-2008 Texas Instruments, Inc.
 *  Copyright (C) 2004-2008 Nokia Corporation
 *
 *  Contacts:
 *  Richard Woodruff <r-woodruff2@ti.com>
 *  Paul Walmsley
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __ARCH_ARM_MACH_OMAP2_CLOCK_H
#define __ARCH_ARM_MACH_OMAP2_CLOCK_H

#include <mach/clock.h>

/* The maximum error between a target DPLL rate and the rounded rate in Hz */
#define DEFAULT_DPLL_RATE_TOLERANCE	50000

int omap2_clk_init(void);
int omap2_clk_enable(struct clk *clk);
void omap2_clk_disable(struct clk *clk);
long omap2_clk_round_rate(struct clk *clk, unsigned long rate);
int omap2_clk_set_rate(struct clk *clk, unsigned long rate);
int omap2_clk_set_parent(struct clk *clk, struct clk *new_parent);
int omap2_dpll_rate_tolerance_set(struct clk *clk, unsigned int tolerance);
long omap2_dpll_round_rate(struct clk *clk, unsigned long target_rate);

#ifdef CONFIG_OMAP_RESET_CLOCKS
void omap2_clk_disable_unused(struct clk *clk);
#else
#define omap2_clk_disable_unused	NULL
#endif

void omap2_clksel_recalc(struct clk *clk);
void omap2_init_clk_clkdm(struct clk *clk);
void omap2_init_clksel_parent(struct clk *clk);
u32 omap2_clksel_get_divisor(struct clk *clk);
u32 omap2_clksel_round_rate_div(struct clk *clk, unsigned long target_rate,
				u32 *new_div);
u32 omap2_clksel_to_divisor(struct clk *clk, u32 field_val);
u32 omap2_divisor_to_clksel(struct clk *clk, u32 div);
void omap2_fixed_divisor_recalc(struct clk *clk);
long omap2_clksel_round_rate(struct clk *clk, unsigned long target_rate);
int omap2_clksel_set_rate(struct clk *clk, unsigned long rate);
u32 omap2_get_dpll_rate(struct clk *clk);
int omap2_wait_clock_ready(void __iomem *reg, u32 cval, const char *name);
void omap2_clk_prepare_for_reboot(void);

extern u8 cpu_mask;

/* clksel_rate data common to 24xx/343x */
static const struct clksel_rate gpt_32k_rates[] = {
	 { .div = 1, .val = 0, .flags = RATE_IN_24XX | RATE_IN_343X | DEFAULT_RATE },
	 { .div = 0 }
};

static const struct clksel_rate gpt_sys_rates[] = {
	 { .div = 1, .val = 1, .flags = RATE_IN_24XX | RATE_IN_343X | DEFAULT_RATE },
	 { .div = 0 }
};

static const struct clksel_rate gfx_l3_rates[] = {
	{ .div = 1, .val = 1, .flags = RATE_IN_24XX | RATE_IN_343X },
	{ .div = 2, .val = 2, .flags = RATE_IN_24XX | RATE_IN_343X | DEFAULT_RATE },
	{ .div = 3, .val = 3, .flags = RATE_IN_243X | RATE_IN_343X },
	{ .div = 4, .val = 4, .flags = RATE_IN_243X | RATE_IN_343X },
	{ .div = 0 }
};


#endif
