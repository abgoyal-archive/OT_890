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
 * arch/arm/mach-pnx4008/irq.c
 *
 * PNX4008 IRQ controller driver
 *
 * Author: Dmitry Chigirev <source@mvista.com>
 *
 * Based on reference code received from Philips:
 * Copyright (C) 2003 Philips Semiconductors
 *
 * 2005 (c) MontaVista Software, Inc. This file is licensed under
 * the terms of the GNU General Public License version 2. This program
 * is licensed "as is" without any warranty of any kind, whether express
 * or implied.
 */

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/mm.h>
#include <linux/interrupt.h>
#include <linux/list.h>
#include <linux/init.h>
#include <linux/ioport.h>
#include <linux/device.h>
#include <linux/irq.h>
#include <linux/io.h>
#include <mach/hardware.h>
#include <asm/setup.h>
#include <asm/pgtable.h>
#include <asm/page.h>
#include <asm/system.h>
#include <asm/mach/arch.h>
#include <asm/mach/irq.h>
#include <asm/mach/map.h>
#include <mach/irq.h>

static u8 pnx4008_irq_type[NR_IRQS] = PNX4008_IRQ_TYPES;

static void pnx4008_mask_irq(unsigned int irq)
{
	__raw_writel(__raw_readl(INTC_ER(irq)) & ~INTC_BIT(irq), INTC_ER(irq));	/* mask interrupt */
}

static void pnx4008_unmask_irq(unsigned int irq)
{
	__raw_writel(__raw_readl(INTC_ER(irq)) | INTC_BIT(irq), INTC_ER(irq));	/* unmask interrupt */
}

static void pnx4008_mask_ack_irq(unsigned int irq)
{
	__raw_writel(__raw_readl(INTC_ER(irq)) & ~INTC_BIT(irq), INTC_ER(irq));	/* mask interrupt */
	__raw_writel(INTC_BIT(irq), INTC_SR(irq));	/* clear interrupt status */
}

static int pnx4008_set_irq_type(unsigned int irq, unsigned int type)
{
	switch (type) {
	case IRQ_TYPE_EDGE_RISING:
		__raw_writel(__raw_readl(INTC_ATR(irq)) | INTC_BIT(irq), INTC_ATR(irq));	/*edge sensitive */
		__raw_writel(__raw_readl(INTC_APR(irq)) | INTC_BIT(irq), INTC_APR(irq));	/*rising edge */
		set_irq_handler(irq, handle_edge_irq);
		break;
	case IRQ_TYPE_EDGE_FALLING:
		__raw_writel(__raw_readl(INTC_ATR(irq)) | INTC_BIT(irq), INTC_ATR(irq));	/*edge sensitive */
		__raw_writel(__raw_readl(INTC_APR(irq)) & ~INTC_BIT(irq), INTC_APR(irq));	/*falling edge */
		set_irq_handler(irq, handle_edge_irq);
		break;
	case IRQ_TYPE_LEVEL_LOW:
		__raw_writel(__raw_readl(INTC_ATR(irq)) & ~INTC_BIT(irq), INTC_ATR(irq));	/*level sensitive */
		__raw_writel(__raw_readl(INTC_APR(irq)) & ~INTC_BIT(irq), INTC_APR(irq));	/*low level */
		set_irq_handler(irq, handle_level_irq);
		break;
	case IRQ_TYPE_LEVEL_HIGH:
		__raw_writel(__raw_readl(INTC_ATR(irq)) & ~INTC_BIT(irq), INTC_ATR(irq));	/*level sensitive */
		__raw_writel(__raw_readl(INTC_APR(irq)) | INTC_BIT(irq), INTC_APR(irq));	/* high level */
		set_irq_handler(irq, handle_level_irq);
		break;

	/* IRQ_TYPE_EDGE_BOTH is not supported */
	default:
		printk(KERN_ERR "PNX4008 IRQ: Unsupported irq type %d\n", type);
		return -1;
	}
	return 0;
}

static struct irq_chip pnx4008_irq_chip = {
	.ack = pnx4008_mask_ack_irq,
	.mask = pnx4008_mask_irq,
	.unmask = pnx4008_unmask_irq,
	.set_type = pnx4008_set_irq_type,
};

void __init pnx4008_init_irq(void)
{
	unsigned int i;

	/* configure IRQ's */
	for (i = 0; i < NR_IRQS; i++) {
		set_irq_flags(i, IRQF_VALID);
		set_irq_chip(i, &pnx4008_irq_chip);
		pnx4008_set_irq_type(i, pnx4008_irq_type[i]);
	}

	/* configure and enable IRQ 0,1,30,31 (cascade interrupts) */
	pnx4008_set_irq_type(SUB1_IRQ_N, pnx4008_irq_type[SUB1_IRQ_N]);
	pnx4008_set_irq_type(SUB2_IRQ_N, pnx4008_irq_type[SUB2_IRQ_N]);
	pnx4008_set_irq_type(SUB1_FIQ_N, pnx4008_irq_type[SUB1_FIQ_N]);
	pnx4008_set_irq_type(SUB2_FIQ_N, pnx4008_irq_type[SUB2_FIQ_N]);

	/* mask all others */
	__raw_writel((1 << SUB2_FIQ_N) | (1 << SUB1_FIQ_N) |
			(1 << SUB2_IRQ_N) | (1 << SUB1_IRQ_N),
		INTC_ER(MAIN_BASE_INT));
	__raw_writel(0, INTC_ER(SIC1_BASE_INT));
	__raw_writel(0, INTC_ER(SIC2_BASE_INT));
}

