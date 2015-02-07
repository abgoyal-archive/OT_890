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
 * arch/arm/mach-mv78xx0/irq.c
 *
 * MV78xx0 IRQ handling.
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2.  This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/pci.h>
#include <linux/irq.h>
#include <asm/gpio.h>
#include <mach/mv78xx0.h>
#include <plat/irq.h>
#include "common.h"

static void gpio_irq_handler(unsigned int irq, struct irq_desc *desc)
{
	BUG_ON(irq < IRQ_MV78XX0_GPIO_0_7 || irq > IRQ_MV78XX0_GPIO_24_31);

	orion_gpio_irq_handler((irq - IRQ_MV78XX0_GPIO_0_7) << 3);
}

void __init mv78xx0_init_irq(void)
{
	int i;

	orion_irq_init(0, (void __iomem *)(IRQ_VIRT_BASE + IRQ_MASK_LOW_OFF));
	orion_irq_init(32, (void __iomem *)(IRQ_VIRT_BASE + IRQ_MASK_HIGH_OFF));
	orion_irq_init(64, (void __iomem *)(IRQ_VIRT_BASE + IRQ_MASK_ERR_OFF));

	/*
	 * Mask and clear GPIO IRQ interrupts.
	 */
	writel(0, GPIO_LEVEL_MASK(0));
	writel(0, GPIO_EDGE_MASK(0));
	writel(0, GPIO_EDGE_CAUSE(0));

	for (i = IRQ_MV78XX0_GPIO_START; i < NR_IRQS; i++) {
		set_irq_chip(i, &orion_gpio_irq_chip);
		set_irq_handler(i, handle_level_irq);
		irq_desc[i].status |= IRQ_LEVEL;
		set_irq_flags(i, IRQF_VALID);
	}
	set_irq_chained_handler(IRQ_MV78XX0_GPIO_0_7, gpio_irq_handler);
	set_irq_chained_handler(IRQ_MV78XX0_GPIO_8_15, gpio_irq_handler);
	set_irq_chained_handler(IRQ_MV78XX0_GPIO_16_23, gpio_irq_handler);
	set_irq_chained_handler(IRQ_MV78XX0_GPIO_24_31, gpio_irq_handler);
}
