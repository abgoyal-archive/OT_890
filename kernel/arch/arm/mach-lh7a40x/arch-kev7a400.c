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

/* arch/arm/mach-lh7a40x/arch-kev7a400.c
 *
 *  Copyright (C) 2004 Logic Product Development
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.
 *
 */

#include <linux/tty.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/interrupt.h>

#include <mach/hardware.h>
#include <asm/setup.h>
#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <asm/irq.h>
#include <asm/mach/irq.h>
#include <asm/mach/map.h>

#include "common.h"

      /* This function calls the board specific IRQ initialization function. */

static struct map_desc kev7a400_io_desc[] __initdata = {
	{
		.virtual	= IO_VIRT,
		.pfn		= __phys_to_pfn(IO_PHYS),
		.length		= IO_SIZE,
		.type		= MT_DEVICE
	}, {
		.virtual	= CPLD_VIRT,
		.pfn		= __phys_to_pfn(CPLD_PHYS),
		.length		= CPLD_SIZE,
		.type		= MT_DEVICE
	}
};

void __init kev7a400_map_io(void)
{
	iotable_init (kev7a400_io_desc, ARRAY_SIZE (kev7a400_io_desc));
}

static u16 CPLD_IRQ_mask;	/* Mask for CPLD IRQs, 1 == unmasked */

static void kev7a400_ack_cpld_irq (u32 irq)
{
	CPLD_CL_INT = 1 << (irq - IRQ_KEV7A400_CPLD);
}

static void kev7a400_mask_cpld_irq (u32 irq)
{
	CPLD_IRQ_mask &= ~(1 << (irq - IRQ_KEV7A400_CPLD));
	CPLD_WR_PB_INT_MASK = CPLD_IRQ_mask;
}

static void kev7a400_unmask_cpld_irq (u32 irq)
{
	CPLD_IRQ_mask |= 1 << (irq - IRQ_KEV7A400_CPLD);
	CPLD_WR_PB_INT_MASK = CPLD_IRQ_mask;
}

static struct irq_chip kev7a400_cpld_chip = {
	.name	= "CPLD",
	.ack	= kev7a400_ack_cpld_irq,
	.mask	= kev7a400_mask_cpld_irq,
	.unmask	= kev7a400_unmask_cpld_irq,
};


static void kev7a400_cpld_handler (unsigned int irq, struct irq_desc *desc)
{
	u32 mask = CPLD_LATCHED_INTS;
	irq = IRQ_KEV7A400_CPLD;
	for (; mask; mask >>= 1, ++irq)
		if (mask & 1)
			generic_handle_irq(irq);
}

void __init lh7a40x_init_board_irq (void)
{
	int irq;

	for (irq = IRQ_KEV7A400_CPLD;
	     irq < IRQ_KEV7A400_CPLD + NR_IRQ_BOARD; ++irq) {
		set_irq_chip (irq, &kev7a400_cpld_chip);
		set_irq_handler (irq, handle_edge_irq);
		set_irq_flags (irq, IRQF_VALID);
	}
	set_irq_chained_handler (IRQ_CPLD, kev7a400_cpld_handler);

		/* Clear all CPLD interrupts */
	CPLD_CL_INT = 0xff; /* CPLD_INTR_MMC_CD | CPLD_INTR_ETH_INT; */

	GPIO_GPIOINTEN = 0;		/* Disable all GPIO interrupts */
	barrier();

#if 0
	GPIO_INTTYPE1
		= (GPIO_INTR_PCC1_CD | GPIO_INTR_PCC1_CD); /* Edge trig. */
	GPIO_INTTYPE2 = 0;		/* Falling edge & low-level */
	GPIO_GPIOFEOI = 0xff;		/* Clear all GPIO interrupts */
	GPIO_GPIOINTEN = 0xff;		/* Enable all GPIO interrupts */

	init_FIQ();
#endif
}

MACHINE_START (KEV7A400, "Sharp KEV7a400")
	/* Maintainer: Marc Singer */
	.phys_io	= 0x80000000,
	.io_pg_offst	= ((io_p2v (0x80000000))>>18) & 0xfffc,
	.boot_params	= 0xc0000100,
	.map_io		= kev7a400_map_io,
	.init_irq	= lh7a400_init_irq,
	.timer		= &lh7a40x_timer,
MACHINE_END
