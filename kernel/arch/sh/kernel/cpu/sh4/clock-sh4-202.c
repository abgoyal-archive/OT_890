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
 * arch/sh/kernel/cpu/sh4/clock-sh4-202.c
 *
 * Additional SH4-202 support for the clock framework
 *
 *  Copyright (C) 2005  Paul Mundt
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/err.h>
#include <asm/clock.h>
#include <asm/freq.h>
#include <asm/io.h>

#define CPG2_FRQCR3	0xfe0a0018

static int frqcr3_divisors[] = { 1, 2, 3, 4, 6, 8, 16 };
static int frqcr3_values[]   = { 0, 1, 2, 3, 4, 5, 6  };

static void emi_clk_recalc(struct clk *clk)
{
	int idx = ctrl_inl(CPG2_FRQCR3) & 0x0007;
	clk->rate = clk->parent->rate / frqcr3_divisors[idx];
}

static inline int frqcr3_lookup(struct clk *clk, unsigned long rate)
{
	int divisor = clk->parent->rate / rate;
	int i;

	for (i = 0; i < ARRAY_SIZE(frqcr3_divisors); i++)
		if (frqcr3_divisors[i] == divisor)
			return frqcr3_values[i];

	/* Safe fallback */
	return 5;
}

static struct clk_ops sh4202_emi_clk_ops = {
	.recalc		= emi_clk_recalc,
};

static struct clk sh4202_emi_clk = {
	.name		= "emi_clk",
	.flags		= CLK_ALWAYS_ENABLED,
	.ops		= &sh4202_emi_clk_ops,
};

static void femi_clk_recalc(struct clk *clk)
{
	int idx = (ctrl_inl(CPG2_FRQCR3) >> 3) & 0x0007;
	clk->rate = clk->parent->rate / frqcr3_divisors[idx];
}

static struct clk_ops sh4202_femi_clk_ops = {
	.recalc		= femi_clk_recalc,
};

static struct clk sh4202_femi_clk = {
	.name		= "femi_clk",
	.flags		= CLK_ALWAYS_ENABLED,
	.ops		= &sh4202_femi_clk_ops,
};

static void shoc_clk_init(struct clk *clk)
{
	int i;

	/*
	 * For some reason, the shoc_clk seems to be set to some really
	 * insane value at boot (values outside of the allowable frequency
	 * range for instance). We deal with this by scaling it back down
	 * to something sensible just in case.
	 *
	 * Start scaling from the high end down until we find something
	 * that passes rate verification..
	 */
	for (i = 0; i < ARRAY_SIZE(frqcr3_divisors); i++) {
		int divisor = frqcr3_divisors[i];

		if (clk->ops->set_rate(clk, clk->parent->rate /
						divisor, 0) == 0)
			break;
	}

	WARN_ON(i == ARRAY_SIZE(frqcr3_divisors));	/* Undefined clock */
}

static void shoc_clk_recalc(struct clk *clk)
{
	int idx = (ctrl_inl(CPG2_FRQCR3) >> 6) & 0x0007;
	clk->rate = clk->parent->rate / frqcr3_divisors[idx];
}

static int shoc_clk_verify_rate(struct clk *clk, unsigned long rate)
{
	struct clk *bclk = clk_get(NULL, "bus_clk");
	unsigned long bclk_rate = clk_get_rate(bclk);

	clk_put(bclk);

	if (rate > bclk_rate)
		return 1;
	if (rate > 66000000)
		return 1;

	return 0;
}

static int shoc_clk_set_rate(struct clk *clk, unsigned long rate, int algo_id)
{
	unsigned long frqcr3;
	unsigned int tmp;

	/* Make sure we have something sensible to switch to */
	if (shoc_clk_verify_rate(clk, rate) != 0)
		return -EINVAL;

	tmp = frqcr3_lookup(clk, rate);

	frqcr3 = ctrl_inl(CPG2_FRQCR3);
	frqcr3 &= ~(0x0007 << 6);
	frqcr3 |= tmp << 6;
	ctrl_outl(frqcr3, CPG2_FRQCR3);

	clk->rate = clk->parent->rate / frqcr3_divisors[tmp];

	return 0;
}

static struct clk_ops sh4202_shoc_clk_ops = {
	.init		= shoc_clk_init,
	.recalc		= shoc_clk_recalc,
	.set_rate	= shoc_clk_set_rate,
};

static struct clk sh4202_shoc_clk = {
	.name		= "shoc_clk",
	.flags		= CLK_ALWAYS_ENABLED,
	.ops		= &sh4202_shoc_clk_ops,
};

static struct clk *sh4202_onchip_clocks[] = {
	&sh4202_emi_clk,
	&sh4202_femi_clk,
	&sh4202_shoc_clk,
};

static int __init sh4202_clk_init(void)
{
	struct clk *clk = clk_get(NULL, "master_clk");
	int i;

	for (i = 0; i < ARRAY_SIZE(sh4202_onchip_clocks); i++) {
		struct clk *clkp = sh4202_onchip_clocks[i];

		clkp->parent = clk;
		clk_register(clkp);
		clk_enable(clkp);
	}

	/*
	 * Now that we have the rest of the clocks registered, we need to
	 * force the parent clock to propagate so that these clocks will
	 * automatically figure out their rate. We cheat by handing the
	 * parent clock its current rate and forcing child propagation.
	 */
	clk_set_rate(clk, clk_get_rate(clk));

	clk_put(clk);

	return 0;
}

arch_initcall(sh4202_clk_init);

