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
 * AMD Geode southbridge support code
 * Copyright (C) 2006, Advanced Micro Devices, Inc.
 * Copyright (C) 2007, Andres Salomon <dilinger@debian.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of version 2 of the GNU General Public License
 * as published by the Free Software Foundation.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/ioport.h>
#include <linux/io.h>
#include <asm/msr.h>
#include <asm/geode.h>

static struct {
	char *name;
	u32 msr;
	int size;
	u32 base;
} lbars[] = {
	{ "geode-pms",   MSR_LBAR_PMS, LBAR_PMS_SIZE, 0 },
	{ "geode-acpi",  MSR_LBAR_ACPI, LBAR_ACPI_SIZE, 0 },
	{ "geode-gpio",  MSR_LBAR_GPIO, LBAR_GPIO_SIZE, 0 },
	{ "geode-mfgpt", MSR_LBAR_MFGPT, LBAR_MFGPT_SIZE, 0 }
};

static void __init init_lbars(void)
{
	u32 lo, hi;
	int i;

	for (i = 0; i < ARRAY_SIZE(lbars); i++) {
		rdmsr(lbars[i].msr, lo, hi);
		if (hi & 0x01)
			lbars[i].base = lo & 0x0000ffff;

		if (lbars[i].base == 0)
			printk(KERN_ERR "geode:  Couldn't initialize '%s'\n",
					lbars[i].name);
	}
}

int geode_get_dev_base(unsigned int dev)
{
	BUG_ON(dev >= ARRAY_SIZE(lbars));
	return lbars[dev].base;
}
EXPORT_SYMBOL_GPL(geode_get_dev_base);

/* === GPIO API === */

void geode_gpio_set(u32 gpio, unsigned int reg)
{
	u32 base = geode_get_dev_base(GEODE_DEV_GPIO);

	if (!base)
		return;

	/* low bank register */
	if (gpio & 0xFFFF)
		outl(gpio & 0xFFFF, base + reg);
	/* high bank register */
	gpio >>= 16;
	if (gpio)
		outl(gpio, base + 0x80 + reg);
}
EXPORT_SYMBOL_GPL(geode_gpio_set);

void geode_gpio_clear(u32 gpio, unsigned int reg)
{
	u32 base = geode_get_dev_base(GEODE_DEV_GPIO);

	if (!base)
		return;

	/* low bank register */
	if (gpio & 0xFFFF)
		outl((gpio & 0xFFFF) << 16, base + reg);
	/* high bank register */
	gpio &= (0xFFFF << 16);
	if (gpio)
		outl(gpio, base + 0x80 + reg);
}
EXPORT_SYMBOL_GPL(geode_gpio_clear);

int geode_gpio_isset(u32 gpio, unsigned int reg)
{
	u32 base = geode_get_dev_base(GEODE_DEV_GPIO);
	u32 val;

	if (!base)
		return 0;

	/* low bank register */
	if (gpio & 0xFFFF) {
		val = inl(base + reg) & (gpio & 0xFFFF);
		if ((gpio & 0xFFFF) == val)
			return 1;
	}
	/* high bank register */
	gpio >>= 16;
	if (gpio) {
		val = inl(base + 0x80 + reg) & gpio;
		if (gpio == val)
			return 1;
	}
	return 0;
}
EXPORT_SYMBOL_GPL(geode_gpio_isset);

void geode_gpio_set_irq(unsigned int group, unsigned int irq)
{
	u32 lo, hi;

	if (group > 7 || irq > 15)
		return;

	rdmsr(MSR_PIC_ZSEL_HIGH, lo, hi);

	lo &= ~(0xF << (group * 4));
	lo |= (irq & 0xF) << (group * 4);

	wrmsr(MSR_PIC_ZSEL_HIGH, lo, hi);
}
EXPORT_SYMBOL_GPL(geode_gpio_set_irq);

void geode_gpio_setup_event(unsigned int gpio, int pair, int pme)
{
	u32 base = geode_get_dev_base(GEODE_DEV_GPIO);
	u32 offset, shift, val;

	if (gpio >= 24)
		offset = GPIO_MAP_W;
	else if (gpio >= 16)
		offset = GPIO_MAP_Z;
	else if (gpio >= 8)
		offset = GPIO_MAP_Y;
	else
		offset = GPIO_MAP_X;

	shift = (gpio % 8) * 4;

	val = inl(base + offset);

	/* Clear whatever was there before */
	val &= ~(0xF << shift);

	/* And set the new value */

	val |= ((pair & 7) << shift);

	/* Set the PME bit if this is a PME event */

	if (pme)
		val |= (1 << (shift + 3));

	outl(val, base + offset);
}
EXPORT_SYMBOL_GPL(geode_gpio_setup_event);

int geode_has_vsa2(void)
{
	static int has_vsa2 = -1;

	if (has_vsa2 == -1) {
		u16 val;

		/*
		 * The VSA has virtual registers that we can query for a
		 * signature.
		 */
		outw(VSA_VR_UNLOCK, VSA_VRC_INDEX);
		outw(VSA_VR_SIGNATURE, VSA_VRC_INDEX);

		val = inw(VSA_VRC_DATA);
		has_vsa2 = (val == AMD_VSA_SIG || val == GSW_VSA_SIG);
	}

	return has_vsa2;
}
EXPORT_SYMBOL_GPL(geode_has_vsa2);

static int __init geode_southbridge_init(void)
{
	if (!is_geode())
		return -ENODEV;

	init_lbars();
	(void) mfgpt_timer_setup();
	return 0;
}

postcore_initcall(geode_southbridge_init);
