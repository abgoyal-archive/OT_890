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
 *  linux/include/linux/serial_8250.h
 *
 *  Copyright (C) 2004 Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#ifndef _LINUX_SERIAL_8250_H
#define _LINUX_SERIAL_8250_H

#include <linux/serial_core.h>
#include <linux/platform_device.h>

/*
 * This is the platform device platform_data structure
 */
struct plat_serial8250_port {
	unsigned long	iobase;		/* io base address */
	void __iomem	*membase;	/* ioremap cookie or NULL */
	resource_size_t	mapbase;	/* resource base */
	unsigned int	irq;		/* interrupt number */
	unsigned int	uartclk;	/* UART clock rate */
	void            *private_data;
	unsigned char	regshift;	/* register shift */
	unsigned char	iotype;		/* UPIO_* */
	unsigned char	hub6;
	upf_t		flags;		/* UPF_* flags */
	unsigned int	type;		/* If UPF_FIXED_TYPE */
	unsigned int	(*serial_in)(struct uart_port *, int);
	void		(*serial_out)(struct uart_port *, int, int);
};

/*
 * Allocate 8250 platform device IDs.  Nothing is implied by
 * the numbering here, except for the legacy entry being -1.
 */
enum {
	PLAT8250_DEV_LEGACY = -1,
	PLAT8250_DEV_PLATFORM,
	PLAT8250_DEV_PLATFORM1,
	PLAT8250_DEV_PLATFORM2,
	PLAT8250_DEV_FOURPORT,
	PLAT8250_DEV_ACCENT,
	PLAT8250_DEV_BOCA,
	PLAT8250_DEV_EXAR_ST16C554,
	PLAT8250_DEV_HUB6,
	PLAT8250_DEV_MCA,
	PLAT8250_DEV_AU1X00,
	PLAT8250_DEV_SM501,
};

/*
 * This should be used by drivers which want to register
 * their own 8250 ports without registering their own
 * platform device.  Using these will make your driver
 * dependent on the 8250 driver.
 */
struct uart_port;

int serial8250_register_port(struct uart_port *);
void serial8250_unregister_port(int line);
void serial8250_suspend_port(int line);
void serial8250_resume_port(int line);

extern int early_serial_setup(struct uart_port *port);

extern int serial8250_find_port(struct uart_port *p);
extern int serial8250_find_port_for_earlycon(void);
extern int setup_early_serial8250_console(char *cmdline);

#endif
