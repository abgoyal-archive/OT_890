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
 *  linux/arch/arm/mm/mm-lusl7200.c
 *
 *  Copyright (C) 2000 Steve Hill (sjhill@cotw.com)
 *
 *  Extra MM routines for L7200 architecture
 */
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/irq.h>
#include <linux/device.h>

#include <asm/types.h>
#include <asm/irq.h>
#include <asm/mach-types.h>
#include <mach/hardware.h>
#include <asm/page.h>

#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <asm/mach/irq.h>

/*
 * IRQ base register
 */
#define	IRQ_BASE	(IO_BASE_2 + 0x1000)

/* 
 * Normal IRQ registers
 */
#define IRQ_STATUS	(*(volatile unsigned long *) (IRQ_BASE + 0x000))
#define IRQ_RAWSTATUS	(*(volatile unsigned long *) (IRQ_BASE + 0x004))
#define IRQ_ENABLE	(*(volatile unsigned long *) (IRQ_BASE + 0x008))
#define IRQ_ENABLECLEAR	(*(volatile unsigned long *) (IRQ_BASE + 0x00c))
#define IRQ_SOFT	(*(volatile unsigned long *) (IRQ_BASE + 0x010))
#define IRQ_SOURCESEL	(*(volatile unsigned long *) (IRQ_BASE + 0x018))

/* 
 * Fast IRQ registers
 */
#define FIQ_STATUS	(*(volatile unsigned long *) (IRQ_BASE + 0x100))
#define FIQ_RAWSTATUS	(*(volatile unsigned long *) (IRQ_BASE + 0x104))
#define FIQ_ENABLE	(*(volatile unsigned long *) (IRQ_BASE + 0x108))
#define FIQ_ENABLECLEAR	(*(volatile unsigned long *) (IRQ_BASE + 0x10c))
#define FIQ_SOFT	(*(volatile unsigned long *) (IRQ_BASE + 0x110))
#define FIQ_SOURCESEL	(*(volatile unsigned long *) (IRQ_BASE + 0x118))

static void l7200_mask_irq(unsigned int irq)
{
	IRQ_ENABLECLEAR = 1 << irq;
}

static void l7200_unmask_irq(unsigned int irq)
{
	IRQ_ENABLE = 1 << irq;
}

static struct irq_chip l7200_irq_chip = {
	.ack		= l7200_mask_irq,
	.mask		= l7200_mask_irq,
	.unmask		= l7200_unmask_irq
};
 
static void __init l7200_init_irq(void)
{
	int irq;

	IRQ_ENABLECLEAR = 0xffffffff;	/* clear all interrupt enables */
	FIQ_ENABLECLEAR = 0xffffffff;	/* clear all fast interrupt enables */

	for (irq = 0; irq < NR_IRQS; irq++) {
		set_irq_chip(irq, &l7200_irq_chip);
		set_irq_flags(irq, IRQF_VALID);
		set_irq_handler(irq, handle_level_irq);
	}

	init_FIQ();
}

static struct map_desc l7200_io_desc[] __initdata = {
	{ IO_BASE,	IO_START,	IO_SIZE,	MT_DEVICE },
	{ IO_BASE_2,	IO_START_2,	IO_SIZE_2,	MT_DEVICE },
	{ AUX_BASE,     AUX_START,      AUX_SIZE,       MT_DEVICE },
	{ FLASH1_BASE,  FLASH1_START,   FLASH1_SIZE,    MT_DEVICE },
	{ FLASH2_BASE,  FLASH2_START,   FLASH2_SIZE,    MT_DEVICE }
};

static void __init l7200_map_io(void)
{
	iotable_init(l7200_io_desc, ARRAY_SIZE(l7200_io_desc));
}

MACHINE_START(L7200, "LinkUp Systems L7200")
	/* Maintainer: Steve Hill / Scott McConnell */
	.phys_io	= 0x80040000,
	.io_pg_offst	= ((0xd0000000) >> 18) & 0xfffc,
	.map_io		= l7200_map_io,
	.init_irq	= l7200_init_irq,
MACHINE_END

