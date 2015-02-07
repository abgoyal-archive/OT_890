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
 * OpenFirmware bindings for the MMC-over-SPI driver
 *
 * Copyright (c) MontaVista Software, Inc. 2008.
 *
 * Author: Anton Vorontsov <avorontsov@ru.mvista.com>
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/gpio.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/spi/spi.h>
#include <linux/spi/mmc_spi.h>
#include <linux/mmc/core.h>
#include <linux/mmc/host.h>

enum {
	CD_GPIO = 0,
	WP_GPIO,
	NUM_GPIOS,
};

struct of_mmc_spi {
	int gpios[NUM_GPIOS];
	bool alow_gpios[NUM_GPIOS];
	struct mmc_spi_platform_data pdata;
};

static struct of_mmc_spi *to_of_mmc_spi(struct device *dev)
{
	return container_of(dev->platform_data, struct of_mmc_spi, pdata);
}

static int of_mmc_spi_read_gpio(struct device *dev, int gpio_num)
{
	struct of_mmc_spi *oms = to_of_mmc_spi(dev);
	bool active_low = oms->alow_gpios[gpio_num];
	bool value = gpio_get_value(oms->gpios[gpio_num]);

	return active_low ^ value;
}

static int of_mmc_spi_get_cd(struct device *dev)
{
	return of_mmc_spi_read_gpio(dev, CD_GPIO);
}

static int of_mmc_spi_get_ro(struct device *dev)
{
	return of_mmc_spi_read_gpio(dev, WP_GPIO);
}

struct mmc_spi_platform_data *mmc_spi_get_pdata(struct spi_device *spi)
{
	struct device *dev = &spi->dev;
	struct device_node *np = dev_archdata_get_node(&dev->archdata);
	struct of_mmc_spi *oms;
	const u32 *voltage_ranges;
	int num_ranges;
	int i;
	int ret = -EINVAL;

	if (dev->platform_data || !np)
		return dev->platform_data;

	oms = kzalloc(sizeof(*oms), GFP_KERNEL);
	if (!oms)
		return NULL;

	voltage_ranges = of_get_property(np, "voltage-ranges", &num_ranges);
	num_ranges = num_ranges / sizeof(*voltage_ranges) / 2;
	if (!voltage_ranges || !num_ranges) {
		dev_err(dev, "OF: voltage-ranges unspecified\n");
		goto err_ocr;
	}

	for (i = 0; i < num_ranges; i++) {
		const int j = i * 2;
		u32 mask;

		mask = mmc_vddrange_to_ocrmask(voltage_ranges[j],
					       voltage_ranges[j + 1]);
		if (!mask) {
			ret = -EINVAL;
			dev_err(dev, "OF: voltage-range #%d is invalid\n", i);
			goto err_ocr;
		}
		oms->pdata.ocr_mask |= mask;
	}

	for (i = 0; i < ARRAY_SIZE(oms->gpios); i++) {
		enum of_gpio_flags gpio_flags;

		oms->gpios[i] = of_get_gpio_flags(np, i, &gpio_flags);
		if (!gpio_is_valid(oms->gpios[i]))
			continue;

		ret = gpio_request(oms->gpios[i], dev->bus_id);
		if (ret < 0) {
			oms->gpios[i] = -EINVAL;
			continue;
		}

		if (gpio_flags & OF_GPIO_ACTIVE_LOW)
			oms->alow_gpios[i] = true;
	}

	if (gpio_is_valid(oms->gpios[CD_GPIO]))
		oms->pdata.get_cd = of_mmc_spi_get_cd;
	if (gpio_is_valid(oms->gpios[WP_GPIO]))
		oms->pdata.get_ro = of_mmc_spi_get_ro;

	/* We don't support interrupts yet, let's poll. */
	oms->pdata.caps |= MMC_CAP_NEEDS_POLL;

	dev->platform_data = &oms->pdata;
	return dev->platform_data;
err_ocr:
	kfree(oms);
	return NULL;
}
EXPORT_SYMBOL(mmc_spi_get_pdata);

void mmc_spi_put_pdata(struct spi_device *spi)
{
	struct device *dev = &spi->dev;
	struct device_node *np = dev_archdata_get_node(&dev->archdata);
	struct of_mmc_spi *oms = to_of_mmc_spi(dev);
	int i;

	if (!dev->platform_data || !np)
		return;

	for (i = 0; i < ARRAY_SIZE(oms->gpios); i++) {
		if (gpio_is_valid(oms->gpios[i]))
			gpio_free(oms->gpios[i]);
	}
	kfree(oms);
	dev->platform_data = NULL;
}
EXPORT_SYMBOL(mmc_spi_put_pdata);
