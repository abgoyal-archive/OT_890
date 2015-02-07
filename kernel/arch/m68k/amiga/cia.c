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
 *  linux/arch/m68k/amiga/cia.c - CIA support
 *
 *  Copyright (C) 1996 Roman Zippel
 *
 *  The concept of some functions bases on the original Amiga OS function
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file COPYING in the main directory of this archive
 * for more details.
 */

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/errno.h>
#include <linux/kernel_stat.h>
#include <linux/init.h>
#include <linux/seq_file.h>
#include <linux/interrupt.h>

#include <asm/irq.h>
#include <asm/amigahw.h>
#include <asm/amigaints.h>

struct ciabase {
	volatile struct CIA *cia;
	unsigned char icr_mask, icr_data;
	unsigned short int_mask;
	int handler_irq, cia_irq, server_irq;
	char *name;
} ciaa_base = {
	.cia		= &ciaa,
	.int_mask	= IF_PORTS,
	.handler_irq	= IRQ_AMIGA_PORTS,
	.cia_irq	= IRQ_AMIGA_CIAA,
	.name		= "CIAA"
}, ciab_base = {
	.cia		= &ciab,
	.int_mask	= IF_EXTER,
	.handler_irq	= IRQ_AMIGA_EXTER,
	.cia_irq	= IRQ_AMIGA_CIAB,
	.name		= "CIAB"
};

/*
 *  Cause or clear CIA interrupts, return old interrupt status.
 */

unsigned char cia_set_irq(struct ciabase *base, unsigned char mask)
{
	unsigned char old;

	old = (base->icr_data |= base->cia->icr);
	if (mask & CIA_ICR_SETCLR)
		base->icr_data |= mask;
	else
		base->icr_data &= ~mask;
	if (base->icr_data & base->icr_mask)
		amiga_custom.intreq = IF_SETCLR | base->int_mask;
	return old & base->icr_mask;
}

/*
 *  Enable or disable CIA interrupts, return old interrupt mask,
 */

unsigned char cia_able_irq(struct ciabase *base, unsigned char mask)
{
	unsigned char old;

	old = base->icr_mask;
	base->icr_data |= base->cia->icr;
	base->cia->icr = mask;
	if (mask & CIA_ICR_SETCLR)
		base->icr_mask |= mask;
	else
		base->icr_mask &= ~mask;
	base->icr_mask &= CIA_ICR_ALL;
	if (base->icr_data & base->icr_mask)
		amiga_custom.intreq = IF_SETCLR | base->int_mask;
	return old;
}

static irqreturn_t cia_handler(int irq, void *dev_id)
{
	struct ciabase *base = dev_id;
	int mach_irq;
	unsigned char ints;

	mach_irq = base->cia_irq;
	ints = cia_set_irq(base, CIA_ICR_ALL);
	amiga_custom.intreq = base->int_mask;
	for (; ints; mach_irq++, ints >>= 1) {
		if (ints & 1)
			m68k_handle_int(mach_irq);
	}
	return IRQ_HANDLED;
}

static void cia_enable_irq(unsigned int irq)
{
	unsigned char mask;

	if (irq >= IRQ_AMIGA_CIAB) {
		mask = 1 << (irq - IRQ_AMIGA_CIAB);
		cia_set_irq(&ciab_base, mask);
		cia_able_irq(&ciab_base, CIA_ICR_SETCLR | mask);
	} else {
		mask = 1 << (irq - IRQ_AMIGA_CIAA);
		cia_set_irq(&ciaa_base, mask);
		cia_able_irq(&ciaa_base, CIA_ICR_SETCLR | mask);
	}
}

static void cia_disable_irq(unsigned int irq)
{
	if (irq >= IRQ_AMIGA_CIAB)
		cia_able_irq(&ciab_base, 1 << (irq - IRQ_AMIGA_CIAB));
	else
		cia_able_irq(&ciaa_base, 1 << (irq - IRQ_AMIGA_CIAA));
}

static struct irq_controller cia_irq_controller = {
	.name		= "cia",
	.lock		= __SPIN_LOCK_UNLOCKED(cia_irq_controller.lock),
	.enable		= cia_enable_irq,
	.disable	= cia_disable_irq,
};

/*
 * Override auto irq 2 & 6 and use them as general chain
 * for external interrupts, we link the CIA interrupt sources
 * into this chain.
 */

static void auto_enable_irq(unsigned int irq)
{
	switch (irq) {
	case IRQ_AUTO_2:
		amiga_custom.intena = IF_SETCLR | IF_PORTS;
		break;
	case IRQ_AUTO_6:
		amiga_custom.intena = IF_SETCLR | IF_EXTER;
		break;
	}
}

static void auto_disable_irq(unsigned int irq)
{
	switch (irq) {
	case IRQ_AUTO_2:
		amiga_custom.intena = IF_PORTS;
		break;
	case IRQ_AUTO_6:
		amiga_custom.intena = IF_EXTER;
		break;
	}
}

static struct irq_controller auto_irq_controller = {
	.name		= "auto",
	.lock		= __SPIN_LOCK_UNLOCKED(auto_irq_controller.lock),
	.enable		= auto_enable_irq,
	.disable	= auto_disable_irq,
};

void __init cia_init_IRQ(struct ciabase *base)
{
	m68k_setup_irq_controller(&cia_irq_controller, base->cia_irq, CIA_IRQS);

	/* clear any pending interrupt and turn off all interrupts */
	cia_set_irq(base, CIA_ICR_ALL);
	cia_able_irq(base, CIA_ICR_ALL);

	/* override auto int and install CIA handler */
	m68k_setup_irq_controller(&auto_irq_controller, base->handler_irq, 1);
	m68k_irq_startup(base->handler_irq);
	if (request_irq(base->handler_irq, cia_handler, IRQF_SHARED,
			base->name, base))
		pr_err("Couldn't register %s interrupt\n", base->name);
}
