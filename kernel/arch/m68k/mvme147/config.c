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
 *  arch/m68k/mvme147/config.c
 *
 *  Copyright (C) 1996 Dave Frascone [chaos@mindspring.com]
 *  Cloned from        Richard Hirst [richard@sleepie.demon.co.uk]
 *
 * Based on:
 *
 *  Copyright (C) 1993 Hamish Macdonald
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file README.legal in the main directory of this archive
 * for more details.
 */

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/tty.h>
#include <linux/console.h>
#include <linux/linkage.h>
#include <linux/init.h>
#include <linux/major.h>
#include <linux/genhd.h>
#include <linux/rtc.h>
#include <linux/interrupt.h>

#include <asm/bootinfo.h>
#include <asm/system.h>
#include <asm/pgtable.h>
#include <asm/setup.h>
#include <asm/irq.h>
#include <asm/traps.h>
#include <asm/rtc.h>
#include <asm/machdep.h>
#include <asm/mvme147hw.h>


static void mvme147_get_model(char *model);
extern void mvme147_sched_init(irq_handler_t handler);
extern unsigned long mvme147_gettimeoffset (void);
extern int mvme147_hwclk (int, struct rtc_time *);
extern int mvme147_set_clock_mmss (unsigned long);
extern void mvme147_reset (void);


static int bcd2int (unsigned char b);

/* Save tick handler routine pointer, will point to do_timer() in
 * kernel/sched.c, called via mvme147_process_int() */

irq_handler_t tick_handler;


int mvme147_parse_bootinfo(const struct bi_record *bi)
{
	if (bi->tag == BI_VME_TYPE || bi->tag == BI_VME_BRDINFO)
		return 0;
	else
		return 1;
}

void mvme147_reset(void)
{
	printk ("\r\n\nCalled mvme147_reset\r\n");
	m147_pcc->watchdog = 0x0a;	/* Clear timer */
	m147_pcc->watchdog = 0xa5;	/* Enable watchdog - 100ms to reset */
	while (1)
		;
}

static void mvme147_get_model(char *model)
{
	sprintf(model, "Motorola MVME147");
}

/*
 * This function is called during kernel startup to initialize
 * the mvme147 IRQ handling routines.
 */

void __init mvme147_init_IRQ(void)
{
	m68k_setup_user_interrupt(VEC_USER, 192, NULL);
}

void __init config_mvme147(void)
{
	mach_max_dma_address	= 0x01000000;
	mach_sched_init		= mvme147_sched_init;
	mach_init_IRQ		= mvme147_init_IRQ;
	mach_gettimeoffset	= mvme147_gettimeoffset;
	mach_hwclk		= mvme147_hwclk;
	mach_set_clock_mmss	= mvme147_set_clock_mmss;
	mach_reset		= mvme147_reset;
	mach_get_model		= mvme147_get_model;

	/* Board type is only set by newer versions of vmelilo/tftplilo */
	if (!vme_brdtype)
		vme_brdtype = VME_TYPE_MVME147;
}


/* Using pcc tick timer 1 */

static irqreturn_t mvme147_timer_int (int irq, void *dev_id)
{
	m147_pcc->t1_int_cntrl = PCC_TIMER_INT_CLR;
	m147_pcc->t1_int_cntrl = PCC_INT_ENAB|PCC_LEVEL_TIMER1;
	return tick_handler(irq, dev_id);
}


void mvme147_sched_init (irq_handler_t timer_routine)
{
	tick_handler = timer_routine;
	if (request_irq(PCC_IRQ_TIMER1, mvme147_timer_int, IRQ_FLG_REPLACE,
			"timer 1", NULL))
		pr_err("Couldn't register timer interrupt\n");

	/* Init the clock with a value */
	/* our clock goes off every 6.25us */
	m147_pcc->t1_preload = PCC_TIMER_PRELOAD;
	m147_pcc->t1_cntrl = 0x0;	/* clear timer */
	m147_pcc->t1_cntrl = 0x3;	/* start timer */
	m147_pcc->t1_int_cntrl = PCC_TIMER_INT_CLR;  /* clear pending ints */
	m147_pcc->t1_int_cntrl = PCC_INT_ENAB|PCC_LEVEL_TIMER1;
}

/* This is always executed with interrupts disabled.  */
/* XXX There are race hazards in this code XXX */
unsigned long mvme147_gettimeoffset (void)
{
	volatile unsigned short *cp = (volatile unsigned short *)0xfffe1012;
	unsigned short n;

	n = *cp;
	while (n != *cp)
		n = *cp;

	n -= PCC_TIMER_PRELOAD;
	return (unsigned long)n * 25 / 4;
}

static int bcd2int (unsigned char b)
{
	return ((b>>4)*10 + (b&15));
}

int mvme147_hwclk(int op, struct rtc_time *t)
{
#warning check me!
	if (!op) {
		m147_rtc->ctrl = RTC_READ;
		t->tm_year = bcd2int (m147_rtc->bcd_year);
		t->tm_mon  = bcd2int (m147_rtc->bcd_mth);
		t->tm_mday = bcd2int (m147_rtc->bcd_dom);
		t->tm_hour = bcd2int (m147_rtc->bcd_hr);
		t->tm_min  = bcd2int (m147_rtc->bcd_min);
		t->tm_sec  = bcd2int (m147_rtc->bcd_sec);
		m147_rtc->ctrl = 0;
	}
	return 0;
}

int mvme147_set_clock_mmss (unsigned long nowtime)
{
	return 0;
}

/*-------------------  Serial console stuff ------------------------*/

static void scc_delay (void)
{
	int n;
	volatile int trash;

	for (n = 0; n < 20; n++)
		trash = n;
}

static void scc_write (char ch)
{
	volatile char *p = (volatile char *)M147_SCC_A_ADDR;

	do {
		scc_delay();
	}
	while (!(*p & 4));
	scc_delay();
	*p = 8;
	scc_delay();
	*p = ch;
}


void m147_scc_write (struct console *co, const char *str, unsigned count)
{
	unsigned long flags;

	local_irq_save(flags);

	while (count--)
	{
		if (*str == '\n')
			scc_write ('\r');
		scc_write (*str++);
	}
	local_irq_restore(flags);
}

void mvme147_init_console_port (struct console *co, int cflag)
{
	co->write    = m147_scc_write;
}
