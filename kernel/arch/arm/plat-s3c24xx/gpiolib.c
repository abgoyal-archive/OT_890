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

/* linux/arch/arm/plat-s3c24xx/gpiolib.c
 *
 * Copyright (c) 2008 Simtec Electronics
 *	http://armlinux.simtec.co.uk/
 *	Ben Dooks <ben@simtec.co.uk>
 *
 * S3C24XX GPIOlib support
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
*/

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/io.h>
#include <linux/gpio.h>

#include <plat/gpio-core.h>
#include <mach/hardware.h>
#include <asm/irq.h>

#include <mach/regs-gpio.h>

static int s3c24xx_gpiolib_banka_input(struct gpio_chip *chip, unsigned offset)
{
	return -EINVAL;
}

static int s3c24xx_gpiolib_banka_output(struct gpio_chip *chip,
					unsigned offset, int value)
{
	struct s3c_gpio_chip *ourchip = to_s3c_gpio(chip);
	void __iomem *base = ourchip->base;
	unsigned long flags;
	unsigned long dat;
	unsigned long con;

	local_irq_save(flags);

	con = __raw_readl(base + 0x00);
	dat = __raw_readl(base + 0x04);

	dat &= ~(1 << offset);
	if (value)
		dat |= 1 << offset;

	__raw_writel(dat, base + 0x04);

	con &= ~(1 << offset);

	__raw_writel(con, base + 0x00);
	__raw_writel(dat, base + 0x04);

	local_irq_restore(flags);
	return 0;
}

static int s3c24xx_gpiolib_bankf_toirq(struct gpio_chip *chip, unsigned offset)
{
	if (offset < 4)
		return IRQ_EINT0 + offset;
	
	if (offset < 8)
		return IRQ_EINT4 + offset - 4;
	
	return -EINVAL;
}

static int s3c24xx_gpiolib_bankg_toirq(struct gpio_chip *chip, unsigned offset)
{
	return IRQ_EINT8 + offset;
}

struct s3c_gpio_chip s3c24xx_gpios[] = {
	[0] = {
		.base	= S3C24XX_GPIO_BASE(S3C2410_GPA0),
		.chip	= {
			.base			= S3C2410_GPA0,
			.owner			= THIS_MODULE,
			.label			= "GPIOA",
			.ngpio			= 24,
			.direction_input	= s3c24xx_gpiolib_banka_input,
			.direction_output	= s3c24xx_gpiolib_banka_output,
		},
	},
	[1] = {
		.base	= S3C24XX_GPIO_BASE(S3C2410_GPB0),
		.chip	= {
			.base			= S3C2410_GPB0,
			.owner			= THIS_MODULE,
			.label			= "GPIOB",
			.ngpio			= 16,
		},
	},
	[2] = {
		.base	= S3C24XX_GPIO_BASE(S3C2410_GPC0),
		.chip	= {
			.base			= S3C2410_GPC0,
			.owner			= THIS_MODULE,
			.label			= "GPIOC",
			.ngpio			= 16,
		},
	},
	[3] = {
		.base	= S3C24XX_GPIO_BASE(S3C2410_GPD0),
		.chip	= {
			.base			= S3C2410_GPD0,
			.owner			= THIS_MODULE,
			.label			= "GPIOD",
			.ngpio			= 16,
		},
	},
	[4] = {
		.base	= S3C24XX_GPIO_BASE(S3C2410_GPE0),
		.chip	= {
			.base			= S3C2410_GPE0,
			.label			= "GPIOE",
			.owner			= THIS_MODULE,
			.ngpio			= 16,
		},
	},
	[5] = {
		.base	= S3C24XX_GPIO_BASE(S3C2410_GPF0),
		.chip	= {
			.base			= S3C2410_GPF0,
			.owner			= THIS_MODULE,
			.label			= "GPIOF",
			.ngpio			= 8,
			.to_irq			= s3c24xx_gpiolib_bankf_toirq,
		},
	},
	[6] = {
		.base	= S3C24XX_GPIO_BASE(S3C2410_GPG0),
		.chip	= {
			.base			= S3C2410_GPG0,
			.owner			= THIS_MODULE,
			.label			= "GPIOG",
			.ngpio			= 10,
			.to_irq			= s3c24xx_gpiolib_bankg_toirq,
		},
	},
};

static __init int s3c24xx_gpiolib_init(void)
{
	struct s3c_gpio_chip *chip = s3c24xx_gpios;
	int gpn;

	for (gpn = 0; gpn < ARRAY_SIZE(s3c24xx_gpios); gpn++, chip++)
		s3c_gpiolib_add(chip);

	return 0;
}

arch_initcall(s3c24xx_gpiolib_init);
