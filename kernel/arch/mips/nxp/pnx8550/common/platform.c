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
 * Platform device support for NXP PNX8550 SoCs
 *
 * Copyright 2005, Embedded Alley Solutions, Inc
 *
 * Based on arch/mips/au1000/common/platform.c
 * Platform device support for Au1x00 SoCs.
 *
 * Copyright 2004, Matt Porter <mporter@kernel.crashing.org>
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2.  This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */
#include <linux/device.h>
#include <linux/dma-mapping.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/resource.h>
#include <linux/serial.h>
#include <linux/serial_pnx8xxx.h>
#include <linux/platform_device.h>

#include <int.h>
#include <usb.h>
#include <uart.h>

static struct resource pnx8550_usb_ohci_resources[] = {
	[0] = {
		.start		= PNX8550_USB_OHCI_OP_BASE,
		.end		= PNX8550_USB_OHCI_OP_BASE +
				  PNX8550_USB_OHCI_OP_LEN,
		.flags		= IORESOURCE_MEM,
	},
	[1] = {
		.start		= PNX8550_INT_USB,
		.end		= PNX8550_INT_USB,
		.flags		= IORESOURCE_IRQ,
	},
};

static struct resource pnx8550_uart_resources[] = {
	[0] = {
		.start		= PNX8550_UART_PORT0,
		.end		= PNX8550_UART_PORT0 + 0xfff,
		.flags		= IORESOURCE_MEM,
	},
	[1] = {
		.start		= PNX8550_UART_INT(0),
		.end		= PNX8550_UART_INT(0),
		.flags		= IORESOURCE_IRQ,
	},
	[2] = {
		.start		= PNX8550_UART_PORT1,
		.end		= PNX8550_UART_PORT1 + 0xfff,
		.flags		= IORESOURCE_MEM,
	},
	[3] = {
		.start		= PNX8550_UART_INT(1),
		.end		= PNX8550_UART_INT(1),
		.flags		= IORESOURCE_IRQ,
	},
};

struct pnx8xxx_port pnx8xxx_ports[] = {
	[0] = {
		.port   = {
			.type		= PORT_PNX8XXX,
			.iotype		= UPIO_MEM,
			.membase	= (void __iomem *)PNX8550_UART_PORT0,
			.mapbase	= PNX8550_UART_PORT0,
			.irq		= PNX8550_UART_INT(0),
			.uartclk	= 3692300,
			.fifosize	= 16,
			.flags		= UPF_BOOT_AUTOCONF,
			.line		= 0,
		},
	},
	[1] = {
		.port   = {
			.type		= PORT_PNX8XXX,
			.iotype		= UPIO_MEM,
			.membase	= (void __iomem *)PNX8550_UART_PORT1,
			.mapbase	= PNX8550_UART_PORT1,
			.irq		= PNX8550_UART_INT(1),
			.uartclk	= 3692300,
			.fifosize	= 16,
			.flags		= UPF_BOOT_AUTOCONF,
			.line		= 1,
		},
	},
};

/* The dmamask must be set for OHCI to work */
static u64 ohci_dmamask = DMA_32BIT_MASK;

static u64 uart_dmamask = DMA_32BIT_MASK;

static struct platform_device pnx8550_usb_ohci_device = {
	.name		= "pnx8550-ohci",
	.id		= -1,
	.dev = {
		.dma_mask		= &ohci_dmamask,
		.coherent_dma_mask	= DMA_32BIT_MASK,
	},
	.num_resources	= ARRAY_SIZE(pnx8550_usb_ohci_resources),
	.resource	= pnx8550_usb_ohci_resources,
};

static struct platform_device pnx8550_uart_device = {
	.name		= "pnx8xxx-uart",
	.id		= -1,
	.dev = {
		.dma_mask		= &uart_dmamask,
		.coherent_dma_mask	= DMA_32BIT_MASK,
		.platform_data = pnx8xxx_ports,
	},
	.num_resources	= ARRAY_SIZE(pnx8550_uart_resources),
	.resource	= pnx8550_uart_resources,
};

static struct platform_device *pnx8550_platform_devices[] __initdata = {
	&pnx8550_usb_ohci_device,
	&pnx8550_uart_device,
};

static int __init pnx8550_platform_init(void)
{
	return platform_add_devices(pnx8550_platform_devices,
			            ARRAY_SIZE(pnx8550_platform_devices));
}

arch_initcall(pnx8550_platform_init);
