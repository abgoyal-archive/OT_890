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
 * linux/arch/arm/mach-omap1/board-sx1-mmc.c
 *
 * Copyright (C) 2007 Instituto Nokia de Tecnologia - INdT
 * Author: Carlos Eduardo Aguiar <carlos.aguiar@indt.org.br>
 *
 * This code is based on linux/arch/arm/mach-omap1/board-h2-mmc.c, which is:
 * Copyright (C) 2007 Instituto Nokia de Tecnologia - INdT
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/platform_device.h>

#include <mach/hardware.h>
#include <mach/mmc.h>
#include <mach/gpio.h>

#if defined(CONFIG_MMC_OMAP) || defined(CONFIG_MMC_OMAP_MODULE)

static int mmc_set_power(struct device *dev, int slot, int power_on,
				int vdd)
{
	int err;
	u8 dat = 0;

	err = sx1_i2c_read_byte(SOFIA_I2C_ADDR, SOFIA_POWER1_REG, &dat);
	if (err < 0)
		return err;

	if (power_on)
		dat |= SOFIA_MMC_POWER;
	else
		dat &= ~SOFIA_MMC_POWER;

	return sx1_i2c_write_byte(SOFIA_I2C_ADDR, SOFIA_POWER1_REG, dat);
}

/* Cover switch is at OMAP_MPUIO(3) */
static struct omap_mmc_platform_data mmc1_data = {
	.nr_slots                       = 1,
	.slots[0]       = {
		.set_power              = mmc_set_power,
		.ocr_mask               = MMC_VDD_28_29 | MMC_VDD_30_31 |
					  MMC_VDD_32_33 | MMC_VDD_33_34,
		.name                   = "mmcblk",
	},
};

static struct omap_mmc_platform_data *mmc_data[OMAP15XX_NR_MMC];

void __init sx1_mmc_init(void)
{
	mmc_data[0] = &mmc1_data;
	omap1_init_mmc(mmc_data, OMAP15XX_NR_MMC);
}

#else

void __init sx1_mmc_init(void)
{
}

#endif
