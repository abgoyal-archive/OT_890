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

#ifndef __LINUX_SPI_GPIO_H
#define __LINUX_SPI_GPIO_H

/*
 * For each bitbanged SPI bus, set up a platform_device node with:
 *   - name "spi_gpio"
 *   - id the same as the SPI bus number it implements
 *   - dev.platform data pointing to a struct spi_gpio_platform_data
 *
 * Or, see the driver code for information about speedups that are
 * possible on platforms that support inlined access for GPIOs (no
 * spi_gpio_platform_data is used).
 *
 * Use spi_board_info with these busses in the usual way, being sure
 * that the controller_data being the GPIO used for each device's
 * chipselect:
 *
 *	static struct spi_board_info ... [] = {
 *	...
 *		// this slave uses GPIO 42 for its chipselect
 *		.controller_data = (void *) 42,
 *	...
 *		// this one uses GPIO 86 for its chipselect
 *		.controller_data = (void *) 86,
 *	...
 *	};
 *
 * If the bitbanged bus is later switched to a "native" controller,
 * that platform_device and controller_data should be removed.
 */

/**
 * struct spi_gpio_platform_data - parameter for bitbanged SPI master
 * @sck: number of the GPIO used for clock output
 * @mosi: number of the GPIO used for Master Output, Slave In (MOSI) data
 * @miso: number of the GPIO used for Master Input, Slave Output (MISO) data
 * @num_chipselect: how many slaves to allow
 *
 * All GPIO signals used with the SPI bus managed through this driver
 * (chipselects, MOSI, MISO, SCK) must be configured as GPIOs, instead
 * of some alternate function.
 *
 * It can be convenient to use this driver with pins that have alternate
 * functions associated with a "native" SPI controller if a driver for that
 * controller is not available, or is missing important functionality.
 *
 * On platforms which can do so, configure MISO with a weak pullup unless
 * there's an external pullup on that signal.  That saves power by avoiding
 * floating signals.  (A weak pulldown would save power too, but many
 * drivers expect to see all-ones data as the no slave "response".)
 */
struct spi_gpio_platform_data {
	unsigned	sck;
	unsigned	mosi;
	unsigned	miso;

	u16		num_chipselect;
};

#endif /* __LINUX_SPI_GPIO_H */
