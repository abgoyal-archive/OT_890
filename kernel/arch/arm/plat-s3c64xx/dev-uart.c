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

/* linux/arch/arm/plat-s3c64xx/dev-uart.c
 *
 * Copyright 2008 Openmoko, Inc.
 * Copyright 2008 Simtec Electronics
 *	Ben Dooks <ben@simtec.co.uk>
 *	http://armlinux.simtec.co.uk/
 *
 * Base S3C64XX UART resource and device definitions
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
*/

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/interrupt.h>
#include <linux/list.h>
#include <linux/platform_device.h>

#include <asm/mach/arch.h>
#include <asm/mach/irq.h>
#include <mach/hardware.h>
#include <mach/map.h>

#include <plat/devs.h>

/* Serial port registrations */

/* 64xx uarts are closer together */

static struct resource s3c64xx_uart0_resource[] = {
	[0] = {
		.start	= S3C_PA_UART0,
		.end	= S3C_PA_UART0 + 0x100,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= IRQ_S3CUART_RX0,
		.end	= IRQ_S3CUART_RX0,
		.flags	= IORESOURCE_IRQ,
	},
	[2] = {
		.start	= IRQ_S3CUART_TX0,
		.end	= IRQ_S3CUART_TX0,
		.flags	= IORESOURCE_IRQ,

	},
	[3] = {
		.start	= IRQ_S3CUART_ERR0,
		.end	= IRQ_S3CUART_ERR0,
		.flags	= IORESOURCE_IRQ,
	}
};

static struct resource s3c64xx_uart1_resource[] = {
	[0] = {
		.start = S3C_PA_UART1,
		.end   = S3C_PA_UART1 + 0x100,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start	= IRQ_S3CUART_RX1,
		.end	= IRQ_S3CUART_RX1,
		.flags	= IORESOURCE_IRQ,
	},
	[2] = {
		.start	= IRQ_S3CUART_TX1,
		.end	= IRQ_S3CUART_TX1,
		.flags	= IORESOURCE_IRQ,

	},
	[3] = {
		.start	= IRQ_S3CUART_ERR1,
		.end	= IRQ_S3CUART_ERR1,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct resource s3c6xx_uart2_resource[] = {
	[0] = {
		.start = S3C_PA_UART2,
		.end   = S3C_PA_UART2 + 0x100,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start	= IRQ_S3CUART_RX2,
		.end	= IRQ_S3CUART_RX2,
		.flags	= IORESOURCE_IRQ,
	},
	[2] = {
		.start	= IRQ_S3CUART_TX2,
		.end	= IRQ_S3CUART_TX2,
		.flags	= IORESOURCE_IRQ,

	},
	[3] = {
		.start	= IRQ_S3CUART_ERR2,
		.end	= IRQ_S3CUART_ERR2,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct resource s3c64xx_uart3_resource[] = {
	[0] = {
		.start = S3C_PA_UART3,
		.end   = S3C_PA_UART3 + 0x100,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start	= IRQ_S3CUART_RX3,
		.end	= IRQ_S3CUART_RX3,
		.flags	= IORESOURCE_IRQ,
	},
	[2] = {
		.start	= IRQ_S3CUART_TX3,
		.end	= IRQ_S3CUART_TX3,
		.flags	= IORESOURCE_IRQ,

	},
	[3] = {
		.start	= IRQ_S3CUART_ERR3,
		.end	= IRQ_S3CUART_ERR3,
		.flags	= IORESOURCE_IRQ,
	},
};


struct s3c24xx_uart_resources s3c64xx_uart_resources[] __initdata = {
	[0] = {
		.resources	= s3c64xx_uart0_resource,
		.nr_resources	= ARRAY_SIZE(s3c64xx_uart0_resource),
	},
	[1] = {
		.resources	= s3c64xx_uart1_resource,
		.nr_resources	= ARRAY_SIZE(s3c64xx_uart1_resource),
	},
	[2] = {
		.resources	= s3c6xx_uart2_resource,
		.nr_resources	= ARRAY_SIZE(s3c6xx_uart2_resource),
	},
	[3] = {
		.resources	= s3c64xx_uart3_resource,
		.nr_resources	= ARRAY_SIZE(s3c64xx_uart3_resource),
	},
};

/* uart devices */

static struct platform_device s3c24xx_uart_device0 = {
	.id		= 0,
};

static struct platform_device s3c24xx_uart_device1 = {
	.id		= 1,
};

static struct platform_device s3c24xx_uart_device2 = {
	.id		= 2,
};

static struct platform_device s3c24xx_uart_device3 = {
	.id		= 3,
};

struct platform_device *s3c24xx_uart_src[4] = {
	&s3c24xx_uart_device0,
	&s3c24xx_uart_device1,
	&s3c24xx_uart_device2,
	&s3c24xx_uart_device3,
};

struct platform_device *s3c24xx_uart_devs[4] = {
};

