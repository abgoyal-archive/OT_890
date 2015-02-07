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
 * A gpio chip driver for TXx9 SoCs
 *
 * Copyright (C) 2008 Atsushi Nemoto <anemo@mba.ocn.ne.jp>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/init.h>
#include <linux/spinlock.h>
#include <linux/gpio.h>
#include <linux/errno.h>
#include <linux/io.h>
#include <asm/txx9pio.h>

static DEFINE_SPINLOCK(txx9_gpio_lock);

static struct txx9_pio_reg __iomem *txx9_pioptr;

static int txx9_gpio_get(struct gpio_chip *chip, unsigned int offset)
{
	return __raw_readl(&txx9_pioptr->din) & (1 << offset);
}

static void txx9_gpio_set_raw(unsigned int offset, int value)
{
	u32 val;
	val = __raw_readl(&txx9_pioptr->dout);
	if (value)
		val |= 1 << offset;
	else
		val &= ~(1 << offset);
	__raw_writel(val, &txx9_pioptr->dout);
}

static void txx9_gpio_set(struct gpio_chip *chip, unsigned int offset,
			  int value)
{
	unsigned long flags;
	spin_lock_irqsave(&txx9_gpio_lock, flags);
	txx9_gpio_set_raw(offset, value);
	mmiowb();
	spin_unlock_irqrestore(&txx9_gpio_lock, flags);
}

static int txx9_gpio_dir_in(struct gpio_chip *chip, unsigned int offset)
{
	unsigned long flags;
	spin_lock_irqsave(&txx9_gpio_lock, flags);
	__raw_writel(__raw_readl(&txx9_pioptr->dir) & ~(1 << offset),
		     &txx9_pioptr->dir);
	mmiowb();
	spin_unlock_irqrestore(&txx9_gpio_lock, flags);
	return 0;
}

static int txx9_gpio_dir_out(struct gpio_chip *chip, unsigned int offset,
			     int value)
{
	unsigned long flags;
	spin_lock_irqsave(&txx9_gpio_lock, flags);
	txx9_gpio_set_raw(offset, value);
	__raw_writel(__raw_readl(&txx9_pioptr->dir) | (1 << offset),
		     &txx9_pioptr->dir);
	mmiowb();
	spin_unlock_irqrestore(&txx9_gpio_lock, flags);
	return 0;
}

static struct gpio_chip txx9_gpio_chip = {
	.get = txx9_gpio_get,
	.set = txx9_gpio_set,
	.direction_input = txx9_gpio_dir_in,
	.direction_output = txx9_gpio_dir_out,
	.label = "TXx9",
};

int __init txx9_gpio_init(unsigned long baseaddr,
			  unsigned int base, unsigned int num)
{
	txx9_pioptr = ioremap(baseaddr, sizeof(struct txx9_pio_reg));
	if (!txx9_pioptr)
		return -ENODEV;
	txx9_gpio_chip.base = base;
	txx9_gpio_chip.ngpio = num;
	return gpiochip_add(&txx9_gpio_chip);
}
