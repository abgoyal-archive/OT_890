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
 *  linux/drivers/acorn/char/i2c.c
 *
 *  Copyright (C) 2000 Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  ARM IOC/IOMD i2c driver.
 *
 *  On Acorn machines, the following i2c devices are on the bus:
 *	- PCF8583 real time clock & static RAM
 */
#include <linux/init.h>
#include <linux/i2c.h>
#include <linux/i2c-algo-bit.h>

#include <mach/hardware.h>
#include <asm/io.h>
#include <asm/hardware/ioc.h>
#include <asm/system.h>

#define FORCE_ONES	0xdc
#define SCL		0x02
#define SDA		0x01

/*
 * We must preserve all non-i2c output bits in IOC_CONTROL.
 * Note also that we need to preserve the value of SCL and
 * SDA outputs as well (which may be different from the
 * values read back from IOC_CONTROL).
 */
static u_int force_ones;

static void ioc_setscl(void *data, int state)
{
	u_int ioc_control = ioc_readb(IOC_CONTROL) & ~(SCL | SDA);
	u_int ones = force_ones;

	if (state)
		ones |= SCL;
	else
		ones &= ~SCL;

	force_ones = ones;

 	ioc_writeb(ioc_control | ones, IOC_CONTROL);
}

static void ioc_setsda(void *data, int state)
{
	u_int ioc_control = ioc_readb(IOC_CONTROL) & ~(SCL | SDA);
	u_int ones = force_ones;

	if (state)
		ones |= SDA;
	else
		ones &= ~SDA;

	force_ones = ones;

 	ioc_writeb(ioc_control | ones, IOC_CONTROL);
}

static int ioc_getscl(void *data)
{
	return (ioc_readb(IOC_CONTROL) & SCL) != 0;
}

static int ioc_getsda(void *data)
{
	return (ioc_readb(IOC_CONTROL) & SDA) != 0;
}

static struct i2c_algo_bit_data ioc_data = {
	.setsda		= ioc_setsda,
	.setscl		= ioc_setscl,
	.getsda		= ioc_getsda,
	.getscl		= ioc_getscl,
	.udelay		= 80,
	.timeout	= HZ,
};

static struct i2c_adapter ioc_ops = {
	.nr			= 0,
	.algo_data		= &ioc_data,
};

static int __init i2c_ioc_init(void)
{
	force_ones = FORCE_ONES | SCL | SDA;

	return i2c_bit_add_numbered_bus(&ioc_ops);
}

module_init(i2c_ioc_init);
