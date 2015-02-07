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
 * arch/arm/mach-ns9xxx/gpio.c
 *
 * Copyright (C) 2006,2007 by Digi International Inc.
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 */
#include <linux/kernel.h>
#include <linux/compiler.h>
#include <linux/init.h>
#include <linux/spinlock.h>
#include <linux/module.h>
#include <linux/bitops.h>

#include <mach/gpio.h>
#include <mach/processor.h>
#include <mach/processor-ns9360.h>
#include <asm/bug.h>
#include <asm/types.h>

#include "gpio-ns9360.h"

#if defined(CONFIG_PROCESSOR_NS9360)
#define GPIO_MAX 72
#elif defined(CONFIG_PROCESSOR_NS9750)
#define GPIO_MAX 49
#endif

/* protects BBU_GCONFx and BBU_GCTRLx */
static spinlock_t gpio_lock = __SPIN_LOCK_UNLOCKED(gpio_lock);

/* only access gpiores with atomic ops */
static DECLARE_BITMAP(gpiores, GPIO_MAX + 1);

static inline int ns9xxx_valid_gpio(unsigned gpio)
{
#if defined(CONFIG_PROCESSOR_NS9360)
	if (processor_is_ns9360())
		return gpio <= 72;
	else
#endif
#if defined(CONFIG_PROCESSOR_NS9750)
	if (processor_is_ns9750())
		return gpio <= 49;
	else
#endif
	{
		BUG();
		return 0;
	}
}

int gpio_request(unsigned gpio, const char *label)
{
	if (likely(ns9xxx_valid_gpio(gpio)))
		return test_and_set_bit(gpio, gpiores) ? -EBUSY : 0;
	else
		return -EINVAL;
}
EXPORT_SYMBOL(gpio_request);

void gpio_free(unsigned gpio)
{
	might_sleep();
	clear_bit(gpio, gpiores);
	return;
}
EXPORT_SYMBOL(gpio_free);

int gpio_direction_input(unsigned gpio)
{
	if (likely(ns9xxx_valid_gpio(gpio))) {
		int ret = -EINVAL;
		unsigned long flags;

		spin_lock_irqsave(&gpio_lock, flags);
#if defined(CONFIG_PROCESSOR_NS9360)
		if (processor_is_ns9360())
			ret = __ns9360_gpio_configure(gpio, 0, 0, 3);
		else
#endif
			BUG();

		spin_unlock_irqrestore(&gpio_lock, flags);

		return ret;

	} else
		return -EINVAL;
}
EXPORT_SYMBOL(gpio_direction_input);

int gpio_direction_output(unsigned gpio, int value)
{
	if (likely(ns9xxx_valid_gpio(gpio))) {
		int ret = -EINVAL;
		unsigned long flags;

		gpio_set_value(gpio, value);

		spin_lock_irqsave(&gpio_lock, flags);
#if defined(CONFIG_PROCESSOR_NS9360)
		if (processor_is_ns9360())
			ret = __ns9360_gpio_configure(gpio, 1, 0, 3);
		else
#endif
			BUG();

		spin_unlock_irqrestore(&gpio_lock, flags);

		return ret;
	} else
		return -EINVAL;
}
EXPORT_SYMBOL(gpio_direction_output);

int gpio_get_value(unsigned gpio)
{
#if defined(CONFIG_PROCESSOR_NS9360)
	if (processor_is_ns9360())
		return ns9360_gpio_get_value(gpio);
	else
#endif
	{
		BUG();
		return -EINVAL;
	}
}
EXPORT_SYMBOL(gpio_get_value);

void gpio_set_value(unsigned gpio, int value)
{
	unsigned long flags;
	spin_lock_irqsave(&gpio_lock, flags);
#if defined(CONFIG_PROCESSOR_NS9360)
	if (processor_is_ns9360())
		ns9360_gpio_set_value(gpio, value);
	else
#endif
		BUG();

	spin_unlock_irqrestore(&gpio_lock, flags);
}
EXPORT_SYMBOL(gpio_set_value);
