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

/* irq-mb93093.c: MB93093 FPGA interrupt handling
 *
 * Copyright (C) 2006 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#include <linux/ptrace.h>
#include <linux/errno.h>
#include <linux/signal.h>
#include <linux/sched.h>
#include <linux/ioport.h>
#include <linux/interrupt.h>
#include <linux/init.h>
#include <linux/irq.h>
#include <linux/bitops.h>

#include <asm/io.h>
#include <asm/system.h>
#include <asm/delay.h>
#include <asm/irq.h>
#include <asm/irc-regs.h>

#define __reg16(ADDR) (*(volatile unsigned short *)(__region_CS2 + (ADDR)))

#define __get_IMR()	({ __reg16(0x0a); })
#define __set_IMR(M)	do { __reg16(0x0a) = (M);  wmb(); } while(0)
#define __get_IFR()	({ __reg16(0x02); })
#define __clr_IFR(M)	do { __reg16(0x02) = ~(M); wmb(); } while(0)

/*
 * off-CPU FPGA PIC operations
 */
static void frv_fpga_mask(unsigned int irq)
{
	uint16_t imr = __get_IMR();

	imr |= 1 << (irq - IRQ_BASE_FPGA);
	__set_IMR(imr);
}

static void frv_fpga_ack(unsigned int irq)
{
	__clr_IFR(1 << (irq - IRQ_BASE_FPGA));
}

static void frv_fpga_mask_ack(unsigned int irq)
{
	uint16_t imr = __get_IMR();

	imr |= 1 << (irq - IRQ_BASE_FPGA);
	__set_IMR(imr);

	__clr_IFR(1 << (irq - IRQ_BASE_FPGA));
}

static void frv_fpga_unmask(unsigned int irq)
{
	uint16_t imr = __get_IMR();

	imr &= ~(1 << (irq - IRQ_BASE_FPGA));

	__set_IMR(imr);
}

static struct irq_chip frv_fpga_pic = {
	.name		= "mb93093",
	.ack		= frv_fpga_ack,
	.mask		= frv_fpga_mask,
	.mask_ack	= frv_fpga_mask_ack,
	.unmask		= frv_fpga_unmask,
	.end		= frv_fpga_end,
};

/*
 * FPGA PIC interrupt handler
 */
static irqreturn_t fpga_interrupt(int irq, void *_mask)
{
	uint16_t imr, mask = (unsigned long) _mask;

	imr = __get_IMR();
	mask = mask & ~imr & __get_IFR();

	/* poll all the triggered IRQs */
	while (mask) {
		int irq;

		asm("scan %1,gr0,%0" : "=r"(irq) : "r"(mask));
		irq = 31 - irq;
		mask &= ~(1 << irq);

		generic_irq_handle(IRQ_BASE_FPGA + irq);
	}

	return IRQ_HANDLED;
}

/*
 * define an interrupt action for each FPGA PIC output
 * - use dev_id to indicate the FPGA PIC input to output mappings
 */
static struct irqaction fpga_irq[1]  = {
	[0] = {
		.handler	= fpga_interrupt,
		.flags		= IRQF_DISABLED,
		.mask		= CPU_MASK_NONE,
		.name		= "fpga.0",
		.dev_id		= (void *) 0x0700UL,
	}
};

/*
 * initialise the motherboard FPGA's PIC
 */
void __init fpga_init(void)
{
	int irq;

	/* all PIC inputs are all set to be edge triggered */
	__set_IMR(0x0700);
	__clr_IFR(0x0000);

	for (irq = IRQ_BASE_FPGA + 8; irq <= IRQ_BASE_FPGA + 10; irq++)
		set_irq_chip_and_handler(irq, &frv_fpga_pic, handle_edge_irq);

	/* the FPGA drives external IRQ input #2 on the CPU PIC */
	setup_irq(IRQ_CPU_EXTERNAL2, &fpga_irq[0]);
}
