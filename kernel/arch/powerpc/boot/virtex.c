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
 * The platform specific code for virtex devices since a boot loader is not
 * always used.
 *
 * (C) Copyright 2008 Xilinx, Inc.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
 */

#include "ops.h"
#include "io.h"
#include "stdio.h"

#define UART_DLL		0	/* Out: Divisor Latch Low */
#define UART_DLM		1	/* Out: Divisor Latch High */
#define UART_FCR		2	/* Out: FIFO Control Register */
#define UART_FCR_CLEAR_RCVR 	0x02 	/* Clear the RCVR FIFO */
#define UART_FCR_CLEAR_XMIT	0x04 	/* Clear the XMIT FIFO */
#define UART_LCR		3	/* Out: Line Control Register */
#define UART_MCR		4	/* Out: Modem Control Register */
#define UART_MCR_RTS		0x02 	/* RTS complement */
#define UART_MCR_DTR		0x01 	/* DTR complement */
#define UART_LCR_DLAB		0x80 	/* Divisor latch access bit */
#define UART_LCR_WLEN8		0x03 	/* Wordlength: 8 bits */

static int virtex_ns16550_console_init(void *devp)
{
	unsigned char *reg_base;
	u32 reg_shift, reg_offset, clk, spd;
	u16 divisor;
	int n;

	if (dt_get_virtual_reg(devp, (void **)&reg_base, 1) < 1)
		return -1;

	n = getprop(devp, "reg-offset", &reg_offset, sizeof(reg_offset));
	if (n == sizeof(reg_offset))
		reg_base += reg_offset;

	n = getprop(devp, "reg-shift", &reg_shift, sizeof(reg_shift));
	if (n != sizeof(reg_shift))
		reg_shift = 0;

	n = getprop(devp, "current-speed", (void *)&spd, sizeof(spd));
	if (n != sizeof(spd))
		spd = 9600;

	/* should there be a default clock rate?*/
	n = getprop(devp, "clock-frequency", (void *)&clk, sizeof(clk));
	if (n != sizeof(clk))
		return -1;

	divisor = clk / (16 * spd);

	/* Access baud rate */
	out_8(reg_base + (UART_LCR << reg_shift), UART_LCR_DLAB);

	/* Baud rate based on input clock */
	out_8(reg_base + (UART_DLL << reg_shift), divisor & 0xFF);
	out_8(reg_base + (UART_DLM << reg_shift), divisor >> 8);

	/* 8 data, 1 stop, no parity */
	out_8(reg_base + (UART_LCR << reg_shift), UART_LCR_WLEN8);

	/* RTS/DTR */
	out_8(reg_base + (UART_MCR << reg_shift), UART_MCR_RTS | UART_MCR_DTR);

	/* Clear transmitter and receiver */
	out_8(reg_base + (UART_FCR << reg_shift),
				UART_FCR_CLEAR_XMIT | UART_FCR_CLEAR_RCVR);
	return 0;
}

/* For virtex, the kernel may be loaded without using a bootloader and if so
   some UARTs need more setup than is provided in the normal console init
*/
int platform_specific_init(void)
{
	void *devp;
	char devtype[MAX_PROP_LEN];
	char path[MAX_PATH_LEN];

	devp = finddevice("/chosen");
	if (devp == NULL)
		return -1;

	if (getprop(devp, "linux,stdout-path", path, MAX_PATH_LEN) > 0) {
		devp = finddevice(path);
		if (devp == NULL)
			return -1;

		if ((getprop(devp, "device_type", devtype, sizeof(devtype)) > 0)
				&& !strcmp(devtype, "serial")
				&& (dt_is_compatible(devp, "ns16550")))
				virtex_ns16550_console_init(devp);
	}
	return 0;
}
