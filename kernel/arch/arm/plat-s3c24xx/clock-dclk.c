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

/* linux/arch/arm/plat-s3c24xx/clock-dclk.c
 *
 * Copyright (c) 2004,2008 Simtec Electronics
 *	Ben Dooks <ben@simtec.co.uk>
 *	http://armlinux.simtec.co.uk/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * S3C24XX - definitions for DCLK and CLKOUT registers
 */

#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/clk.h>
#include <linux/io.h>

#include <mach/regs-clock.h>
#include <mach/regs-gpio.h>

#include <plat/clock.h>
#include <plat/cpu.h>

/* clocks that could be registered by external code */

static int s3c24xx_dclk_enable(struct clk *clk, int enable)
{
	unsigned long dclkcon = __raw_readl(S3C24XX_DCLKCON);

	if (enable)
		dclkcon |= clk->ctrlbit;
	else
		dclkcon &= ~clk->ctrlbit;

	__raw_writel(dclkcon, S3C24XX_DCLKCON);

	return 0;
}

static int s3c24xx_dclk_setparent(struct clk *clk, struct clk *parent)
{
	unsigned long dclkcon;
	unsigned int uclk;

	if (parent == &clk_upll)
		uclk = 1;
	else if (parent == &clk_p)
		uclk = 0;
	else
		return -EINVAL;

	clk->parent = parent;

	dclkcon = __raw_readl(S3C24XX_DCLKCON);

	if (clk->ctrlbit == S3C2410_DCLKCON_DCLK0EN) {
		if (uclk)
			dclkcon |= S3C2410_DCLKCON_DCLK0_UCLK;
		else
			dclkcon &= ~S3C2410_DCLKCON_DCLK0_UCLK;
	} else {
		if (uclk)
			dclkcon |= S3C2410_DCLKCON_DCLK1_UCLK;
		else
			dclkcon &= ~S3C2410_DCLKCON_DCLK1_UCLK;
	}

	__raw_writel(dclkcon, S3C24XX_DCLKCON);

	return 0;
}
static unsigned long s3c24xx_calc_div(struct clk *clk, unsigned long rate)
{
	unsigned long div;

	if ((rate == 0) || !clk->parent)
		return 0;

	div = clk_get_rate(clk->parent) / rate;
	if (div < 2)
		div = 2;
	else if (div > 16)
		div = 16;

	return div;
}

static unsigned long s3c24xx_round_dclk_rate(struct clk *clk,
	unsigned long rate)
{
	unsigned long div = s3c24xx_calc_div(clk, rate);

	if (div == 0)
		return 0;

	return clk_get_rate(clk->parent) / div;
}

static int s3c24xx_set_dclk_rate(struct clk *clk, unsigned long rate)
{
	unsigned long mask, data, div = s3c24xx_calc_div(clk, rate);

	if (div == 0)
		return -EINVAL;

	if (clk == &s3c24xx_dclk0) {
		mask = S3C2410_DCLKCON_DCLK0_DIV_MASK |
			S3C2410_DCLKCON_DCLK0_CMP_MASK;
		data = S3C2410_DCLKCON_DCLK0_DIV(div) |
			S3C2410_DCLKCON_DCLK0_CMP((div + 1) / 2);
	} else if (clk == &s3c24xx_dclk1) {
		mask = S3C2410_DCLKCON_DCLK1_DIV_MASK |
			S3C2410_DCLKCON_DCLK1_CMP_MASK;
		data = S3C2410_DCLKCON_DCLK1_DIV(div) |
			S3C2410_DCLKCON_DCLK1_CMP((div + 1) / 2);
	} else
		return -EINVAL;

	clk->rate = clk_get_rate(clk->parent) / div;
	__raw_writel(((__raw_readl(S3C24XX_DCLKCON) & ~mask) | data),
		S3C24XX_DCLKCON);
	return clk->rate;
}
static int s3c24xx_clkout_setparent(struct clk *clk, struct clk *parent)
{
	unsigned long mask;
	unsigned long source;

	/* calculate the MISCCR setting for the clock */

	if (parent == &clk_xtal)
		source = S3C2410_MISCCR_CLK0_MPLL;
	else if (parent == &clk_upll)
		source = S3C2410_MISCCR_CLK0_UPLL;
	else if (parent == &clk_f)
		source = S3C2410_MISCCR_CLK0_FCLK;
	else if (parent == &clk_h)
		source = S3C2410_MISCCR_CLK0_HCLK;
	else if (parent == &clk_p)
		source = S3C2410_MISCCR_CLK0_PCLK;
	else if (clk == &s3c24xx_clkout0 && parent == &s3c24xx_dclk0)
		source = S3C2410_MISCCR_CLK0_DCLK0;
	else if (clk == &s3c24xx_clkout1 && parent == &s3c24xx_dclk1)
		source = S3C2410_MISCCR_CLK0_DCLK0;
	else
		return -EINVAL;

	clk->parent = parent;

	if (clk == &s3c24xx_clkout0)
		mask = S3C2410_MISCCR_CLK0_MASK;
	else {
		source <<= 4;
		mask = S3C2410_MISCCR_CLK1_MASK;
	}

	s3c2410_modify_misccr(mask, source);
	return 0;
}

/* external clock definitions */

struct clk s3c24xx_dclk0 = {
	.name		= "dclk0",
	.id		= -1,
	.ctrlbit	= S3C2410_DCLKCON_DCLK0EN,
	.enable	        = s3c24xx_dclk_enable,
	.set_parent	= s3c24xx_dclk_setparent,
	.set_rate	= s3c24xx_set_dclk_rate,
	.round_rate	= s3c24xx_round_dclk_rate,
};

struct clk s3c24xx_dclk1 = {
	.name		= "dclk1",
	.id		= -1,
	.ctrlbit	= S3C2410_DCLKCON_DCLK1EN,
	.enable		= s3c24xx_dclk_enable,
	.set_parent	= s3c24xx_dclk_setparent,
	.set_rate	= s3c24xx_set_dclk_rate,
	.round_rate	= s3c24xx_round_dclk_rate,
};

struct clk s3c24xx_clkout0 = {
	.name		= "clkout0",
	.id		= -1,
	.set_parent	= s3c24xx_clkout_setparent,
};

struct clk s3c24xx_clkout1 = {
	.name		= "clkout1",
	.id		= -1,
	.set_parent	= s3c24xx_clkout_setparent,
};
