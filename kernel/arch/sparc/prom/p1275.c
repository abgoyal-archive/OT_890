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
 * p1275.c: Sun IEEE 1275 PROM low level interface routines
 *
 * Copyright (C) 1996,1997 Jakub Jelinek (jj@sunsite.mff.cuni.cz)
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/smp.h>
#include <linux/string.h>
#include <linux/spinlock.h>

#include <asm/openprom.h>
#include <asm/oplib.h>
#include <asm/system.h>
#include <asm/spitfire.h>
#include <asm/pstate.h>
#include <asm/ldc.h>

struct {
	long prom_callback;			/* 0x00 */
	void (*prom_cif_handler)(long *);	/* 0x08 */
	unsigned long prom_cif_stack;		/* 0x10 */
	unsigned long prom_args [23];		/* 0x18 */
	char prom_buffer [3000];
} p1275buf;

extern void prom_world(int);

extern void prom_cif_interface(void);
extern void prom_cif_callback(void);

/*
 * This provides SMP safety on the p1275buf. prom_callback() drops this lock
 * to allow recursuve acquisition.
 */
DEFINE_SPINLOCK(prom_entry_lock);

long p1275_cmd(const char *service, long fmt, ...)
{
	char *p, *q;
	unsigned long flags;
	int nargs, nrets, i;
	va_list list;
	long attrs, x;
	
	p = p1275buf.prom_buffer;

	spin_lock_irqsave(&prom_entry_lock, flags);

	p1275buf.prom_args[0] = (unsigned long)p;		/* service */
	strcpy (p, service);
	p = (char *)(((long)(strchr (p, 0) + 8)) & ~7);
	p1275buf.prom_args[1] = nargs = (fmt & 0x0f);		/* nargs */
	p1275buf.prom_args[2] = nrets = ((fmt & 0xf0) >> 4); 	/* nrets */
	attrs = fmt >> 8;
	va_start(list, fmt);
	for (i = 0; i < nargs; i++, attrs >>= 3) {
		switch (attrs & 0x7) {
		case P1275_ARG_NUMBER:
			p1275buf.prom_args[i + 3] =
						(unsigned)va_arg(list, long);
			break;
		case P1275_ARG_IN_64B:
			p1275buf.prom_args[i + 3] =
				va_arg(list, unsigned long);
			break;
		case P1275_ARG_IN_STRING:
			strcpy (p, va_arg(list, char *));
			p1275buf.prom_args[i + 3] = (unsigned long)p;
			p = (char *)(((long)(strchr (p, 0) + 8)) & ~7);
			break;
		case P1275_ARG_OUT_BUF:
			(void) va_arg(list, char *);
			p1275buf.prom_args[i + 3] = (unsigned long)p;
			x = va_arg(list, long);
			i++; attrs >>= 3;
			p = (char *)(((long)(p + (int)x + 7)) & ~7);
			p1275buf.prom_args[i + 3] = x;
			break;
		case P1275_ARG_IN_BUF:
			q = va_arg(list, char *);
			p1275buf.prom_args[i + 3] = (unsigned long)p;
			x = va_arg(list, long);
			i++; attrs >>= 3;
			memcpy (p, q, (int)x);
			p = (char *)(((long)(p + (int)x + 7)) & ~7);
			p1275buf.prom_args[i + 3] = x;
			break;
		case P1275_ARG_OUT_32B:
			(void) va_arg(list, char *);
			p1275buf.prom_args[i + 3] = (unsigned long)p;
			p += 32;
			break;
		case P1275_ARG_IN_FUNCTION:
			p1275buf.prom_args[i + 3] =
					(unsigned long)prom_cif_callback;
			p1275buf.prom_callback = va_arg(list, long);
			break;
		}
	}
	va_end(list);

	prom_world(1);
	prom_cif_interface();
	prom_world(0);

	attrs = fmt >> 8;
	va_start(list, fmt);
	for (i = 0; i < nargs; i++, attrs >>= 3) {
		switch (attrs & 0x7) {
		case P1275_ARG_NUMBER:
			(void) va_arg(list, long);
			break;
		case P1275_ARG_IN_STRING:
			(void) va_arg(list, char *);
			break;
		case P1275_ARG_IN_FUNCTION:
			(void) va_arg(list, long);
			break;
		case P1275_ARG_IN_BUF:
			(void) va_arg(list, char *);
			(void) va_arg(list, long);
			i++; attrs >>= 3;
			break;
		case P1275_ARG_OUT_BUF:
			p = va_arg(list, char *);
			x = va_arg(list, long);
			memcpy (p, (char *)(p1275buf.prom_args[i + 3]), (int)x);
			i++; attrs >>= 3;
			break;
		case P1275_ARG_OUT_32B:
			p = va_arg(list, char *);
			memcpy (p, (char *)(p1275buf.prom_args[i + 3]), 32);
			break;
		}
	}
	va_end(list);
	x = p1275buf.prom_args [nargs + 3];

	spin_unlock_irqrestore(&prom_entry_lock, flags);

	return x;
}

void prom_cif_init(void *cif_handler, void *cif_stack)
{
	p1275buf.prom_cif_handler = (void (*)(long *))cif_handler;
	p1275buf.prom_cif_stack = (unsigned long)cif_stack;
}
