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
 * arch/arm/mach-orion5x/mpp.c
 *
 * MPP functions for Marvell Orion 5x SoCs
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2.  This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/mbus.h>
#include <linux/io.h>
#include <asm/gpio.h>
#include <mach/hardware.h>
#include "common.h"
#include "mpp.h"

static int is_5181l(void)
{
	u32 dev;
	u32 rev;

	orion5x_pcie_id(&dev, &rev);

	return !!(dev == MV88F5181_DEV_ID && rev >= MV88F5181L_REV_A0);
}

static int is_5182(void)
{
	u32 dev;
	u32 rev;

	orion5x_pcie_id(&dev, &rev);

	return !!(dev == MV88F5182_DEV_ID);
}

static int is_5281(void)
{
	u32 dev;
	u32 rev;

	orion5x_pcie_id(&dev, &rev);

	return !!(dev == MV88F5281_DEV_ID);
}

static int __init determine_type_encoding(int mpp, enum orion5x_mpp_type type)
{
	switch (type) {
	case MPP_UNUSED:
	case MPP_GPIO:
		if (mpp == 0)
			return 3;
		if (mpp >= 1 && mpp <= 15)
			return 0;
		if (mpp >= 16 && mpp <= 19) {
			if (is_5182())
				return 5;
			if (type == MPP_UNUSED)
				return 0;
		}
		return -1;

	case MPP_PCIE_RST_OUTn:
		if (mpp == 0)
			return 0;
		return -1;

	case MPP_PCI_ARB:
		if (mpp >= 0 && mpp <= 7)
			return 2;
		return -1;

	case MPP_PCI_PMEn:
		if (mpp == 2)
			return 3;
		return -1;

	case MPP_GIGE:
		if (mpp >= 8 && mpp <= 19)
			return 1;
		return -1;

	case MPP_NAND:
		if (is_5182() || is_5281()) {
			if (mpp >= 4 && mpp <= 7)
				return 4;
			if (mpp >= 12 && mpp <= 17)
				return 4;
		}
		return -1;

	case MPP_PCI_CLK:
		if (is_5181l() && mpp >= 6 && mpp <= 7)
			return 5;
		return -1;

	case MPP_SATA_LED:
		if (is_5182()) {
			if (mpp >= 4 && mpp <= 7)
				return 5;
			if (mpp >= 12 && mpp <= 15)
				return 5;
		}
		return -1;

	case MPP_UART:
		if (mpp >= 16 && mpp <= 19)
			return 0;
		return -1;
	}

	printk(KERN_INFO "unknown MPP type %d\n", type);

	return -1;
}

void __init orion5x_mpp_conf(struct orion5x_mpp_mode *mode)
{
	u32 mpp_0_7_ctrl = readl(MPP_0_7_CTRL);
	u32 mpp_8_15_ctrl = readl(MPP_8_15_CTRL);
	u32 mpp_16_19_ctrl = readl(MPP_16_19_CTRL);

	while (mode->mpp >= 0) {
		u32 *reg;
		int num_type;
		int shift;

		if (mode->mpp >= 0 && mode->mpp <= 7)
			reg = &mpp_0_7_ctrl;
		else if (mode->mpp >= 8 && mode->mpp <= 15)
			reg = &mpp_8_15_ctrl;
		else if (mode->mpp >= 16 && mode->mpp <= 19)
			reg = &mpp_16_19_ctrl;
		else {
			printk(KERN_ERR "orion5x_mpp_conf: invalid MPP "
					"(%d)\n", mode->mpp);
			continue;
		}

		num_type = determine_type_encoding(mode->mpp, mode->type);
		if (num_type < 0) {
			printk(KERN_ERR "orion5x_mpp_conf: invalid MPP "
					"combination (%d, %d)\n", mode->mpp,
					mode->type);
			continue;
		}

		shift = (mode->mpp & 7) << 2;
		*reg &= ~(0xf << shift);
		*reg |= (num_type & 0xf) << shift;

		if (mode->type == MPP_UNUSED && (mode->mpp < 16 || is_5182()))
			orion_gpio_set_unused(mode->mpp);

		orion_gpio_set_valid(mode->mpp, !!(mode->type == MPP_GPIO));

		mode++;
	}

	writel(mpp_0_7_ctrl, MPP_0_7_CTRL);
	writel(mpp_8_15_ctrl, MPP_8_15_CTRL);
	writel(mpp_16_19_ctrl, MPP_16_19_CTRL);
}
