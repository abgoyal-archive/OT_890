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
 * arch/sh/kernel/cpu/sh3/clock-sh7712.c
 *
 * SH7712 support for the clock framework
 *
 *  Copyright (C) 2007  Andrew Murray <amurray@mpc-data.co.uk>
 *
 * Based on arch/sh/kernel/cpu/sh3/clock-sh3.c
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

static int multipliers[] = { 1, 2, 3 };
static int divisors[]    = { 1, 2, 3, 4, 6 };

static void master_clk_init(struct clk *clk)
{
	int frqcr = ctrl_inw(FRQCR);
	int idx = (frqcr & 0x0300) >> 8;

	clk->rate *= multipliers[idx];
}

static struct clk_ops sh7712_master_clk_ops = {
	.init		= master_clk_init,
};

static void module_clk_recalc(struct clk *clk)
{
	int frqcr = ctrl_inw(FRQCR);
	int idx = frqcr & 0x0007;

	clk->rate = clk->parent->rate / divisors[idx];
}

static struct clk_ops sh7712_module_clk_ops = {
	.recalc		= module_clk_recalc,
};

static void cpu_clk_recalc(struct clk *clk)
{
	int frqcr = ctrl_inw(FRQCR);
	int idx = (frqcr & 0x0030) >> 4;

	clk->rate = clk->parent->rate / divisors[idx];
}

static struct clk_ops sh7712_cpu_clk_ops = {
	.recalc		= cpu_clk_recalc,
};

static struct clk_ops *sh7712_clk_ops[] = {
	&sh7712_master_clk_ops,
	&sh7712_module_clk_ops,
	&sh7712_cpu_clk_ops,
};

void __init arch_init_clk_ops(struct clk_ops **ops, int idx)
{
	if (idx < ARRAY_SIZE(sh7712_clk_ops))
		*ops = sh7712_clk_ops[idx];
}

