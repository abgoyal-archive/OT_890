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
 * arch/arm/mach-iop32x/irq.c
 *
 * Generic IOP32X IRQ handling functionality
 *
 * Author: Rory Bolt <rorybolt@pacbell.net>
 * Copyright (C) 2002 Rory Bolt
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

static u32 iop32x_mask;

static void intctl_write(u32 val)
{
	asm volatile("mcr p6, 0, %0, c0, c0, 0" : : "r" (val));
}

static void intstr_write(u32 val)
{
	asm volatile("mcr p6, 0, %0, c4, c0, 0" : : "r" (val));
}

static void
iop32x_irq_mask(unsigned int irq)
{
	iop32x_mask &= ~(1 << irq);
	intctl_write(iop32x_mask);
}

static void
iop32x_irq_unmask(unsigned int irq)
{
	iop32x_mask |= 1 << irq;
	intctl_write(iop32x_mask);
}

struct irq_chip ext_chip = {
	.name	= "IOP32x",
	.ack	= iop32x_irq_mask,
	.mask	= iop32x_irq_mask,
	.unmask	= iop32x_irq_unmask,
};

void __init iop32x_init_irq(void)
{
	int i;

	iop_init_cp6_handler();

	intctl_write(0);
	intstr_write(0);
	if (machine_is_glantank() ||
	    machine_is_iq80321() ||
	    machine_is_iq31244() ||
	    machine_is_n2100() ||
	    machine_is_em7210())
		*IOP3XX_PCIIRSR = 0x0f;

	for (i = 0; i < NR_IRQS; i++) {
		set_irq_chip(i, &ext_chip);
		set_irq_handler(i, handle_level_irq);
		set_irq_flags(i, IRQF_VALID | IRQF_PROBE);
	}
}
