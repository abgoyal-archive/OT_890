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
 *  linux/arch/m68k/hp300/time.c
 *
 *  Copyright (C) 1998 Philip Blundell <philb@gnu.org>
 *
 *  This file contains the HP300-specific time handling code.
 */

#include <asm/ptrace.h>
#include <linux/types.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/kernel_stat.h>
#include <linux/interrupt.h>
#include <asm/machdep.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <asm/system.h>
#include <asm/traps.h>
#include <asm/blinken.h>

/* Clock hardware definitions */

#define CLOCKBASE	0xf05f8000

#define	CLKCR1		0x1
#define	CLKCR2		0x3
#define	CLKCR3		CLKCR1
#define	CLKSR		CLKCR2
#define	CLKMSB1		0x5
#define	CLKMSB2		0x9
#define	CLKMSB3		0xD

/* This is for machines which generate the exact clock. */
#define USECS_PER_JIFFY (1000000/HZ)

#define INTVAL ((10000 / 4) - 1)

static irqreturn_t hp300_tick(int irq, void *dev_id)
{
	unsigned long tmp;
	irq_handler_t vector = dev_id;
	in_8(CLOCKBASE + CLKSR);
	asm volatile ("movpw %1@(5),%0" : "=d" (tmp) : "a" (CLOCKBASE));
	/* Turn off the network and SCSI leds */
	blinken_leds(0, 0xe0);
	return vector(irq, NULL);
}

unsigned long hp300_gettimeoffset(void)
{
  /* Read current timer 1 value */
  unsigned char lsb, msb1, msb2;
  unsigned short ticks;

  msb1 = in_8(CLOCKBASE + 5);
  lsb = in_8(CLOCKBASE + 7);
  msb2 = in_8(CLOCKBASE + 5);
  if (msb1 != msb2)
    /* A carry happened while we were reading.  Read it again */
    lsb = in_8(CLOCKBASE + 7);
  ticks = INTVAL - ((msb2 << 8) | lsb);
  return (USECS_PER_JIFFY * ticks) / INTVAL;
}

void __init hp300_sched_init(irq_handler_t vector)
{
  out_8(CLOCKBASE + CLKCR2, 0x1);		/* select CR1 */
  out_8(CLOCKBASE + CLKCR1, 0x1);		/* reset */

  asm volatile(" movpw %0,%1@(5)" : : "d" (INTVAL), "a" (CLOCKBASE));

  if (request_irq(IRQ_AUTO_6, hp300_tick, IRQ_FLG_STD, "timer tick", vector))
    pr_err("Couldn't register timer interrupt\n");

  out_8(CLOCKBASE + CLKCR2, 0x1);		/* select CR1 */
  out_8(CLOCKBASE + CLKCR1, 0x40);		/* enable irq */
}
