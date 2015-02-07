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
 * arch/arm/mach-omap2/serial.c
 *
 * OMAP2 serial support.
 *
 * Copyright (C) 2005-2008 Nokia Corporation
 * Author: Paul Mundt <paul.mundt@nokia.com>
 *
 * Based off of arch/arm/mach-omap/omap1/serial.c
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License. See the file "COPYING" in the main directory of this archive
 * for more details.
 */
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/serial_8250.h>
#include <linux/serial_reg.h>
#include <linux/clk.h>
#include <linux/io.h>

#include <mach/common.h>
#include <mach/board.h>

static struct clk *uart_ick[OMAP_MAX_NR_PORTS];
static struct clk *uart_fck[OMAP_MAX_NR_PORTS];

static struct plat_serial8250_port serial_platform_data[] = {
	{
		.membase	= IO_ADDRESS(OMAP_UART1_BASE),
		.mapbase	= OMAP_UART1_BASE,
		.irq		= 72,
		.flags		= UPF_BOOT_AUTOCONF,
		.iotype		= UPIO_MEM,
		.regshift	= 2,
		.uartclk	= OMAP24XX_BASE_BAUD * 16,
	}, {
		.membase	= IO_ADDRESS(OMAP_UART2_BASE),
		.mapbase	= OMAP_UART2_BASE,
		.irq		= 73,
		.flags		= UPF_BOOT_AUTOCONF,
		.iotype		= UPIO_MEM,
		.regshift	= 2,
		.uartclk	= OMAP24XX_BASE_BAUD * 16,
	}, {
		.membase	= IO_ADDRESS(OMAP_UART3_BASE),
		.mapbase	= OMAP_UART3_BASE,
		.irq		= 74,
		.flags		= UPF_BOOT_AUTOCONF,
		.iotype		= UPIO_MEM,
		.regshift	= 2,
		.uartclk	= OMAP24XX_BASE_BAUD * 16,
	}, {
		.flags		= 0
	}
};

static inline unsigned int serial_read_reg(struct plat_serial8250_port *up,
					   int offset)
{
	offset <<= up->regshift;
	return (unsigned int)__raw_readb(up->membase + offset);
}

static inline void serial_write_reg(struct plat_serial8250_port *p, int offset,
				    int value)
{
	offset <<= p->regshift;
	__raw_writeb(value, p->membase + offset);
}

/*
 * Internal UARTs need to be initialized for the 8250 autoconfig to work
 * properly. Note that the TX watermark initialization may not be needed
 * once the 8250.c watermark handling code is merged.
 */
static inline void __init omap_serial_reset(struct plat_serial8250_port *p)
{
	serial_write_reg(p, UART_OMAP_MDR1, 0x07);
	serial_write_reg(p, UART_OMAP_SCR, 0x08);
	serial_write_reg(p, UART_OMAP_MDR1, 0x00);
	serial_write_reg(p, UART_OMAP_SYSC, (0x02 << 3) | (1 << 2) | (1 << 0));
}

void omap_serial_enable_clocks(int enable)
{
	int i;
	for (i = 0; i < OMAP_MAX_NR_PORTS; i++) {
		if (uart_ick[i] && uart_fck[i]) {
			if (enable) {
				clk_enable(uart_ick[i]);
				clk_enable(uart_fck[i]);
			} else {
				clk_disable(uart_ick[i]);
				clk_disable(uart_fck[i]);
			}
		}
	}
}

void __init omap_serial_init(void)
{
	int i;
	const struct omap_uart_config *info;
	char name[16];

	/*
	 * Make sure the serial ports are muxed on at this point.
	 * You have to mux them off in device drivers later on
	 * if not needed.
	 */

	info = omap_get_config(OMAP_TAG_UART, struct omap_uart_config);

	if (info == NULL)
		return;

	for (i = 0; i < OMAP_MAX_NR_PORTS; i++) {
		struct plat_serial8250_port *p = serial_platform_data + i;

		if (!(info->enabled_uarts & (1 << i))) {
			p->membase = NULL;
			p->mapbase = 0;
			continue;
		}

		sprintf(name, "uart%d_ick", i+1);
		uart_ick[i] = clk_get(NULL, name);
		if (IS_ERR(uart_ick[i])) {
			printk(KERN_ERR "Could not get uart%d_ick\n", i+1);
			uart_ick[i] = NULL;
		} else
			clk_enable(uart_ick[i]);

		sprintf(name, "uart%d_fck", i+1);
		uart_fck[i] = clk_get(NULL, name);
		if (IS_ERR(uart_fck[i])) {
			printk(KERN_ERR "Could not get uart%d_fck\n", i+1);
			uart_fck[i] = NULL;
		} else
			clk_enable(uart_fck[i]);

		omap_serial_reset(p);
	}
}

static struct platform_device serial_device = {
	.name			= "serial8250",
	.id			= PLAT8250_DEV_PLATFORM,
	.dev			= {
		.platform_data	= serial_platform_data,
	},
};

static int __init omap_init(void)
{
	return platform_device_register(&serial_device);
}
arch_initcall(omap_init);
