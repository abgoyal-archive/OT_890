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
 * gpio.h -- GPIO driver for NXP PCF50633
 *
 * (C) 2006-2008 by Openmoko, Inc.
 * All rights reserved.
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */

#ifndef __LINUX_MFD_PCF50633_GPIO_H
#define __LINUX_MFD_PCF50633_GPIO_H

#include <linux/mfd/pcf50633/core.h>

#define PCF50633_GPIO1		1
#define PCF50633_GPIO2		2
#define PCF50633_GPIO3		3
#define PCF50633_GPO		4

#define PCF50633_REG_GPIO1CFG	0x14
#define PCF50633_REG_GPIO2CFG	0x15
#define PCF50633_REG_GPIO3CFG	0x16
#define PCF50633_REG_GPOCFG 	0x17

#define PCF50633_GPOCFG_GPOSEL_MASK	0x07

enum pcf50633_reg_gpocfg {
	PCF50633_GPOCFG_GPOSEL_0	= 0x00,
	PCF50633_GPOCFG_GPOSEL_LED_NFET	= 0x01,
	PCF50633_GPOCFG_GPOSEL_SYSxOK	= 0x02,
	PCF50633_GPOCFG_GPOSEL_CLK32K	= 0x03,
	PCF50633_GPOCFG_GPOSEL_ADAPUSB	= 0x04,
	PCF50633_GPOCFG_GPOSEL_USBxOK	= 0x05,
	PCF50633_GPOCFG_GPOSEL_ACTPH4	= 0x06,
	PCF50633_GPOCFG_GPOSEL_1	= 0x07,
	PCF50633_GPOCFG_GPOSEL_INVERSE	= 0x08,
};

int pcf50633_gpio_set(struct pcf50633 *pcf, int gpio, u8 val);
u8 pcf50633_gpio_get(struct pcf50633 *pcf, int gpio);

int pcf50633_gpio_invert_set(struct pcf50633 *, int gpio, int invert);
int pcf50633_gpio_invert_get(struct pcf50633 *pcf, int gpio);

int pcf50633_gpio_power_supply_set(struct pcf50633 *,
					int gpio, int regulator, int on);
#endif /* __LINUX_MFD_PCF50633_GPIO_H */


