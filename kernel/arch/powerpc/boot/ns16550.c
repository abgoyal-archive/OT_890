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
 * 16550 serial console support.
 *
 * Original copied from <file:arch/ppc/boot/common/ns16550.c>
 * (which had no copyright)
 * Modifications: 2006 (c) MontaVista Software, Inc.
 *
 * Modified by: Mark A. Greer <mgreer@mvista.com>
 */
#include <stdarg.h>
#include <stddef.h>
#include "types.h"
#include "string.h"
#include "stdio.h"
#include "io.h"
#include "ops.h"

#define UART_DLL	0	/* Out: Divisor Latch Low */
#define UART_DLM	1	/* Out: Divisor Latch High */
#define UART_FCR	2	/* Out: FIFO Control Register */
#define UART_LCR	3	/* Out: Line Control Register */
#define UART_MCR	4	/* Out: Modem Control Register */
#define UART_LSR	5	/* In:  Line Status Register */
#define UART_LSR_THRE	0x20	/* Transmit-hold-register empty */
#define UART_LSR_DR	0x01	/* Receiver data ready */
#define UART_MSR	6	/* In:  Modem Status Register */
#define UART_SCR	7	/* I/O: Scratch Register */

static unsigned char *reg_base;
static u32 reg_shift;

static int ns16550_open(void)
{
	out_8(reg_base + (UART_FCR << reg_shift), 0x06);
	return 0;
}

static void ns16550_putc(unsigned char c)
{
	while ((in_8(reg_base + (UART_LSR << reg_shift)) & UART_LSR_THRE) == 0);
	out_8(reg_base, c);
}

static unsigned char ns16550_getc(void)
{
	while ((in_8(reg_base + (UART_LSR << reg_shift)) & UART_LSR_DR) == 0);
	return in_8(reg_base);
}

static u8 ns16550_tstc(void)
{
	return ((in_8(reg_base + (UART_LSR << reg_shift)) & UART_LSR_DR) != 0);
}

int ns16550_console_init(void *devp, struct serial_console_data *scdp)
{
	int n;
	u32 reg_offset;

	if (dt_get_virtual_reg(devp, (void **)&reg_base, 1) < 1)
		return -1;

	n = getprop(devp, "reg-offset", &reg_offset, sizeof(reg_offset));
	if (n == sizeof(reg_offset))
		reg_base += reg_offset;

	n = getprop(devp, "reg-shift", &reg_shift, sizeof(reg_shift));
	if (n != sizeof(reg_shift))
		reg_shift = 0;

	scdp->open = ns16550_open;
	scdp->putc = ns16550_putc;
	scdp->getc = ns16550_getc;
	scdp->tstc = ns16550_tstc;
	scdp->close = NULL;

	return 0;
}
