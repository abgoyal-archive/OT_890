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
 * arch/sh/kernel/cpu/sh2/clock-sh7619.c
 *
 * SH7619 support for the clock framework
 *
 *  Copyright (C) 2006  Yoshinori Sato
 *
 * Based on clock-sh4.c
 *  Copyright (C) 2005  Paul Mundt
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */
#include <linux/init.h>
#include <linux/kernel.h>
#include <asm/clock.h>
#include <asm/freq.h>
#include <asm/io.h>

static const int pll1rate[] = {1,2};
static const int pfc_divisors[] = {1,2,0,4};

#if (CONFIG_SH_CLK_MD == 1) || (CONFIG_SH_CLK_MD == 2)
#define PLL2 (4)
#elif (CONFIG_SH_CLK_MD == 5) || (CONFIG_SH_CLK_MD == 6)
#define PLL2 (2)
#else
#error "Illigal Clock Mode!"
#endif

static void master_clk_init(struct clk *clk)
{
	clk->rate *= PLL2 * pll1rate[(ctrl_inw(FREQCR) >> 8) & 7];
}

static struct clk_ops sh7619_master_clk_ops = {
	.init		= master_clk_init,
};

static void module_clk_recalc(struct clk *clk)
{
	int idx = (ctrl_inw(FREQCR) & 0x0007);
	clk->rate = clk->parent->rate / pfc_divisors[idx];
}

static struct clk_ops sh7619_module_clk_ops = {
	.recalc		= module_clk_recalc,
};

static void bus_clk_recalc(struct clk *clk)
{
	clk->rate = clk->parent->rate / pll1rate[(ctrl_inw(FREQCR) >> 8) & 7];
}

static struct clk_ops sh7619_bus_clk_ops = {
	.recalc		= bus_clk_recalc,
};

static void cpu_clk_recalc(struct clk *clk)
{
	clk->rate = clk->parent->rate;
}

static struct clk_ops sh7619_cpu_clk_ops = {
	.recalc		= cpu_clk_recalc,
};

static struct clk_ops *sh7619_clk_ops[] = {
	&sh7619_master_clk_ops,
	&sh7619_module_clk_ops,
	&sh7619_bus_clk_ops,
	&sh7619_cpu_clk_ops,
};

void __init arch_init_clk_ops(struct clk_ops **ops, int idx)
{
	if (idx < ARRAY_SIZE(sh7619_clk_ops))
		*ops = sh7619_clk_ops[idx];
}

