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
 * I/O string operations
 *    Copyright (C) 1995-1996 Gary Thomas (gdt@linuxppc.org)
 *    Copyright (C) 2006 IBM Corporation
 *
 * Largely rewritten by Cort Dougan (cort@cs.nmt.edu)
 * and Paul Mackerras.
 *
 * Adapted for iSeries by Mike Corrigan (mikejc@us.ibm.com)
 * PPC64 updates by Dave Engebretsen (engebret@us.ibm.com)
 *
 * Rewritten in C by Stephen Rothwell.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/compiler.h>
#include <linux/module.h>

#include <asm/io.h>
#include <asm/firmware.h>
#include <asm/bug.h>

void _insb(const volatile u8 __iomem *port, void *buf, long count)
{
	u8 *tbuf = buf;
	u8 tmp;

	if (unlikely(count <= 0))
		return;
	asm volatile("sync");
	do {
		tmp = *port;
		eieio();
		*tbuf++ = tmp;
	} while (--count != 0);
	asm volatile("twi 0,%0,0; isync" : : "r" (tmp));
}
EXPORT_SYMBOL(_insb);

void _outsb(volatile u8 __iomem *port, const void *buf, long count)
{
	const u8 *tbuf = buf;

	if (unlikely(count <= 0))
		return;
	asm volatile("sync");
	do {
		*port = *tbuf++;
	} while (--count != 0);
	asm volatile("sync");
}
EXPORT_SYMBOL(_outsb);

void _insw_ns(const volatile u16 __iomem *port, void *buf, long count)
{
	u16 *tbuf = buf;
	u16 tmp;

	if (unlikely(count <= 0))
		return;
	asm volatile("sync");
	do {
		tmp = *port;
		eieio();
		*tbuf++ = tmp;
	} while (--count != 0);
	asm volatile("twi 0,%0,0; isync" : : "r" (tmp));
}
EXPORT_SYMBOL(_insw_ns);

void _outsw_ns(volatile u16 __iomem *port, const void *buf, long count)
{
	const u16 *tbuf = buf;

	if (unlikely(count <= 0))
		return;
	asm volatile("sync");
	do {
		*port = *tbuf++;
	} while (--count != 0);
	asm volatile("sync");
}
EXPORT_SYMBOL(_outsw_ns);

void _insl_ns(const volatile u32 __iomem *port, void *buf, long count)
{
	u32 *tbuf = buf;
	u32 tmp;

	if (unlikely(count <= 0))
		return;
	asm volatile("sync");
	do {
		tmp = *port;
		eieio();
		*tbuf++ = tmp;
	} while (--count != 0);
	asm volatile("twi 0,%0,0; isync" : : "r" (tmp));
}
EXPORT_SYMBOL(_insl_ns);

void _outsl_ns(volatile u32 __iomem *port, const void *buf, long count)
{
	const u32 *tbuf = buf;

	if (unlikely(count <= 0))
		return;
	asm volatile("sync");
	do {
		*port = *tbuf++;
	} while (--count != 0);
	asm volatile("sync");
}
EXPORT_SYMBOL(_outsl_ns);

#define IO_CHECK_ALIGN(v,a) ((((unsigned long)(v)) & ((a) - 1)) == 0)

notrace void
_memset_io(volatile void __iomem *addr, int c, unsigned long n)
{
	void *p = (void __force *)addr;
	u32 lc = c;
	lc |= lc << 8;
	lc |= lc << 16;

	__asm__ __volatile__ ("sync" : : : "memory");
	while(n && !IO_CHECK_ALIGN(p, 4)) {
		*((volatile u8 *)p) = c;
		p++;
		n--;
	}
	while(n >= 4) {
		*((volatile u32 *)p) = lc;
		p += 4;
		n -= 4;
	}
	while(n) {
		*((volatile u8 *)p) = c;
		p++;
		n--;
	}
	__asm__ __volatile__ ("sync" : : : "memory");
}
EXPORT_SYMBOL(_memset_io);

void _memcpy_fromio(void *dest, const volatile void __iomem *src,
		    unsigned long n)
{
	void *vsrc = (void __force *) src;

	__asm__ __volatile__ ("sync" : : : "memory");
	while(n && (!IO_CHECK_ALIGN(vsrc, 4) || !IO_CHECK_ALIGN(dest, 4))) {
		*((u8 *)dest) = *((volatile u8 *)vsrc);
		eieio();
		vsrc++;
		dest++;
		n--;
	}
	while(n > 4) {
		*((u32 *)dest) = *((volatile u32 *)vsrc);
		eieio();
		vsrc += 4;
		dest += 4;
		n -= 4;
	}
	while(n) {
		*((u8 *)dest) = *((volatile u8 *)vsrc);
		eieio();
		vsrc++;
		dest++;
		n--;
	}
	__asm__ __volatile__ ("sync" : : : "memory");
}
EXPORT_SYMBOL(_memcpy_fromio);

void _memcpy_toio(volatile void __iomem *dest, const void *src, unsigned long n)
{
	void *vdest = (void __force *) dest;

	__asm__ __volatile__ ("sync" : : : "memory");
	while(n && (!IO_CHECK_ALIGN(vdest, 4) || !IO_CHECK_ALIGN(src, 4))) {
		*((volatile u8 *)vdest) = *((u8 *)src);
		src++;
		vdest++;
		n--;
	}
	while(n > 4) {
		*((volatile u32 *)vdest) = *((volatile u32 *)src);
		src += 4;
		vdest += 4;
		n-=4;
	}
	while(n) {
		*((volatile u8 *)vdest) = *((u8 *)src);
		src++;
		vdest++;
		n--;
	}
	__asm__ __volatile__ ("sync" : : : "memory");
}
EXPORT_SYMBOL(_memcpy_toio);
