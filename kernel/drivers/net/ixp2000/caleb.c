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
 * Helper functions for the SPI-3 bridge FPGA on the Radisys ENP2611
 * Copyright (C) 2004, 2005 Lennert Buytenhek <buytenh@wantstofly.org>
 * Dedicated to Marija Kulikova.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <linux/module.h>
#include <linux/delay.h>
#include <asm/io.h>
#include "caleb.h"

#define CALEB_IDLO		0x00
#define CALEB_IDHI		0x01
#define CALEB_RID		0x02
#define CALEB_RESET		0x03
#define CALEB_INTREN0		0x04
#define CALEB_INTREN1		0x05
#define CALEB_INTRSTAT0		0x06
#define CALEB_INTRSTAT1		0x07
#define CALEB_PORTEN		0x08
#define CALEB_BURST		0x09
#define CALEB_PORTPAUS		0x0A
#define CALEB_PORTPAUSD		0x0B
#define CALEB_PHY0RX		0x10
#define CALEB_PHY1RX		0x11
#define CALEB_PHY0TX		0x12
#define CALEB_PHY1TX		0x13
#define CALEB_IXPRX_HI_CNTR	0x15
#define CALEB_PHY0RX_HI_CNTR	0x16
#define CALEB_PHY1RX_HI_CNTR	0x17
#define CALEB_IXPRX_CNTR	0x18
#define CALEB_PHY0RX_CNTR	0x19
#define CALEB_PHY1RX_CNTR	0x1A
#define CALEB_IXPTX_CNTR	0x1B
#define CALEB_PHY0TX_CNTR	0x1C
#define CALEB_PHY1TX_CNTR	0x1D
#define CALEB_DEBUG0		0x1E
#define CALEB_DEBUG1		0x1F


static u8 caleb_reg_read(int reg)
{
	u8 value;

	value = *((volatile u8 *)(ENP2611_CALEB_VIRT_BASE + reg));

//	printk(KERN_INFO "caleb_reg_read(%d) = %.2x\n", reg, value);

	return value;
}

static void caleb_reg_write(int reg, u8 value)
{
	u8 dummy;

//	printk(KERN_INFO "caleb_reg_write(%d, %.2x)\n", reg, value);

	*((volatile u8 *)(ENP2611_CALEB_VIRT_BASE + reg)) = value;

	dummy = *((volatile u8 *)ENP2611_CALEB_VIRT_BASE);
	__asm__ __volatile__("mov %0, %0" : "+r" (dummy));
}


void caleb_reset(void)
{
	/*
	 * Perform a chip reset.
	 */
	caleb_reg_write(CALEB_RESET, 0x02);
	udelay(1);

	/*
	 * Enable all interrupt sources.  This is needed to get
	 * meaningful results out of the status bits (register 6
	 * and 7.)
	 */
	caleb_reg_write(CALEB_INTREN0, 0xff);
	caleb_reg_write(CALEB_INTREN1, 0x07);

	/*
	 * Set RX and TX FIFO thresholds to 1.5kb.
	 */
	caleb_reg_write(CALEB_PHY0RX, 0x11);
	caleb_reg_write(CALEB_PHY1RX, 0x11);
	caleb_reg_write(CALEB_PHY0TX, 0x11);
	caleb_reg_write(CALEB_PHY1TX, 0x11);

	/*
	 * Program SPI-3 burst size.
	 */
	caleb_reg_write(CALEB_BURST, 0);	// 64-byte RBUF mpackets
//	caleb_reg_write(CALEB_BURST, 1);	// 128-byte RBUF mpackets
//	caleb_reg_write(CALEB_BURST, 2);	// 256-byte RBUF mpackets
}

void caleb_enable_rx(int port)
{
	u8 temp;

	temp = caleb_reg_read(CALEB_PORTEN);
	temp |= 1 << port;
	caleb_reg_write(CALEB_PORTEN, temp);
}

void caleb_disable_rx(int port)
{
	u8 temp;

	temp = caleb_reg_read(CALEB_PORTEN);
	temp &= ~(1 << port);
	caleb_reg_write(CALEB_PORTEN, temp);
}

void caleb_enable_tx(int port)
{
	u8 temp;

	temp = caleb_reg_read(CALEB_PORTEN);
	temp |= 1 << (port + 4);
	caleb_reg_write(CALEB_PORTEN, temp);
}

void caleb_disable_tx(int port)
{
	u8 temp;

	temp = caleb_reg_read(CALEB_PORTEN);
	temp &= ~(1 << (port + 4));
	caleb_reg_write(CALEB_PORTEN, temp);
}
