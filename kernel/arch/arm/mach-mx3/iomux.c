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
 * Copyright 2004-2006 Freescale Semiconductor, Inc. All Rights Reserved.
 * Copyright (C) 2008 by Sascha Hauer <kernel@pengutronix.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

#include <linux/module.h>
#include <linux/spinlock.h>
#include <linux/io.h>
#include <linux/gpio.h>
#include <mach/hardware.h>
#include <mach/gpio.h>
#include <mach/iomux-mx3.h>

/*
 * IOMUX register (base) addresses
 */
#define IOMUX_BASE	IO_ADDRESS(IOMUXC_BASE_ADDR)
#define IOMUXINT_OBS1	(IOMUX_BASE + 0x000)
#define IOMUXINT_OBS2	(IOMUX_BASE + 0x004)
#define IOMUXGPR	(IOMUX_BASE + 0x008)
#define IOMUXSW_MUX_CTL	(IOMUX_BASE + 0x00C)
#define IOMUXSW_PAD_CTL	(IOMUX_BASE + 0x154)

static DEFINE_SPINLOCK(gpio_mux_lock);

#define IOMUX_REG_MASK (IOMUX_PADNUM_MASK & ~0x3)
/*
 * set the mode for a IOMUX pin.
 */
int mxc_iomux_mode(unsigned int pin_mode)
{
	u32 field, l, mode, ret = 0;
	void __iomem *reg;

	reg = IOMUXSW_MUX_CTL + (pin_mode & IOMUX_REG_MASK);
	field = pin_mode & 0x3;
	mode = (pin_mode & IOMUX_MODE_MASK) >> IOMUX_MODE_SHIFT;

	pr_debug("%s: reg offset = 0x%x field = %d mode = 0x%02x\n",
			__func__, (pin_mode & IOMUX_REG_MASK), field, mode);

	spin_lock(&gpio_mux_lock);

	l = __raw_readl(reg);
	l &= ~(0xff << (field * 8));
	l |= mode << (field * 8);
	__raw_writel(l, reg);

	spin_unlock(&gpio_mux_lock);

	return ret;
}
EXPORT_SYMBOL(mxc_iomux_mode);

/*
 * This function configures the pad value for a IOMUX pin.
 */
void mxc_iomux_set_pad(enum iomux_pins pin, u32 config)
{
	u32 field, l;
	void __iomem *reg;

	pin &= IOMUX_PADNUM_MASK;
	reg = IOMUXSW_PAD_CTL + (pin + 2) / 3 * 4;
	field = (pin + 2) % 3;

	pr_debug("%s: reg offset = 0x%x, field = %d\n",
			__func__, (pin + 2) / 3, field);

	spin_lock(&gpio_mux_lock);

	l = __raw_readl(reg);
	l &= ~(0x1ff << (field * 10));
	l |= config << (field * 10);
	__raw_writel(l, reg);

	spin_unlock(&gpio_mux_lock);
}
EXPORT_SYMBOL(mxc_iomux_set_pad);

/*
 * This function enables/disables the general purpose function for a particular
 * signal.
 */
void mxc_iomux_set_gpr(enum iomux_gp_func gp, bool en)
{
	u32 l;

	spin_lock(&gpio_mux_lock);
	l = __raw_readl(IOMUXGPR);
	if (en)
		l |= gp;
	else
		l &= ~gp;

	__raw_writel(l, IOMUXGPR);
	spin_unlock(&gpio_mux_lock);
}
EXPORT_SYMBOL(mxc_iomux_set_gpr);

