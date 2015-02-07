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
 * linux/arch/arm/mach-w90x900/irq.c
 *
 * based on linux/arch/arm/plat-s3c24xx/irq.c by Ben Dooks
 *
 * Copyright (c) 2008 Nuvoton technology corporation
 * All rights reserved.
 *
 * Wan ZongShun <mcuos.com@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/ptrace.h>
#include <linux/sysdev.h>
#include <linux/io.h>

#include <asm/irq.h>
#include <asm/mach/irq.h>

#include <mach/hardware.h>
#include <mach/regs-irq.h>

static void w90x900_irq_mask(unsigned int irq)
{
	__raw_writel(1 << irq, REG_AIC_MDCR);
}

/*
 * By the w90p910 spec,any irq,only write 1
 * to REG_AIC_EOSCR for ACK
 */

static void w90x900_irq_ack(unsigned int irq)
{
	__raw_writel(0x01, REG_AIC_EOSCR);
}

static void w90x900_irq_unmask(unsigned int irq)
{
	unsigned long mask;

	if (irq == IRQ_T_INT_GROUP) {
		mask = __raw_readl(REG_AIC_GEN);
		__raw_writel(TIME_GROUP_IRQ | mask, REG_AIC_GEN);
		__raw_writel(1 << IRQ_T_INT_GROUP, REG_AIC_MECR);
	}
	__raw_writel(1 << irq, REG_AIC_MECR);
}

static struct irq_chip w90x900_irq_chip = {
	.ack	   = w90x900_irq_ack,
	.mask	   = w90x900_irq_mask,
	.unmask	   = w90x900_irq_unmask,
};

void __init w90x900_init_irq(void)
{
	int irqno;

	__raw_writel(0xFFFFFFFE, REG_AIC_MDCR);

	for (irqno = IRQ_WDT; irqno <= IRQ_ADC; irqno++) {
		set_irq_chip(irqno, &w90x900_irq_chip);
		set_irq_handler(irqno, handle_level_irq);
		set_irq_flags(irqno, IRQF_VALID);
	}
}
