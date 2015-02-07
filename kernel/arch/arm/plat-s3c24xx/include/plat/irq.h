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

/* linux/include/asm-arm/plat-s3c24xx/irq.h
 *
 * Copyright (c) 2004-2005 Simtec Electronics
 *	Ben Dooks <ben@simtec.co.uk>
 *
 * Header file for S3C24XX CPU IRQ support
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#define irqdbf(x...)
#define irqdbf2(x...)

#define EXTINT_OFF (IRQ_EINT4 - 4)

/* these are exported for arch/arm/mach-* usage */
extern struct irq_chip s3c_irq_level_chip;
extern struct irq_chip s3c_irq_chip;

static inline void
s3c_irqsub_mask(unsigned int irqno, unsigned int parentbit,
		int subcheck)
{
	unsigned long mask;
	unsigned long submask;

	submask = __raw_readl(S3C2410_INTSUBMSK);
	mask = __raw_readl(S3C2410_INTMSK);

	submask |= (1UL << (irqno - IRQ_S3CUART_RX0));

	/* check to see if we need to mask the parent IRQ */

	if ((submask  & subcheck) == subcheck) {
		__raw_writel(mask | parentbit, S3C2410_INTMSK);
	}

	/* write back masks */
	__raw_writel(submask, S3C2410_INTSUBMSK);

}

static inline void
s3c_irqsub_unmask(unsigned int irqno, unsigned int parentbit)
{
	unsigned long mask;
	unsigned long submask;

	submask = __raw_readl(S3C2410_INTSUBMSK);
	mask = __raw_readl(S3C2410_INTMSK);

	submask &= ~(1UL << (irqno - IRQ_S3CUART_RX0));
	mask &= ~parentbit;

	/* write back masks */
	__raw_writel(submask, S3C2410_INTSUBMSK);
	__raw_writel(mask, S3C2410_INTMSK);
}


static inline void
s3c_irqsub_maskack(unsigned int irqno, unsigned int parentmask, unsigned int group)
{
	unsigned int bit = 1UL << (irqno - IRQ_S3CUART_RX0);

	s3c_irqsub_mask(irqno, parentmask, group);

	__raw_writel(bit, S3C2410_SUBSRCPND);

	/* only ack parent if we've got all the irqs (seems we must
	 * ack, all and hope that the irq system retriggers ok when
	 * the interrupt goes off again)
	 */

	if (1) {
		__raw_writel(parentmask, S3C2410_SRCPND);
		__raw_writel(parentmask, S3C2410_INTPND);
	}
}

static inline void
s3c_irqsub_ack(unsigned int irqno, unsigned int parentmask, unsigned int group)
{
	unsigned int bit = 1UL << (irqno - IRQ_S3CUART_RX0);

	__raw_writel(bit, S3C2410_SUBSRCPND);

	/* only ack parent if we've got all the irqs (seems we must
	 * ack, all and hope that the irq system retriggers ok when
	 * the interrupt goes off again)
	 */

	if (1) {
		__raw_writel(parentmask, S3C2410_SRCPND);
		__raw_writel(parentmask, S3C2410_INTPND);
	}
}

/* exported for use in arch/arm/mach-s3c2410 */

#ifdef CONFIG_PM
extern int s3c_irq_wake(unsigned int irqno, unsigned int state);
#else
#define s3c_irq_wake NULL
#endif

extern int s3c_irqext_type(unsigned int irq, unsigned int type);
