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
 * linux/arch/arm/mach-sa1100/gpio.c
 *
 * Generic SA-1100 GPIO handling
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/init.h>
#include <linux/module.h>

#include <asm/gpio.h>
#include <mach/hardware.h>
#include "generic.h"

static int sa1100_gpio_get(struct gpio_chip *chip, unsigned offset)
{
	return GPLR & GPIO_GPIO(offset);
}

static void sa1100_gpio_set(struct gpio_chip *chip, unsigned offset, int value)
{
	if (value)
		GPSR = GPIO_GPIO(offset);
	else
		GPCR = GPIO_GPIO(offset);
}

static int sa1100_direction_input(struct gpio_chip *chip, unsigned offset)
{
	unsigned long flags;

	local_irq_save(flags);
	GPDR &= ~GPIO_GPIO(offset);
	local_irq_restore(flags);
	return 0;
}

static int sa1100_direction_output(struct gpio_chip *chip, unsigned offset, int value)
{
	unsigned long flags;

	local_irq_save(flags);
	sa1100_gpio_set(chip, offset, value);
	GPDR |= GPIO_GPIO(offset);
	local_irq_restore(flags);
	return 0;
}

static struct gpio_chip sa1100_gpio_chip = {
	.label			= "gpio",
	.direction_input	= sa1100_direction_input,
	.direction_output	= sa1100_direction_output,
	.set			= sa1100_gpio_set,
	.get			= sa1100_gpio_get,
	.base			= 0,
	.ngpio			= GPIO_MAX + 1,
};

void __init sa1100_init_gpio(void)
{
	gpiochip_add(&sa1100_gpio_chip);
}
