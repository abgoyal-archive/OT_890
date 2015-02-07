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

/* arch/arm/mach-lh7a40x/clocks.c
 *
 *  Copyright (C) 2004 Marc Singer
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.
 *
 */

#include <linux/cpufreq.h>
#include <mach/hardware.h>
#include <mach/clocks.h>
#include <linux/err.h>

struct module;

struct clk {
	struct list_head node;
	unsigned long rate;
	struct module *owner;
	const char *name;
};

/* ----- */

#define MAINDIV1(c)	(((c) >>  7) & 0x0f)
#define MAINDIV2(c)	(((c) >> 11) & 0x1f)
#define PS(c)		(((c) >> 18) & 0x03)
#define PREDIV(c)	(((c) >>  2) & 0x1f)
#define HCLKDIV(c)	(((c) >>  0) & 0x02)
#define PCLKDIV(c)	(((c) >> 16) & 0x03)

unsigned int cpufreq_get (unsigned int cpu) /* in kHz */
{
	return fclkfreq_get ()/1000;
}
EXPORT_SYMBOL(cpufreq_get);

unsigned int fclkfreq_get (void)
{
	unsigned int clkset = CSC_CLKSET;
	unsigned int gclk
		= XTAL_IN
		/ (1 << PS(clkset))
		* (MAINDIV1(clkset) + 2)
		/ (PREDIV(clkset)   + 2)
		* (MAINDIV2(clkset) + 2)
		;
	return gclk;
}

unsigned int hclkfreq_get (void)
{
	unsigned int clkset = CSC_CLKSET;
	unsigned int hclk = fclkfreq_get () / (HCLKDIV(clkset) + 1);

	return hclk;
}

unsigned int pclkfreq_get (void)
{
	unsigned int clkset = CSC_CLKSET;
	int pclkdiv = PCLKDIV(clkset);
	unsigned int pclk;
	if (pclkdiv == 0x3)
		pclkdiv = 0x2;
	pclk = hclkfreq_get () / (1 << pclkdiv);

	return pclk;
}

/* ----- */

struct clk *clk_get (struct device *dev, const char *id)
{
	return dev && strcmp(dev_name(dev), "cldc-lh7a40x") == 0
		 ? NULL : ERR_PTR(-ENOENT);
}
EXPORT_SYMBOL(clk_get);

void clk_put (struct clk *clk)
{
}
EXPORT_SYMBOL(clk_put);

int clk_enable (struct clk *clk)
{
	return 0;
}
EXPORT_SYMBOL(clk_enable);

void clk_disable (struct clk *clk)
{
}
EXPORT_SYMBOL(clk_disable);

unsigned long clk_get_rate (struct clk *clk)
{
	return 0;
}
EXPORT_SYMBOL(clk_get_rate);

long clk_round_rate (struct clk *clk, unsigned long rate)
{
	return rate;
}
EXPORT_SYMBOL(clk_round_rate);

int clk_set_rate (struct clk *clk, unsigned long rate)
{
	return -EIO;
}
EXPORT_SYMBOL(clk_set_rate);
