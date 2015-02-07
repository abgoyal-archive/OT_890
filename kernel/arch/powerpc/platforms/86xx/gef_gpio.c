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
 * Driver for GE Fanuc's FPGA based GPIO pins
 *
 * Author: Martyn Welch <martyn.welch@gefanuc.com>
 *
 * 2008 (c) GE Fanuc Intelligent Platforms Embedded Systems, Inc.
 *
 * This file is licensed under the terms of the GNU General Public License
 * version 2.  This program is licensed "as is" without any warranty of any
 * kind, whether express or implied.
 */

/* TODO
 *
 * Configuration of output modes (totem-pole/open-drain)
 * Interrupt configuration - interrupts are always generated the FPGA relies on
 * 	the I/O interrupt controllers mask to stop them propergating
 */

#include <linux/kernel.h>
#include <linux/compiler.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_platform.h>
#include <linux/of_gpio.h>
#include <linux/gpio.h>

#define GEF_GPIO_DIRECT		0x00
#define GEF_GPIO_IN		0x04
#define GEF_GPIO_OUT		0x08
#define GEF_GPIO_TRIG		0x0C
#define GEF_GPIO_POLAR_A	0x10
#define GEF_GPIO_POLAR_B	0x14
#define GEF_GPIO_INT_STAT	0x18
#define GEF_GPIO_OVERRUN	0x1C
#define GEF_GPIO_MODE		0x20

#define NUM_GPIO 19

static void _gef_gpio_set(void __iomem *reg, unsigned int offset, int value)
{
	unsigned int data;

	data = ioread32be(reg);
	/* value: 0=low; 1=high */
	if (value & 0x1)
		data = data | (0x1 << offset);
	else
		data = data & ~(0x1 << offset);

	iowrite32be(data, reg);
}


static int gef_gpio_dir_in(struct gpio_chip *chip, unsigned offset)
{
	unsigned int data;
	struct of_mm_gpio_chip *mmchip = to_of_mm_gpio_chip(chip);

	data = ioread32be(mmchip->regs + GEF_GPIO_DIRECT);
	data = data | (0x1 << offset);
	iowrite32be(data, mmchip->regs + GEF_GPIO_DIRECT);

	return 0;
}

static int gef_gpio_dir_out(struct gpio_chip *chip, unsigned offset, int value)
{
	unsigned int data;
	struct of_mm_gpio_chip *mmchip = to_of_mm_gpio_chip(chip);

	/* Set direction before switching to input */
	_gef_gpio_set(mmchip->regs + GEF_GPIO_OUT, offset, value);

	data = ioread32be(mmchip->regs + GEF_GPIO_DIRECT);
	data = data & ~(0x1 << offset);
	iowrite32be(data, mmchip->regs + GEF_GPIO_DIRECT);

	return 0;
}

static int gef_gpio_get(struct gpio_chip *chip, unsigned offset)
{
	unsigned int data;
	int state = 0;
	struct of_mm_gpio_chip *mmchip = to_of_mm_gpio_chip(chip);

	data = ioread32be(mmchip->regs + GEF_GPIO_IN);
	state = (int)((data >> offset) & 0x1);

	return state;
}

static void gef_gpio_set(struct gpio_chip *chip, unsigned offset, int value)
{
	struct of_mm_gpio_chip *mmchip = to_of_mm_gpio_chip(chip);

	_gef_gpio_set(mmchip->regs + GEF_GPIO_OUT, offset, value);
}

static int __init gef_gpio_init(void)
{
	struct device_node *np;

	for_each_compatible_node(np, NULL, "gef,sbc610-gpio") {
		int retval;
		struct of_mm_gpio_chip *gef_gpio_chip;

		pr_debug("%s: Initialising GEF GPIO\n", np->full_name);

		/* Allocate chip structure */
		gef_gpio_chip = kzalloc(sizeof(*gef_gpio_chip), GFP_KERNEL);
		if (!gef_gpio_chip) {
			pr_err("%s: Unable to allocate structure\n",
				np->full_name);
			continue;
		}

		/* Setup pointers to chip functions */
		gef_gpio_chip->of_gc.gpio_cells = 2;
		gef_gpio_chip->of_gc.gc.ngpio = NUM_GPIO;
		gef_gpio_chip->of_gc.gc.direction_input = gef_gpio_dir_in;
		gef_gpio_chip->of_gc.gc.direction_output = gef_gpio_dir_out;
		gef_gpio_chip->of_gc.gc.get = gef_gpio_get;
		gef_gpio_chip->of_gc.gc.set = gef_gpio_set;

		/* This function adds a memory mapped GPIO chip */
		retval = of_mm_gpiochip_add(np, gef_gpio_chip);
		if (retval) {
			kfree(gef_gpio_chip);
			pr_err("%s: Unable to add GPIO\n", np->full_name);
		}
	}

	return 0;
};
arch_initcall(gef_gpio_init);

MODULE_DESCRIPTION("GE Fanuc I/O FPGA GPIO driver");
MODULE_AUTHOR("Martyn Welch <martyn.welch@gefanuc.com");
MODULE_LICENSE("GPL");
