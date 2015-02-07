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

/* debug-stub.c: debug-mode stub
 *
 * Copyright (C) 2004 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#include <linux/string.h>
#include <linux/kernel.h>
#include <linux/signal.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/serial_reg.h>

#include <asm/system.h>
#include <asm/serial-regs.h>
#include <asm/timer-regs.h>
#include <asm/irc-regs.h>
#include <asm/gdb-stub.h>
#include "gdb-io.h"

/* CPU board CON5 */
#define __UART0(X) (*(volatile uint8_t *)(UART0_BASE + (UART_##X)))

#define LSR_WAIT_FOR0(STATE)			\
do {						\
} while (!(__UART0(LSR) & UART_LSR_##STATE))

#define FLOWCTL_QUERY0(LINE)	({ __UART0(MSR) & UART_MSR_##LINE; })
#define FLOWCTL_CLEAR0(LINE)	do { __UART0(MCR) &= ~UART_MCR_##LINE; } while (0)
#define FLOWCTL_SET0(LINE)	do { __UART0(MCR) |= UART_MCR_##LINE; } while (0)

#define FLOWCTL_WAIT_FOR0(LINE)			\
do {						\
	gdbstub_do_rx();			\
} while(!FLOWCTL_QUERY(LINE))

struct frv_debug_status __debug_status;

static void __init debug_stub_init(void);

/*****************************************************************************/
/*
 * debug mode handler stub
 * - we come here with the CPU in debug mode and with exceptions disabled
 * - handle debugging services for userspace
 */
asmlinkage void debug_stub(void)
{
	unsigned long hsr0;
	int type = 0;

	static u8 inited = 0;
	if (!inited) {
		debug_stub_init();
		type = -1;
		inited = 1;
	}

	hsr0 = __get_HSR(0);
	if (hsr0 & HSR0_ETMD)
		__set_HSR(0, hsr0 & ~HSR0_ETMD);

	/* disable single stepping */
	__debug_status.dcr &= ~DCR_SE;

	/* kernel mode can propose an exception be handled in debug mode by jumping to a special
	 * location */
	if (__debug_frame->pc == (unsigned long) __break_hijack_kernel_event_breaks_here) {
		/* replace the debug frame with the kernel frame and discard
		 * the top kernel context */
		*__debug_frame = *__frame;
		__frame = __debug_frame->next_frame;
		__debug_status.brr = (__debug_frame->tbr & TBR_TT) << 12;
		__debug_status.brr |= BRR_EB;
	}

	if (__debug_frame->pc == (unsigned long) __debug_bug_trap + 4) {
		__debug_frame->pc = __debug_frame->lr;
		type = __debug_frame->gr8;
	}

#ifdef CONFIG_GDBSTUB
	gdbstub(type);
#endif

	if (hsr0 & HSR0_ETMD)
		__set_HSR(0, __get_HSR(0) | HSR0_ETMD);

} /* end debug_stub() */

/*****************************************************************************/
/*
 * debug stub initialisation
 */
static void __init debug_stub_init(void)
{
	__set_IRR(6, 0xff000000);	/* map ERRs to NMI */
	__set_IITMR(1, 0x20000000);	/* ERR0/1, UART0/1 IRQ detect levels */

	asm volatile("	movgs	gr0,ibar0	\n"
		     "	movgs	gr0,ibar1	\n"
		     "	movgs	gr0,ibar2	\n"
		     "	movgs	gr0,ibar3	\n"
		     "	movgs	gr0,dbar0	\n"
		     "	movgs	gr0,dbmr00	\n"
		     "	movgs	gr0,dbmr01	\n"
		     "	movgs	gr0,dbdr00	\n"
		     "	movgs	gr0,dbdr01	\n"
		     "	movgs	gr0,dbar1	\n"
		     "	movgs	gr0,dbmr10	\n"
		     "	movgs	gr0,dbmr11	\n"
		     "	movgs	gr0,dbdr10	\n"
		     "	movgs	gr0,dbdr11	\n"
		     );

	/* deal with debugging stub initialisation and initial pause */
	if (__debug_frame->pc == (unsigned long) __debug_stub_init_break)
		__debug_frame->pc = (unsigned long) start_kernel;

	/* enable the debug events we want to trap */
	__debug_status.dcr = DCR_EBE;

#ifdef CONFIG_GDBSTUB
	gdbstub_init();
#endif

	__clr_MASK_all();
	__clr_MASK(15);
	__clr_RC(15);

} /* end debug_stub_init() */

/*****************************************************************************/
/*
 * kernel "exit" trap for gdb stub
 */
void debug_stub_exit(int status)
{

#ifdef CONFIG_GDBSTUB
	gdbstub_exit(status);
#endif

} /* end debug_stub_exit() */

/*****************************************************************************/
/*
 * send string to serial port
 */
void debug_to_serial(const char *p, int n)
{
	char ch;

	for (; n > 0; n--) {
		ch = *p++;
		FLOWCTL_SET0(DTR);
		LSR_WAIT_FOR0(THRE);
		// FLOWCTL_WAIT_FOR(CTS);

		if (ch == 0x0a) {
			__UART0(TX) = 0x0d;
			mb();
			LSR_WAIT_FOR0(THRE);
			// FLOWCTL_WAIT_FOR(CTS);
		}
		__UART0(TX) = ch;
		mb();

		FLOWCTL_CLEAR0(DTR);
	}

} /* end debug_to_serial() */

/*****************************************************************************/
/*
 * send string to serial port
 */
void debug_to_serial2(const char *fmt, ...)
{
	va_list va;
	char buf[64];
	int n;

	va_start(va, fmt);
	n = vsprintf(buf, fmt, va);
	va_end(va);

	debug_to_serial(buf, n);

} /* end debug_to_serial2() */

/*****************************************************************************/
/*
 * set up the ttyS0 serial port baud rate timers
 */
void __init console_set_baud(unsigned baud)
{
	unsigned value, high, low;
	u8 lcr;

	/* work out the divisor to give us the nearest higher baud rate */
	value = __serial_clock_speed_HZ / 16 / baud;

	/* determine the baud rate range */
	high = __serial_clock_speed_HZ / 16 / value;
	low = __serial_clock_speed_HZ / 16 / (value + 1);

	/* pick the nearest bound */
	if (low + (high - low) / 2 > baud)
		value++;

	lcr = __UART0(LCR);
	__UART0(LCR) |= UART_LCR_DLAB;
	mb();
	__UART0(DLL) = value & 0xff;
	__UART0(DLM) = (value >> 8) & 0xff;
	mb();
	__UART0(LCR) = lcr;
	mb();

} /* end console_set_baud() */

/*****************************************************************************/
/*
 *
 */
int __init console_get_baud(void)
{
	unsigned value;
	u8 lcr;

	lcr = __UART0(LCR);
	__UART0(LCR) |= UART_LCR_DLAB;
	mb();
	value =  __UART0(DLM) << 8;
	value |= __UART0(DLL);
	__UART0(LCR) = lcr;
	mb();

	return value;
} /* end console_get_baud() */

/*****************************************************************************/
/*
 * display BUG() info
 */
#ifndef CONFIG_NO_KERNEL_MSG
void __debug_bug_printk(const char *file, unsigned line)
{
	printk("kernel BUG at %s:%d!\n", file, line);

} /* end __debug_bug_printk() */
#endif
