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
 * @file /arch/mips/pmc-sierra/msp71xx/gpio_extended.c
 *
 * Generic PMC MSP71xx EXTENDED (EXD) GPIO handling. The extended gpio is
 * a set of hardware registers that have no need for explicit locking as
 * it is handled by unique method of writing individual set/clr bits.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * @author Patrick Glass <patrickglass@gmail.com>
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/gpio.h>
#include <linux/io.h>

#define MSP71XX_DATA_OFFSET(gpio)	(2 * (gpio))
#define MSP71XX_READ_OFFSET(gpio)	(MSP71XX_DATA_OFFSET(gpio) + 1)
#define MSP71XX_CFG_OUT_OFFSET(gpio)	(MSP71XX_DATA_OFFSET(gpio) + 16)
#define MSP71XX_CFG_IN_OFFSET(gpio)	(MSP71XX_CFG_OUT_OFFSET(gpio) + 1)

#define MSP71XX_EXD_GPIO_BASE	0x0BC000000L

#define to_msp71xx_exd_gpio_chip(c) \
			container_of(c, struct msp71xx_exd_gpio_chip, chip)

/*
 * struct msp71xx_exd_gpio_chip - container for gpio chip and registers
 * @chip: chip structure for the specified gpio bank
 * @reg: register for control and data of gpio pin
 */
struct msp71xx_exd_gpio_chip {
	struct gpio_chip chip;
	void __iomem *reg;
};

/*
 * msp71xx_exd_gpio_get() - return the chip's gpio value
 * @chip: chip structure which controls the specified gpio
 * @offset: gpio whose value will be returned
 *
 * It will return 0 if gpio value is low and other if high.
 */
static int msp71xx_exd_gpio_get(struct gpio_chip *chip, unsigned offset)
{
	struct msp71xx_exd_gpio_chip *msp71xx_chip =
	    to_msp71xx_exd_gpio_chip(chip);
	const unsigned bit = MSP71XX_READ_OFFSET(offset);

	return __raw_readl(msp71xx_chip->reg) & (1 << bit);
}

/*
 * msp71xx_exd_gpio_set() - set the output value for the gpio
 * @chip: chip structure who controls the specified gpio
 * @offset: gpio whose value will be assigned
 * @value: logic level to assign to the gpio initially
 *
 * This will set the gpio bit specified to the desired value. It will set the
 * gpio pin low if value is 0 otherwise it will be high.
 */
static void msp71xx_exd_gpio_set(struct gpio_chip *chip,
				 unsigned offset, int value)
{
	struct msp71xx_exd_gpio_chip *msp71xx_chip =
	    to_msp71xx_exd_gpio_chip(chip);
	const unsigned bit = MSP71XX_DATA_OFFSET(offset);

	__raw_writel(1 << (bit + (value ? 1 : 0)), msp71xx_chip->reg);
}

/*
 * msp71xx_exd_direction_output() - declare the direction mode for a gpio
 * @chip: chip structure which controls the specified gpio
 * @offset: gpio whose value will be assigned
 * @value: logic level to assign to the gpio initially
 *
 * This call will set the mode for the @gpio to output. It will set the
 * gpio pin low if value is 0 otherwise it will be high.
 */
static int msp71xx_exd_direction_output(struct gpio_chip *chip,
					unsigned offset, int value)
{
	struct msp71xx_exd_gpio_chip *msp71xx_chip =
	    to_msp71xx_exd_gpio_chip(chip);

	msp71xx_exd_gpio_set(chip, offset, value);
	__raw_writel(1 << MSP71XX_CFG_OUT_OFFSET(offset), msp71xx_chip->reg);
	return 0;
}

/*
 * msp71xx_exd_direction_input() - declare the direction mode for a gpio
 * @chip: chip structure which controls the specified gpio
 * @offset: gpio whose to which the value will be assigned
 *
 * This call will set the mode for the @gpio to input.
 */
static int msp71xx_exd_direction_input(struct gpio_chip *chip, unsigned offset)
{
	struct msp71xx_exd_gpio_chip *msp71xx_chip =
	    to_msp71xx_exd_gpio_chip(chip);

	__raw_writel(1 << MSP71XX_CFG_IN_OFFSET(offset), msp71xx_chip->reg);
	return 0;
}

#define MSP71XX_EXD_GPIO_BANK(name, exd_reg, base_gpio, num_gpio) \
{ \
	.chip = { \
		.label		  = name, \
		.direction_input  = msp71xx_exd_direction_input, \
		.direction_output = msp71xx_exd_direction_output, \
		.get		  = msp71xx_exd_gpio_get, \
		.set		  = msp71xx_exd_gpio_set, \
		.base		  = base_gpio, \
		.ngpio		  = num_gpio, \
	}, \
	.reg	= (void __iomem *)(MSP71XX_EXD_GPIO_BASE + exd_reg), \
}

/*
 * struct msp71xx_exd_gpio_banks[] - container array of gpio banks
 * @chip: chip structure for the specified gpio bank
 * @reg: register for reading and writing the gpio pin value
 *
 * This array structure defines the extended gpio banks for the
 * PMC MIPS Processor. We specify the bank name, the data/config
 * register,the base starting gpio number, and the number of
 * gpios exposed by the bank of gpios.
 */
static struct msp71xx_exd_gpio_chip msp71xx_exd_gpio_banks[] = {

	MSP71XX_EXD_GPIO_BANK("GPIO_23_16", 0x188, 16, 8),
	MSP71XX_EXD_GPIO_BANK("GPIO_27_24", 0x18C, 24, 4),
};

void __init msp71xx_init_gpio_extended(void)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(msp71xx_exd_gpio_banks); i++)
		gpiochip_add(&msp71xx_exd_gpio_banks[i].chip);
}
