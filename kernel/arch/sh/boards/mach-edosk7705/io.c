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
 * arch/sh/boards/renesas/edosk7705/io.c
 *
 * Copyright (C) 2001  Ian da Silva, Jeremy Siegel
 * Based largely on io_se.c.
 *
 * I/O routines for Hitachi EDOSK7705 board.
 *
 */

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/io.h>
#include <mach/edosk7705.h>
#include <asm/addrspace.h>

#define SMC_IOADDR	0xA2000000

/* Map the Ethernet addresses as if it is at 0x300 - 0x320 */
static unsigned long sh_edosk7705_isa_port2addr(unsigned long port)
{
	/*
	 * SMC91C96 registers are 4 byte aligned rather than the
	 * usual 2 byte!
	 */
	if (port >= 0x300 && port < 0x320)
		return SMC_IOADDR + ((port - 0x300) * 2);

	maybebadio(port);
	return port;
}

/* Trying to read / write bytes on odd-byte boundaries to the Ethernet
 * registers causes problems. So we bit-shift the value and read / write
 * in 2 byte chunks. Setting the low byte to 0 does not cause problems
 * now as odd byte writes are only made on the bit mask / interrupt
 * register. This may not be the case in future Mar-2003 SJD
 */
unsigned char sh_edosk7705_inb(unsigned long port)
{
	if (port >= 0x300 && port < 0x320 && port & 0x01)
		return __raw_readw(port - 1) >> 8;

	return __raw_readb(sh_edosk7705_isa_port2addr(port));
}

void sh_edosk7705_outb(unsigned char value, unsigned long port)
{
	if (port >= 0x300 && port < 0x320 && port & 0x01) {
		__raw_writew(((unsigned short)value << 8), port - 1);
		return;
	}

	__raw_writeb(value, sh_edosk7705_isa_port2addr(port));
}

void sh_edosk7705_insb(unsigned long port, void *addr, unsigned long count)
{
	unsigned char *p = addr;

	while (count--)
		*p++ = sh_edosk7705_inb(port);
}

void sh_edosk7705_outsb(unsigned long port, const void *addr, unsigned long count)
{
	unsigned char *p = (unsigned char *)addr;

	while (count--)
		sh_edosk7705_outb(*p++, port);
}
