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
 *  linux/arch/arm/mach-sa1100/clock.c
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/errno.h>
#include <linux/err.h>
#include <linux/string.h>
#include <linux/clk.h>
#include <linux/spinlock.h>
#include <linux/platform_device.h>
#include <linux/delay.h>

#include <asm/clkdev.h>
#include <mach/pxa2xx-regs.h>
#include <mach/hardware.h>

#include "devices.h"
#include "generic.h"
#include "clock.h"

static DEFINE_SPINLOCK(clocks_lock);

int clk_enable(struct clk *clk)
{
	unsigned long flags;

	spin_lock_irqsave(&clocks_lock, flags);
	if (clk->enabled++ == 0)
		clk->ops->enable(clk);
	spin_unlock_irqrestore(&clocks_lock, flags);

	if (clk->delay)
		udelay(clk->delay);

	return 0;
}
EXPORT_SYMBOL(clk_enable);

void clk_disable(struct clk *clk)
{
	unsigned long flags;

	WARN_ON(clk->enabled == 0);

	spin_lock_irqsave(&clocks_lock, flags);
	if (--clk->enabled == 0)
		clk->ops->disable(clk);
	spin_unlock_irqrestore(&clocks_lock, flags);
}
EXPORT_SYMBOL(clk_disable);

unsigned long clk_get_rate(struct clk *clk)
{
	unsigned long rate;

	rate = clk->rate;
	if (clk->ops->getrate)
		rate = clk->ops->getrate(clk);

	return rate;
}
EXPORT_SYMBOL(clk_get_rate);


void clk_cken_enable(struct clk *clk)
{
	CKEN |= 1 << clk->cken;
}

void clk_cken_disable(struct clk *clk)
{
	CKEN &= ~(1 << clk->cken);
}

const struct clkops clk_cken_ops = {
	.enable		= clk_cken_enable,
	.disable	= clk_cken_disable,
};

void clks_register(struct clk_lookup *clks, size_t num)
{
	int i;

	for (i = 0; i < num; i++)
		clkdev_add(&clks[i]);
}

int clk_add_alias(char *alias, struct device *alias_dev, char *id,
	struct device *dev)
{
	struct clk *r = clk_get(dev, id);
	struct clk_lookup *l;

	if (!r)
		return -ENODEV;

	l = clkdev_alloc(r, alias, alias_dev ? dev_name(alias_dev) : NULL);
	clk_put(r);
	if (!l)
		return -ENODEV;
	clkdev_add(l);
	return 0;
}
