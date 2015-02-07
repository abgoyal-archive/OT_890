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
 * IRQ vector handles
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1995, 1996, 1997, 2003 by Ralf Baechle
 */
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/pci.h>

#include <asm/i8259.h>
#include <asm/irq_cpu.h>
#include <asm/irq_gt641xx.h>
#include <asm/gt64120.h>

#include <irq.h>

asmlinkage void plat_irq_dispatch(void)
{
	unsigned pending = read_c0_status() & read_c0_cause() & ST0_IM;
	int irq;

	if (pending & CAUSEF_IP2)
		gt641xx_irq_dispatch();
	else if (pending & CAUSEF_IP6) {
		irq = i8259_irq();
		if (irq < 0)
			spurious_interrupt();
		else
			do_IRQ(irq);
	} else if (pending & CAUSEF_IP3)
		do_IRQ(MIPS_CPU_IRQ_BASE + 3);
	else if (pending & CAUSEF_IP4)
		do_IRQ(MIPS_CPU_IRQ_BASE + 4);
	else if (pending & CAUSEF_IP5)
		do_IRQ(MIPS_CPU_IRQ_BASE + 5);
	else if (pending & CAUSEF_IP7)
		do_IRQ(MIPS_CPU_IRQ_BASE + 7);
	else
		spurious_interrupt();
}

static struct irqaction cascade = {
	.handler	= no_action,
	.mask		= CPU_MASK_NONE,
	.name		= "cascade",
};

void __init arch_init_irq(void)
{
	mips_cpu_irq_init();
	gt641xx_irq_init();
	init_i8259_irqs();

	setup_irq(GT641XX_CASCADE_IRQ, &cascade);
	setup_irq(I8259_CASCADE_IRQ, &cascade);
}
