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

#ifndef __ASM_SH_IRQ_H
#define __ASM_SH_IRQ_H

#include <asm/machvec.h>

/*
 * A sane default based on a reasonable vector table size, platforms are
 * advised to cap this at the hard limit that they're interested in
 * through the machvec.
 */
#define NR_IRQS 256

/*
 * Convert back and forth between INTEVT and IRQ values.
 */
#ifdef CONFIG_CPU_HAS_INTEVT
#define evt2irq(evt)		(((evt) >> 5) - 16)
#define irq2evt(irq)		(((irq) + 16) << 5)
#else
#define evt2irq(evt)		(evt)
#define irq2evt(irq)		(irq)
#endif

/*
 * Simple Mask Register Support
 */
extern void make_maskreg_irq(unsigned int irq);
extern unsigned short *irq_mask_register;

/*
 * PINT IRQs
 */
void init_IRQ_pint(void);
void make_imask_irq(unsigned int irq);

static inline int generic_irq_demux(int irq)
{
	return irq;
}

#define irq_canonicalize(irq)	(irq)
#define irq_demux(irq)		sh_mv.mv_irq_demux(irq)

void init_IRQ(void);
asmlinkage int do_IRQ(unsigned int irq, struct pt_regs *regs);

#ifdef CONFIG_IRQSTACKS
extern void irq_ctx_init(int cpu);
extern void irq_ctx_exit(int cpu);
# define __ARCH_HAS_DO_SOFTIRQ
#else
# define irq_ctx_init(cpu) do { } while (0)
# define irq_ctx_exit(cpu) do { } while (0)
#endif

#ifdef CONFIG_CPU_SH5
#include <cpu/irq.h>
#endif

#endif /* __ASM_SH_IRQ_H */
