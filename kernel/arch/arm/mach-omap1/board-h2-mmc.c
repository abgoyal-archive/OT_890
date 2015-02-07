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
 * linux/arch/arm/mach-omap1/board-h2-mmc.c
 *
 * Copyright (C) 2007 Instituto Nokia de Tecnologia - INdT
 * Author: Felipe Balbi <felipe.lima@indt.org.br>
 *
 * This code is based on linux/arch/arm/mach-omap2/board-n800-mmc.c, which is:
 * Copyright (C) 2006 Nokia Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/platform_device.h>

#include <linux/i2c/tps65010.h>

#include <mach/mmc.h>
#include <mach/gpio.h>

#if defined(CONFIG_MMC_OMAP) || defined(CONFIG_MMC_OMAP_MODULE)

static int mmc_set_power(struct device *dev, int slot, int power_on,
				int vdd)
{
	if (power_on)
		gpio_direction_output(H2_TPS_GPIO_MMC_PWR_EN, 1);
	else
		gpio_direction_output(H2_TPS_GPIO_MMC_PWR_EN, 0);

	return 0;
}

static int mmc_late_init(struct device *dev)
{
	int ret;

	ret = gpio_request(H2_TPS_GPIO_MMC_PWR_EN, "MMC power");
	if (ret < 0)
		return ret;

	gpio_direction_output(H2_TPS_GPIO_MMC_PWR_EN, 0);

	return ret;
}

static void mmc_shutdown(struct device *dev)
{
	gpio_free(H2_TPS_GPIO_MMC_PWR_EN);
}

/*
 * H2 could use the following functions tested:
 * - mmc_get_cover_state that uses OMAP_MPUIO(1)
 * - mmc_get_wp that uses OMAP_MPUIO(3)
 */
static struct omap_mmc_platform_data mmc1_data = {
	.nr_slots                       = 1,
	.init				= mmc_late_init,
	.shutdown			= mmc_shutdown,
	.dma_mask			= 0xffffffff,
	.slots[0]       = {
		.set_power              = mmc_set_power,
		.ocr_mask               = MMC_VDD_28_29 | MMC_VDD_30_31 |
					  MMC_VDD_32_33 | MMC_VDD_33_34,
		.name                   = "mmcblk",
	},
};

static struct omap_mmc_platform_data *mmc_data[OMAP16XX_NR_MMC];

void __init h2_mmc_init(void)
{
	mmc_data[0] = &mmc1_data;
	omap1_init_mmc(mmc_data, OMAP16XX_NR_MMC);
}

#else

void __init h2_mmc_init(void)
{
}

#endif
