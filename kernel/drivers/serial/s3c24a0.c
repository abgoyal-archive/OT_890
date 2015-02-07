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

/* linux/drivers/serial/s3c24a0.c
 *
 * Driver for Samsung S3C24A0 SoC onboard UARTs.
 *
 * Based on drivers/serial/s3c2410.c
 *
 * Author: Sandeep Patil <sandeep.patil@azingo.com>
 *
 * Ben Dooks, Copyright (c) 2003-2005,2008 Simtec Electronics
 *	http://armlinux.simtec.co.uk/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#include <linux/module.h>
#include <linux/ioport.h>
#include <linux/platform_device.h>
#include <linux/init.h>
#include <linux/serial_core.h>
#include <linux/serial.h>
#include <linux/io.h>
#include <linux/irq.h>

#include <mach/hardware.h>

#include <plat/regs-serial.h>
#include <mach/regs-gpio.h>

#include "samsung.h"

static int s3c24a0_serial_setsource(struct uart_port *port,
				    struct s3c24xx_uart_clksrc *clk)
{
	unsigned long ucon = rd_regl(port, S3C2410_UCON);

	if (strcmp(clk->name, "uclk") == 0)
		ucon |= S3C2410_UCON_UCLK;
	else
		ucon &= ~S3C2410_UCON_UCLK;

	wr_regl(port, S3C2410_UCON, ucon);
	return 0;
}

static int s3c24a0_serial_getsource(struct uart_port *port,
				    struct s3c24xx_uart_clksrc *clk)
{
	unsigned long ucon = rd_regl(port, S3C2410_UCON);

	clk->divisor = 1;
	clk->name = (ucon & S3C2410_UCON_UCLK) ? "uclk" : "pclk";

	return 0;
}

static int s3c24a0_serial_resetport(struct uart_port *port,
				    struct s3c2410_uartcfg *cfg)
{
	dbg("s3c24a0_serial_resetport: port=%p (%08lx), cfg=%p\n",
	    port, port->mapbase, cfg);

	wr_regl(port, S3C2410_UCON,  cfg->ucon);
	wr_regl(port, S3C2410_ULCON, cfg->ulcon);

	/* reset both fifos */

	wr_regl(port, S3C2410_UFCON, cfg->ufcon | S3C2410_UFCON_RESETBOTH);
	wr_regl(port, S3C2410_UFCON, cfg->ufcon);

	return 0;
}

static struct s3c24xx_uart_info s3c24a0_uart_inf = {
	.name		= "Samsung S3C24A0 UART",
	.type		= PORT_S3C2410,
	.fifosize	= 16,
	.rx_fifomask	= S3C24A0_UFSTAT_RXMASK,
	.rx_fifoshift	= S3C24A0_UFSTAT_RXSHIFT,
	.rx_fifofull	= S3C24A0_UFSTAT_RXFULL,
	.tx_fifofull	= S3C24A0_UFSTAT_TXFULL,
	.tx_fifomask	= S3C24A0_UFSTAT_TXMASK,
	.tx_fifoshift	= S3C24A0_UFSTAT_TXSHIFT,
	.get_clksrc	= s3c24a0_serial_getsource,
	.set_clksrc	= s3c24a0_serial_setsource,
	.reset_port	= s3c24a0_serial_resetport,
};

static int s3c24a0_serial_probe(struct platform_device *dev)
{
	return s3c24xx_serial_probe(dev, &s3c24a0_uart_inf);
}

static struct platform_driver s3c24a0_serial_drv = {
	.probe		= s3c24a0_serial_probe,
	.remove		= s3c24xx_serial_remove,
	.driver		= {
		.name	= "s3c24a0-uart",
		.owner	= THIS_MODULE,
	},
};

s3c24xx_console_init(&s3c24a0_serial_drv, &s3c24a0_uart_inf);

static int __init s3c24a0_serial_init(void)
{
	return s3c24xx_serial_init(&s3c24a0_serial_drv, &s3c24a0_uart_inf);
}

static void __exit s3c24a0_serial_exit(void)
{
	platform_driver_unregister(&s3c24a0_serial_drv);
}

module_init(s3c24a0_serial_init);
module_exit(s3c24a0_serial_exit);

