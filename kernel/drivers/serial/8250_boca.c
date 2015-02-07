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
 *  linux/drivers/serial/8250_boca.c
 *
 *  Copyright (C) 2005 Russell King.
 *  Data taken from include/asm-i386/serial.h
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/module.h>
#include <linux/init.h>
#include <linux/serial_8250.h>

#define PORT(_base,_irq)				\
	{						\
		.iobase		= _base,		\
		.irq		= _irq,			\
		.uartclk	= 1843200,		\
		.iotype		= UPIO_PORT,		\
		.flags		= UPF_BOOT_AUTOCONF,	\
	}

static struct plat_serial8250_port boca_data[] = {
	PORT(0x100, 12),
	PORT(0x108, 12),
	PORT(0x110, 12),
	PORT(0x118, 12),
	PORT(0x120, 12),
	PORT(0x128, 12),
	PORT(0x130, 12),
	PORT(0x138, 12),
	PORT(0x140, 12),
	PORT(0x148, 12),
	PORT(0x150, 12),
	PORT(0x158, 12),
	PORT(0x160, 12),
	PORT(0x168, 12),
	PORT(0x170, 12),
	PORT(0x178, 12),
	{ },
};

static struct platform_device boca_device = {
	.name			= "serial8250",
	.id			= PLAT8250_DEV_BOCA,
	.dev			= {
		.platform_data	= boca_data,
	},
};

static int __init boca_init(void)
{
	return platform_device_register(&boca_device);
}

module_init(boca_init);

MODULE_AUTHOR("Russell King");
MODULE_DESCRIPTION("8250 serial probe module for Boca cards");
MODULE_LICENSE("GPL");
