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

/* linux/arch/arm/mach-msm/irq.c
 *
 * Copyright (C) 2007 Google, Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/ptrace.h>
#include <linux/timer.h>
#include <linux/irq.h>
#include <linux/io.h>

#include <mach/hardware.h>

#include <mach/msm_iomap.h>

#define VIC_REG(off) (MSM_VIC_BASE + (off))

#define VIC_INT_SELECT0     VIC_REG(0x0000)  /* 1: FIQ, 0: IRQ */
#define VIC_INT_SELECT1     VIC_REG(0x0004)  /* 1: FIQ, 0: IRQ */
#define VIC_INT_EN0         VIC_REG(0x0010)
#define VIC_INT_EN1         VIC_REG(0x0014)
#define VIC_INT_ENCLEAR0    VIC_REG(0x0020)
#define VIC_INT_ENCLEAR1    VIC_REG(0x0024)
#define VIC_INT_ENSET0      VIC_REG(0x0030)
#define VIC_INT_ENSET1      VIC_REG(0x0034)
#define VIC_INT_TYPE0       VIC_REG(0x0040)  /* 1: EDGE, 0: LEVEL  */
#define VIC_INT_TYPE1       VIC_REG(0x0044)  /* 1: EDGE, 0: LEVEL  */
#define VIC_INT_POLARITY0   VIC_REG(0x0050)  /* 1: NEG, 0: POS */
#define VIC_INT_POLARITY1   VIC_REG(0x0054)  /* 1: NEG, 0: POS */
#define VIC_NO_PEND_VAL     VIC_REG(0x0060)
#define VIC_INT_MASTEREN    VIC_REG(0x0064)  /* 1: IRQ, 2: FIQ     */
#define VIC_PROTECTION      VIC_REG(0x006C)  /* 1: ENABLE          */
#define VIC_CONFIG          VIC_REG(0x0068)  /* 1: USE ARM1136 VIC */
#define VIC_IRQ_STATUS0     VIC_REG(0x0080)
#define VIC_IRQ_STATUS1     VIC_REG(0x0084)
#define VIC_FIQ_STATUS0     VIC_REG(0x0090)
#define VIC_FIQ_STATUS1     VIC_REG(0x0094)
#define VIC_RAW_STATUS0     VIC_REG(0x00A0)
#define VIC_RAW_STATUS1     VIC_REG(0x00A4)
#define VIC_INT_CLEAR0      VIC_REG(0x00B0)
#define VIC_INT_CLEAR1      VIC_REG(0x00B4)
#define VIC_SOFTINT0        VIC_REG(0x00C0)
#define VIC_SOFTINT1        VIC_REG(0x00C4)
#define VIC_IRQ_VEC_RD      VIC_REG(0x00D0)  /* pending int # */
#define VIC_IRQ_VEC_PEND_RD VIC_REG(0x00D4)  /* pending vector addr */
#define VIC_IRQ_VEC_WR      VIC_REG(0x00D8)
#define VIC_IRQ_IN_SERVICE  VIC_REG(0x00E0)
#define VIC_IRQ_IN_STACK    VIC_REG(0x00E4)
#define VIC_TEST_BUS_SEL    VIC_REG(0x00E8)

#define VIC_VECTPRIORITY(n) VIC_REG(0x0200+((n) * 4))
#define VIC_VECTADDR(n)     VIC_REG(0x0400+((n) * 4))

static void msm_irq_ack(unsigned int irq)
{
	void __iomem *reg = VIC_INT_CLEAR0 + ((irq & 32) ? 4 : 0);
	irq = 1 << (irq & 31);
	writel(irq, reg);
}

static void msm_irq_mask(unsigned int irq)
{
	void __iomem *reg = VIC_INT_ENCLEAR0 + ((irq & 32) ? 4 : 0);
	writel(1 << (irq & 31), reg);
}

static void msm_irq_unmask(unsigned int irq)
{
	void __iomem *reg = VIC_INT_ENSET0 + ((irq & 32) ? 4 : 0);
	writel(1 << (irq & 31), reg);
}

static int msm_irq_set_wake(unsigned int irq, unsigned int on)
{
	return -EINVAL;
}

static int msm_irq_set_type(unsigned int irq, unsigned int flow_type)
{
	void __iomem *treg = VIC_INT_TYPE0 + ((irq & 32) ? 4 : 0);
	void __iomem *preg = VIC_INT_POLARITY0 + ((irq & 32) ? 4 : 0);
	int b = 1 << (irq & 31);

	if (flow_type & (IRQF_TRIGGER_FALLING | IRQF_TRIGGER_LOW))
		writel(readl(preg) | b, preg);
	if (flow_type & (IRQF_TRIGGER_RISING | IRQF_TRIGGER_HIGH))
		writel(readl(preg) & (~b), preg);

	if (flow_type & (IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING)) {
		writel(readl(treg) | b, treg);
		set_irq_handler(irq, handle_edge_irq);
	}
	if (flow_type & (IRQF_TRIGGER_HIGH | IRQF_TRIGGER_LOW)) {
		writel(readl(treg) & (~b), treg);
		set_irq_handler(irq, handle_level_irq);
	}
	return 0;
}

static struct irq_chip msm_irq_chip = {
	.name      = "msm",
	.ack       = msm_irq_ack,
	.mask      = msm_irq_mask,
	.unmask    = msm_irq_unmask,
	.set_wake  = msm_irq_set_wake,
	.set_type  = msm_irq_set_type,
};

void __init msm_init_irq(void)
{
	unsigned n;

	/* select level interrupts */
	writel(0, VIC_INT_TYPE0);
	writel(0, VIC_INT_TYPE1);

	/* select highlevel interrupts */
	writel(0, VIC_INT_POLARITY0);
	writel(0, VIC_INT_POLARITY1);

	/* select IRQ for all INTs */
	writel(0, VIC_INT_SELECT0);
	writel(0, VIC_INT_SELECT1);

	/* disable all INTs */
	writel(0, VIC_INT_EN0);
	writel(0, VIC_INT_EN1);

	/* don't use 1136 vic */
	writel(0, VIC_CONFIG);

	/* enable interrupt controller */
	writel(1, VIC_INT_MASTEREN);

	for (n = 0; n < NR_MSM_IRQS; n++) {
		set_irq_chip(n, &msm_irq_chip);
		set_irq_handler(n, handle_level_irq);
		set_irq_flags(n, IRQF_VALID);
	}
}
