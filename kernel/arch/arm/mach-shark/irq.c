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
 *  linux/arch/arm/mach-shark/irq.c
 *
 * by Alexander Schulz
 *
 * derived from linux/arch/ppc/kernel/i8259.c and:
 * arch/arm/mach-ebsa110/include/mach/irq.h
 * Copyright (C) 1996-1998 Russell King
 */

#include <linux/init.h>
#include <linux/fs.h>
#include <linux/interrupt.h>
#include <linux/io.h>

#include <asm/irq.h>
#include <asm/mach/irq.h>

/*
 * 8259A PIC functions to handle ISA devices:
 */

/*
 * This contains the irq mask for both 8259A irq controllers,
 * Let through the cascade-interrupt no. 2 (ff-(1<<2)==fb)
 */
static unsigned char cached_irq_mask[2] = { 0xfb, 0xff };

/*
 * These have to be protected by the irq controller spinlock
 * before being called.
 */
static void shark_disable_8259A_irq(unsigned int irq)
{
	unsigned int mask;
	if (irq<8) {
	  mask = 1 << irq;
	  cached_irq_mask[0] |= mask;
	  outb(cached_irq_mask[1],0xA1);
	} else {
	  mask = 1 << (irq-8);
	  cached_irq_mask[1] |= mask;
	  outb(cached_irq_mask[0],0x21);
	}
}

static void shark_enable_8259A_irq(unsigned int irq)
{
	unsigned int mask;
	if (irq<8) {
	  mask = ~(1 << irq);
	  cached_irq_mask[0] &= mask;
	  outb(cached_irq_mask[0],0x21);
	} else {
	  mask = ~(1 << (irq-8));
	  cached_irq_mask[1] &= mask;
	  outb(cached_irq_mask[1],0xA1);
	}
}

static void shark_ack_8259A_irq(unsigned int irq){}

static irqreturn_t bogus_int(int irq, void *dev_id)
{
	printk("Got interrupt %i!\n",irq);
	return IRQ_NONE;
}

static struct irqaction cascade;

static struct irq_chip fb_chip = {
	.name	= "XT-PIC",
	.ack	= shark_ack_8259A_irq,
	.mask	= shark_disable_8259A_irq,
	.unmask = shark_enable_8259A_irq,
};

void __init shark_init_irq(void)
{
	int irq;

	for (irq = 0; irq < NR_IRQS; irq++) {
		set_irq_chip(irq, &fb_chip);
		set_irq_handler(irq, handle_edge_irq);
		set_irq_flags(irq, IRQF_VALID | IRQF_PROBE);
	}

	/* init master interrupt controller */
	outb(0x11, 0x20); /* Start init sequence, edge triggered (level: 0x19)*/
	outb(0x00, 0x21); /* Vector base */
	outb(0x04, 0x21); /* Cascade (slave) on IRQ2 */
	outb(0x03, 0x21); /* Select 8086 mode , auto eoi*/
	outb(0x0A, 0x20);
	/* init slave interrupt controller */
	outb(0x11, 0xA0); /* Start init sequence, edge triggered */
	outb(0x08, 0xA1); /* Vector base */
	outb(0x02, 0xA1); /* Cascade (slave) on IRQ2 */
	outb(0x03, 0xA1); /* Select 8086 mode, auto eoi */
	outb(0x0A, 0xA0);
	outb(cached_irq_mask[1],0xA1);
	outb(cached_irq_mask[0],0x21);
	//request_region(0x20,0x2,"pic1");
	//request_region(0xA0,0x2,"pic2");

	cascade.handler = bogus_int;
	cascade.name = "cascade";
	setup_irq(2,&cascade);
}

