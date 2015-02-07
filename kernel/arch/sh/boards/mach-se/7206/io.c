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

/* $Id$
 *
 * linux/arch/sh/boards/se/7206/io.c
 *
 * Copyright (C) 2006 Yoshinori Sato
 *
 * I/O routine for Hitachi 7206 SolutionEngine.
 *
 */

#include <linux/kernel.h>
#include <linux/types.h>
#include <asm/io.h>
#include <mach-se/mach/se7206.h>


static inline void delay(void)
{
	ctrl_inw(0x20000000);  /* P2 ROM Area */
}

/* MS7750 requires special versions of in*, out* routines, since
   PC-like io ports are located at upper half byte of 16-bit word which
   can be accessed only with 16-bit wide.  */

static inline volatile __u16 *
port2adr(unsigned int port)
{
	if (port >= 0x2000 && port < 0x2020)
		return (volatile __u16 *) (PA_MRSHPC + (port - 0x2000));
	else if (port >= 0x300 && port < 0x310)
		return (volatile __u16 *) (PA_SMSC + (port - 0x300));

	return (volatile __u16 *)port;
}

unsigned char se7206_inb(unsigned long port)
{
	return (*port2adr(port)) & 0xff;
}

unsigned char se7206_inb_p(unsigned long port)
{
	unsigned long v;

	v = (*port2adr(port)) & 0xff;
	delay();
	return v;
}

unsigned short se7206_inw(unsigned long port)
{
	return *port2adr(port);;
}

void se7206_outb(unsigned char value, unsigned long port)
{
	*(port2adr(port)) = value;
}

void se7206_outb_p(unsigned char value, unsigned long port)
{
	*(port2adr(port)) = value;
	delay();
}

void se7206_outw(unsigned short value, unsigned long port)
{
	*port2adr(port) = value;
}

void se7206_insb(unsigned long port, void *addr, unsigned long count)
{
	volatile __u16 *p = port2adr(port);
	__u8 *ap = addr;

	while (count--)
		*ap++ = *p;
}

void se7206_insw(unsigned long port, void *addr, unsigned long count)
{
	volatile __u16 *p = port2adr(port);
	__u16 *ap = addr;
	while (count--)
		*ap++ = *p;
}

void se7206_outsb(unsigned long port, const void *addr, unsigned long count)
{
	volatile __u16 *p = port2adr(port);
	const __u8 *ap = addr;

	while (count--)
		*p = *ap++;
}

void se7206_outsw(unsigned long port, const void *addr, unsigned long count)
{
	volatile __u16 *p = port2adr(port);
	const __u16 *ap = addr;
	while (count--)
		*p = *ap++;
}
