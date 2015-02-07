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
 * Simple Memory-Mapped GPIOs
 *
 * Copyright (c) MontaVista Software, Inc. 2008.
 *
 * Author: Anton Vorontsov <avorontsov@ru.mvista.com>
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/spinlock.h>
#include <linux/types.h>
#include <linux/ioport.h>
#include <linux/io.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/gpio.h>
#include <asm/prom.h>
#include "simple_gpio.h"

struct u8_gpio_chip {
	struct of_mm_gpio_chip mm_gc;
	spinlock_t lock;

	/* shadowed data register to clear/set bits safely */
	u8 data;
};

static struct u8_gpio_chip *to_u8_gpio_chip(struct of_mm_gpio_chip *mm_gc)
{
	return container_of(mm_gc, struct u8_gpio_chip, mm_gc);
}

static u8 u8_pin2mask(unsigned int pin)
{
	return 1 << (8 - 1 - pin);
}

static int u8_gpio_get(struct gpio_chip *gc, unsigned int gpio)
{
	struct of_mm_gpio_chip *mm_gc = to_of_mm_gpio_chip(gc);

	return in_8(mm_gc->regs) & u8_pin2mask(gpio);
}

static void u8_gpio_set(struct gpio_chip *gc, unsigned int gpio, int val)
{
	struct of_mm_gpio_chip *mm_gc = to_of_mm_gpio_chip(gc);
	struct u8_gpio_chip *u8_gc = to_u8_gpio_chip(mm_gc);
	unsigned long flags;

	spin_lock_irqsave(&u8_gc->lock, flags);

	if (val)
		u8_gc->data |= u8_pin2mask(gpio);
	else
		u8_gc->data &= ~u8_pin2mask(gpio);

	out_8(mm_gc->regs, u8_gc->data);

	spin_unlock_irqrestore(&u8_gc->lock, flags);
}

static int u8_gpio_dir_in(struct gpio_chip *gc, unsigned int gpio)
{
	return 0;
}

static int u8_gpio_dir_out(struct gpio_chip *gc, unsigned int gpio, int val)
{
	u8_gpio_set(gc, gpio, val);
	return 0;
}

static void u8_gpio_save_regs(struct of_mm_gpio_chip *mm_gc)
{
	struct u8_gpio_chip *u8_gc = to_u8_gpio_chip(mm_gc);

	u8_gc->data = in_8(mm_gc->regs);
}

static int __init u8_simple_gpiochip_add(struct device_node *np)
{
	int ret;
	struct u8_gpio_chip *u8_gc;
	struct of_mm_gpio_chip *mm_gc;
	struct of_gpio_chip *of_gc;
	struct gpio_chip *gc;

	u8_gc = kzalloc(sizeof(*u8_gc), GFP_KERNEL);
	if (!u8_gc)
		return -ENOMEM;

	spin_lock_init(&u8_gc->lock);

	mm_gc = &u8_gc->mm_gc;
	of_gc = &mm_gc->of_gc;
	gc = &of_gc->gc;

	mm_gc->save_regs = u8_gpio_save_regs;
	of_gc->gpio_cells = 2;
	gc->ngpio = 8;
	gc->direction_input = u8_gpio_dir_in;
	gc->direction_output = u8_gpio_dir_out;
	gc->get = u8_gpio_get;
	gc->set = u8_gpio_set;

	ret = of_mm_gpiochip_add(np, mm_gc);
	if (ret)
		goto err;
	return 0;
err:
	kfree(u8_gc);
	return ret;
}

void __init simple_gpiochip_init(const char *compatible)
{
	struct device_node *np;

	for_each_compatible_node(np, NULL, compatible) {
		int ret;
		struct resource r;

		ret = of_address_to_resource(np, 0, &r);
		if (ret)
			goto err;

		switch (resource_size(&r)) {
		case 1:
			ret = u8_simple_gpiochip_add(np);
			if (ret)
				goto err;
			break;
		default:
			/*
			 * Whenever you need support for GPIO bank width > 1,
			 * please just turn u8_ code into huge macros, and
			 * construct needed uX_ code with it.
			 */
			ret = -ENOSYS;
			goto err;
		}
		continue;
err:
		pr_err("%s: registration failed, status %d\n",
		       np->full_name, ret);
	}
}
