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
 * arch/arm/plat-iop/gpio.c
 * GPIO handling for Intel IOP3xx processors.
 *
 * Copyright (C) 2006 Lennert Buytenhek <buytenh@wantstofly.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */

#include <linux/device.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/gpio.h>
#include <asm/hardware/iop3xx.h>

void gpio_line_config(int line, int direction)
{
	unsigned long flags;

	local_irq_save(flags);
	if (direction == GPIO_IN) {
		*IOP3XX_GPOE |= 1 << line;
	} else if (direction == GPIO_OUT) {
		*IOP3XX_GPOE &= ~(1 << line);
	}
	local_irq_restore(flags);
}
EXPORT_SYMBOL(gpio_line_config);

int gpio_line_get(int line)
{
	return !!(*IOP3XX_GPID & (1 << line));
}
EXPORT_SYMBOL(gpio_line_get);

void gpio_line_set(int line, int value)
{
	unsigned long flags;

	local_irq_save(flags);
	if (value == GPIO_LOW) {
		*IOP3XX_GPOD &= ~(1 << line);
	} else if (value == GPIO_HIGH) {
		*IOP3XX_GPOD |= 1 << line;
	}
	local_irq_restore(flags);
}
EXPORT_SYMBOL(gpio_line_set);

static int iop3xx_gpio_direction_input(struct gpio_chip *chip, unsigned gpio)
{
	gpio_line_config(gpio, GPIO_IN);
	return 0;
}

static int iop3xx_gpio_direction_output(struct gpio_chip *chip, unsigned gpio, int level)
{
	gpio_line_set(gpio, level);
	gpio_line_config(gpio, GPIO_OUT);
	return 0;
}

static int iop3xx_gpio_get_value(struct gpio_chip *chip, unsigned gpio)
{
	return gpio_line_get(gpio);
}

static void iop3xx_gpio_set_value(struct gpio_chip *chip, unsigned gpio, int value)
{
	gpio_line_set(gpio, value);
}

static struct gpio_chip iop3xx_chip = {
	.label			= "iop3xx",
	.direction_input	= iop3xx_gpio_direction_input,
	.get			= iop3xx_gpio_get_value,
	.direction_output	= iop3xx_gpio_direction_output,
	.set			= iop3xx_gpio_set_value,
	.base			= 0,
	.ngpio			= IOP3XX_N_GPIOS,
};

static int __init iop3xx_gpio_setup(void)
{
	return gpiochip_add(&iop3xx_chip);
}
arch_initcall(iop3xx_gpio_setup);
