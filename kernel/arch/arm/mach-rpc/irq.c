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

#include <linux/init.h>
#include <linux/list.h>
#include <linux/io.h>

#include <asm/mach/irq.h>
#include <asm/hardware/iomd.h>
#include <asm/irq.h>

static void iomd_ack_irq_a(unsigned int irq)
{
	unsigned int val, mask;

	mask = 1 << irq;
	val = iomd_readb(IOMD_IRQMASKA);
	iomd_writeb(val & ~mask, IOMD_IRQMASKA);
	iomd_writeb(mask, IOMD_IRQCLRA);
}

static void iomd_mask_irq_a(unsigned int irq)
{
	unsigned int val, mask;

	mask = 1 << irq;
	val = iomd_readb(IOMD_IRQMASKA);
	iomd_writeb(val & ~mask, IOMD_IRQMASKA);
}

static void iomd_unmask_irq_a(unsigned int irq)
{
	unsigned int val, mask;

	mask = 1 << irq;
	val = iomd_readb(IOMD_IRQMASKA);
	iomd_writeb(val | mask, IOMD_IRQMASKA);
}

static struct irq_chip iomd_a_chip = {
	.ack	= iomd_ack_irq_a,
	.mask	= iomd_mask_irq_a,
	.unmask = iomd_unmask_irq_a,
};

static void iomd_mask_irq_b(unsigned int irq)
{
	unsigned int val, mask;

	mask = 1 << (irq & 7);
	val = iomd_readb(IOMD_IRQMASKB);
	iomd_writeb(val & ~mask, IOMD_IRQMASKB);
}

static void iomd_unmask_irq_b(unsigned int irq)
{
	unsigned int val, mask;

	mask = 1 << (irq & 7);
	val = iomd_readb(IOMD_IRQMASKB);
	iomd_writeb(val | mask, IOMD_IRQMASKB);
}

static struct irq_chip iomd_b_chip = {
	.ack	= iomd_mask_irq_b,
	.mask	= iomd_mask_irq_b,
	.unmask = iomd_unmask_irq_b,
};

static void iomd_mask_irq_dma(unsigned int irq)
{
	unsigned int val, mask;

	mask = 1 << (irq & 7);
	val = iomd_readb(IOMD_DMAMASK);
	iomd_writeb(val & ~mask, IOMD_DMAMASK);
}

static void iomd_unmask_irq_dma(unsigned int irq)
{
	unsigned int val, mask;

	mask = 1 << (irq & 7);
	val = iomd_readb(IOMD_DMAMASK);
	iomd_writeb(val | mask, IOMD_DMAMASK);
}

static struct irq_chip iomd_dma_chip = {
	.ack	= iomd_mask_irq_dma,
	.mask	= iomd_mask_irq_dma,
	.unmask = iomd_unmask_irq_dma,
};

static void iomd_mask_irq_fiq(unsigned int irq)
{
	unsigned int val, mask;

	mask = 1 << (irq & 7);
	val = iomd_readb(IOMD_FIQMASK);
	iomd_writeb(val & ~mask, IOMD_FIQMASK);
}

static void iomd_unmask_irq_fiq(unsigned int irq)
{
	unsigned int val, mask;

	mask = 1 << (irq & 7);
	val = iomd_readb(IOMD_FIQMASK);
	iomd_writeb(val | mask, IOMD_FIQMASK);
}

static struct irq_chip iomd_fiq_chip = {
	.ack	= iomd_mask_irq_fiq,
	.mask	= iomd_mask_irq_fiq,
	.unmask = iomd_unmask_irq_fiq,
};

void __init rpc_init_irq(void)
{
	unsigned int irq, flags;

	iomd_writeb(0, IOMD_IRQMASKA);
	iomd_writeb(0, IOMD_IRQMASKB);
	iomd_writeb(0, IOMD_FIQMASK);
	iomd_writeb(0, IOMD_DMAMASK);

	for (irq = 0; irq < NR_IRQS; irq++) {
		flags = IRQF_VALID;

		if (irq <= 6 || (irq >= 9 && irq <= 15))
			flags |= IRQF_PROBE;

		if (irq == 21 || (irq >= 16 && irq <= 19) ||
		    irq == IRQ_KEYBOARDTX)
			flags |= IRQF_NOAUTOEN;

		switch (irq) {
		case 0 ... 7:
			set_irq_chip(irq, &iomd_a_chip);
			set_irq_handler(irq, handle_level_irq);
			set_irq_flags(irq, flags);
			break;

		case 8 ... 15:
			set_irq_chip(irq, &iomd_b_chip);
			set_irq_handler(irq, handle_level_irq);
			set_irq_flags(irq, flags);
			break;

		case 16 ... 21:
			set_irq_chip(irq, &iomd_dma_chip);
			set_irq_handler(irq, handle_level_irq);
			set_irq_flags(irq, flags);
			break;

		case 64 ... 71:
			set_irq_chip(irq, &iomd_fiq_chip);
			set_irq_flags(irq, IRQF_VALID);
			break;
		}
	}

	init_FIQ();
}

