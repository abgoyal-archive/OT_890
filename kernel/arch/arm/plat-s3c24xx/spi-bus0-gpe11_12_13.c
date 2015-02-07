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

/* linux/arch/arm/plat-s3c24xx/spi-bus0-gpe11_12_13.c
 *
 * Copyright (c) 2008 Simtec Electronics
 *	http://armlinux.simtec.co.uk/
 *	Ben Dooks <ben@simtec.co.uk>
 *
 * S3C24XX SPI - gpio configuration for bus 0 on gpe11,12,13
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
*/

#include <linux/kernel.h>

#include <mach/hardware.h>

#include <mach/spi.h>
#include <mach/regs-gpio.h>

void s3c24xx_spi_gpiocfg_bus0_gpe11_12_13(struct s3c2410_spi_info *spi,
					  int enable)
{
	if (enable) {
		s3c2410_gpio_cfgpin(S3C2410_GPE13, S3C2410_GPE13_SPICLK0);
		s3c2410_gpio_cfgpin(S3C2410_GPE12, S3C2410_GPE12_SPIMOSI0);
		s3c2410_gpio_cfgpin(S3C2410_GPE11, S3C2410_GPE11_SPIMISO0);
		s3c2410_gpio_pullup(S3C2410_GPE11, 0);
		s3c2410_gpio_pullup(S3C2410_GPE13, 0);
	} else {
		s3c2410_gpio_cfgpin(S3C2410_GPE13, S3C2410_GPIO_INPUT);
		s3c2410_gpio_cfgpin(S3C2410_GPE11, S3C2410_GPIO_INPUT);
		s3c2410_gpio_pullup(S3C2410_GPE11, 1);
		s3c2410_gpio_pullup(S3C2410_GPE12, 1);
		s3c2410_gpio_pullup(S3C2410_GPE13, 1);
	}
}
