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
 * arch/arm/mach-at91/at91x40_time.c
 *
 * (C) Copyright 2007, Greg Ungerer <gerg@snapgear.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/time.h>
#include <linux/io.h>
#include <mach/hardware.h>
#include <asm/mach/time.h>
#include <mach/at91_tc.h>

/*
 *	3 counter/timer units present.
 */
#define	AT91_TC_CLK0BASE	0
#define	AT91_TC_CLK1BASE	0x40
#define	AT91_TC_CLK2BASE	0x80

static unsigned long at91x40_gettimeoffset(void)
{
	return (at91_sys_read(AT91_TC + AT91_TC_CLK1BASE + AT91_TC_CV) * 1000000 / (AT91X40_MASTER_CLOCK / 128));
}

static irqreturn_t at91x40_timer_interrupt(int irq, void *dev_id)
{
	at91_sys_read(AT91_TC + AT91_TC_CLK1BASE + AT91_TC_SR);
	timer_tick();
	return IRQ_HANDLED;
}

static struct irqaction at91x40_timer_irq = {
	.name		= "at91_tick",
	.flags		= IRQF_DISABLED | IRQF_TIMER,
	.handler	= at91x40_timer_interrupt
};

void __init at91x40_timer_init(void)
{
	unsigned int v;

	at91_sys_write(AT91_TC + AT91_TC_BCR, 0);
	v = at91_sys_read(AT91_TC + AT91_TC_BMR);
	v = (v & ~AT91_TC_TC1XC1S) | AT91_TC_TC1XC1S_NONE;
	at91_sys_write(AT91_TC + AT91_TC_BMR, v);

	at91_sys_write(AT91_TC + AT91_TC_CLK1BASE + AT91_TC_CCR, AT91_TC_CLKDIS);
	at91_sys_write(AT91_TC + AT91_TC_CLK1BASE + AT91_TC_CMR, (AT91_TC_TIMER_CLOCK4 | AT91_TC_CPCTRG));
	at91_sys_write(AT91_TC + AT91_TC_CLK1BASE + AT91_TC_IDR, 0xffffffff);
	at91_sys_write(AT91_TC + AT91_TC_CLK1BASE + AT91_TC_RC, (AT91X40_MASTER_CLOCK / 128) / HZ - 1);
	at91_sys_write(AT91_TC + AT91_TC_CLK1BASE + AT91_TC_IER, (1<<4));

	setup_irq(AT91X40_ID_TC1, &at91x40_timer_irq);

	at91_sys_write(AT91_TC + AT91_TC_CLK1BASE + AT91_TC_CCR, (AT91_TC_SWTRG | AT91_TC_CLKEN));
}

struct sys_timer at91x40_timer = {
	.init	= at91x40_timer_init,
	.offset	= at91x40_gettimeoffset,
};

