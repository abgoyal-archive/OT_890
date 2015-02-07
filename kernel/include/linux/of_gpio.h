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
 * OF helpers for the GPIO API
 *
 * Copyright (c) 2007-2008  MontaVista Software, Inc.
 *
 * Author: Anton Vorontsov <avorontsov@ru.mvista.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef __LINUX_OF_GPIO_H
#define __LINUX_OF_GPIO_H

#include <linux/compiler.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/gpio.h>

struct device_node;

/*
 * This is Linux-specific flags. By default controllers' and Linux' mapping
 * match, but GPIO controllers are free to translate their own flags to
 * Linux-specific in their .xlate callback. Though, 1:1 mapping is recommended.
 */
enum of_gpio_flags {
	OF_GPIO_ACTIVE_LOW = 0x1,
};

#ifdef CONFIG_OF_GPIO

/*
 * Generic OF GPIO chip
 */
struct of_gpio_chip {
	struct gpio_chip gc;
	int gpio_cells;
	int (*xlate)(struct of_gpio_chip *of_gc, struct device_node *np,
		     const void *gpio_spec, enum of_gpio_flags *flags);
};

static inline struct of_gpio_chip *to_of_gpio_chip(struct gpio_chip *gc)
{
	return container_of(gc, struct of_gpio_chip, gc);
}

/*
 * OF GPIO chip for memory mapped banks
 */
struct of_mm_gpio_chip {
	struct of_gpio_chip of_gc;
	void (*save_regs)(struct of_mm_gpio_chip *mm_gc);
	void __iomem *regs;
};

static inline struct of_mm_gpio_chip *to_of_mm_gpio_chip(struct gpio_chip *gc)
{
	struct of_gpio_chip *of_gc = to_of_gpio_chip(gc);

	return container_of(of_gc, struct of_mm_gpio_chip, of_gc);
}

extern int of_get_gpio_flags(struct device_node *np, int index,
			     enum of_gpio_flags *flags);
extern unsigned int of_gpio_count(struct device_node *np);

extern int of_mm_gpiochip_add(struct device_node *np,
			      struct of_mm_gpio_chip *mm_gc);
extern int of_gpio_simple_xlate(struct of_gpio_chip *of_gc,
				struct device_node *np,
				const void *gpio_spec,
				enum of_gpio_flags *flags);
#else

/* Drivers may not strictly depend on the GPIO support, so let them link. */
static inline int of_get_gpio_flags(struct device_node *np, int index,
				    enum of_gpio_flags *flags)
{
	return -ENOSYS;
}

static inline unsigned int of_gpio_count(struct device_node *np)
{
	return 0;
}

#endif /* CONFIG_OF_GPIO */

/**
 * of_get_gpio - Get a GPIO number to use with GPIO API
 * @np:		device node to get GPIO from
 * @index:	index of the GPIO
 *
 * Returns GPIO number to use with Linux generic GPIO API, or one of the errno
 * value on the error condition.
 */
static inline int of_get_gpio(struct device_node *np, int index)
{
	return of_get_gpio_flags(np, index, NULL);
}

#endif /* __LINUX_OF_GPIO_H */
