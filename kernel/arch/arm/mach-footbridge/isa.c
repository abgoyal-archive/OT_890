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
 *  linux/arch/arm/mach-footbridge/isa.c
 *
 *  Copyright (C) 2004 Russell King.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/init.h>
#include <linux/serial_8250.h>

#include <asm/irq.h>

static struct resource rtc_resources[] = {
	[0] = {
		.start	= 0x70,
		.end	= 0x73,
		.flags	= IORESOURCE_IO,
	},
	[1] = {
		.start	= IRQ_ISA_RTC_ALARM,
		.end	= IRQ_ISA_RTC_ALARM,
		.flags	= IORESOURCE_IRQ,
	}
};

static struct platform_device rtc_device = {
	.name		= "rtc_cmos",
	.id		= -1,
	.resource	= rtc_resources,
	.num_resources	= ARRAY_SIZE(rtc_resources),
};

static struct resource serial_resources[] = {
	[0] = {
		.start	= 0x3f8,
		.end	= 0x3ff,
		.flags	= IORESOURCE_IO,
	},
	[1] = {
		.start	= 0x2f8,
		.end	= 0x2ff,
		.flags	= IORESOURCE_IO,
	},
};

static struct plat_serial8250_port serial_platform_data[] = {
	{
		.iobase		= 0x3f8,
		.irq		= IRQ_ISA_UART,
		.uartclk	= 1843200,
		.regshift	= 0,
		.iotype		= UPIO_PORT,
		.flags		= UPF_BOOT_AUTOCONF | UPF_SKIP_TEST,
	},
	{
		.iobase		= 0x2f8,
		.irq		= IRQ_ISA_UART2,
		.uartclk	= 1843200,
		.regshift	= 0,
		.iotype		= UPIO_PORT,
		.flags		= UPF_BOOT_AUTOCONF | UPF_SKIP_TEST,
	},
	{ },
};

static struct platform_device serial_device = {
	.name			= "serial8250",
	.id			= PLAT8250_DEV_PLATFORM,
	.dev			= {
		.platform_data	= serial_platform_data,
	},
	.resource		= serial_resources,
	.num_resources		= ARRAY_SIZE(serial_resources),
};

static int __init footbridge_isa_init(void)
{
	int err;

	err = platform_device_register(&rtc_device);
	if (err)
		printk(KERN_ERR "Unable to register RTC device: %d\n", err);
	err = platform_device_register(&serial_device);
	if (err)
		printk(KERN_ERR "Unable to register serial device: %d\n", err);
	return 0;
}

arch_initcall(footbridge_isa_init);
