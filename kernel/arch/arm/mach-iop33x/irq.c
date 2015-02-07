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
 * arch/arm/mach-iop33x/irq.c
 *
 * Generic IOP331 IRQ handling functionality
 *
 * Author: Dave Jiang <dave.jiang@intel.com>
 * Copyright (C) 2003 Intel Corp.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/list.h>
#include <asm/mach/irq.h>
#include <asm/irq.h>
#include <mach/hardware.h>
#include <asm/mach-types.h>

static u32 iop33x_mask0;
static u32 iop33x_mask1;

static void intctl0_write(u32 val)
{
	asm volatile("mcr p6, 0, %0, c0, c0, 0" : : "r" (val));
}

static void intctl1_write(u32 val)
{
	asm volatile("mcr p6, 0, %0, c1, c0, 0" : : "r" (val));
}

static void intstr0_write(u32 val)
{
	asm volatile("mcr p6, 0, %0, c2, c0, 0" : : "r" (val));
}

static void intstr1_write(u32 val)
{
	asm volatile("mcr p6, 0, %0, c3, c0, 0" : : "r" (val));
}

static void intbase_write(u32 val)
{
	asm volatile("mcr p6, 0, %0, c12, c0, 0" : : "r" (val));
}

static void intsize_write(u32 val)
{
	asm volatile("mcr p6, 0, %0, c13, c0, 0" : : "r" (val));
}

static void
iop33x_irq_mask1 (unsigned int irq)
{
	iop33x_mask0 &= ~(1 << irq);
	intctl0_write(iop33x_mask0);
}

static void
iop33x_irq_mask2 (unsigned int irq)
{
	iop33x_mask1 &= ~(1 << (irq - 32));
	intctl1_write(iop33x_mask1);
}

static void
iop33x_irq_unmask1(unsigned int irq)
{
	iop33x_mask0 |= 1 << irq;
	intctl0_write(iop33x_mask0);
}

static void
iop33x_irq_unmask2(unsigned int irq)
{
	iop33x_mask1 |= (1 << (irq - 32));
	intctl1_write(iop33x_mask1);
}

struct irq_chip iop33x_irqchip1 = {
	.name	= "IOP33x-1",
	.ack	= iop33x_irq_mask1,
	.mask	= iop33x_irq_mask1,
	.unmask	= iop33x_irq_unmask1,
};

struct irq_chip iop33x_irqchip2 = {
	.name	= "IOP33x-2",
	.ack	= iop33x_irq_mask2,
	.mask	= iop33x_irq_mask2,
	.unmask	= iop33x_irq_unmask2,
};

void __init iop33x_init_irq(void)
{
	int i;

	iop_init_cp6_handler();

	intctl0_write(0);
	intctl1_write(0);
	intstr0_write(0);
	intstr1_write(0);
	intbase_write(0);
	intsize_write(1);
	if (machine_is_iq80331())
		*IOP3XX_PCIIRSR = 0x0f;

	for (i = 0; i < NR_IRQS; i++) {
		set_irq_chip(i, (i < 32) ? &iop33x_irqchip1 : &iop33x_irqchip2);
		set_irq_handler(i, handle_level_irq);
		set_irq_flags(i, IRQF_VALID | IRQF_PROBE);
	}
}
