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
 * SPI OF support routines
 * Copyright (C) 2008 Secret Lab Technologies Ltd.
 *
 * Support routines for deriving SPI device attachments from the device
 * tree.
 */

#include <linux/of.h>
#include <linux/device.h>
#include <linux/spi/spi.h>
#include <linux/of_spi.h>

/**
 * of_register_spi_devices - Register child devices onto the SPI bus
 * @master:	Pointer to spi_master device
 * @np:		parent node of SPI device nodes
 *
 * Registers an spi_device for each child node of 'np' which has a 'reg'
 * property.
 */
void of_register_spi_devices(struct spi_master *master, struct device_node *np)
{
	struct spi_device *spi;
	struct device_node *nc;
	const u32 *prop;
	int rc;
	int len;

	for_each_child_of_node(np, nc) {
		/* Alloc an spi_device */
		spi = spi_alloc_device(master);
		if (!spi) {
			dev_err(&master->dev, "spi_device alloc error for %s\n",
				nc->full_name);
			spi_dev_put(spi);
			continue;
		}

		/* Select device driver */
		if (of_modalias_node(nc, spi->modalias,
				     sizeof(spi->modalias)) < 0) {
			dev_err(&master->dev, "cannot find modalias for %s\n",
				nc->full_name);
			spi_dev_put(spi);
			continue;
		}

		/* Device address */
		prop = of_get_property(nc, "reg", &len);
		if (!prop || len < sizeof(*prop)) {
			dev_err(&master->dev, "%s has no 'reg' property\n",
				nc->full_name);
			spi_dev_put(spi);
			continue;
		}
		spi->chip_select = *prop;

		/* Mode (clock phase/polarity/etc.) */
		if (of_find_property(nc, "spi-cpha", NULL))
			spi->mode |= SPI_CPHA;
		if (of_find_property(nc, "spi-cpol", NULL))
			spi->mode |= SPI_CPOL;
		if (of_find_property(nc, "spi-cs-high", NULL))
			spi->mode |= SPI_CS_HIGH;

		/* Device speed */
		prop = of_get_property(nc, "spi-max-frequency", &len);
		if (!prop || len < sizeof(*prop)) {
			dev_err(&master->dev, "%s has no 'spi-max-frequency' property\n",
				nc->full_name);
			spi_dev_put(spi);
			continue;
		}
		spi->max_speed_hz = *prop;

		/* IRQ */
		spi->irq = irq_of_parse_and_map(nc, 0);

		/* Store a pointer to the node in the device structure */
		of_node_get(nc);
		spi->dev.archdata.of_node = nc;

		/* Register the new device */
		request_module(spi->modalias);
		rc = spi_add_device(spi);
		if (rc) {
			dev_err(&master->dev, "spi_device register error %s\n",
				nc->full_name);
			spi_dev_put(spi);
		}

	}
}
EXPORT_SYMBOL(of_register_spi_devices);
