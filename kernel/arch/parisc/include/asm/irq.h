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
 * include/asm-parisc/irq.h
 *
 * Copyright 2005 Matthew Wilcox <matthew@wil.cx>
 */

#ifndef _ASM_PARISC_IRQ_H
#define _ASM_PARISC_IRQ_H

#include <linux/cpumask.h>
#include <asm/types.h>

#define NO_IRQ		(-1)

#ifdef CONFIG_GSC
#define GSC_IRQ_BASE	16
#define GSC_IRQ_MAX	63
#define CPU_IRQ_BASE	64
#else
#define CPU_IRQ_BASE	16
#endif

#define TIMER_IRQ	(CPU_IRQ_BASE + 0)
#define	IPI_IRQ		(CPU_IRQ_BASE + 1)
#define CPU_IRQ_MAX	(CPU_IRQ_BASE + (BITS_PER_LONG - 1))

#define NR_IRQS		(CPU_IRQ_MAX + 1)

static __inline__ int irq_canonicalize(int irq)
{
	return (irq == 2) ? 9 : irq;
}

struct irq_chip;

/*
 * Some useful "we don't have to do anything here" handlers.  Should
 * probably be provided by the generic code.
 */
void no_ack_irq(unsigned int irq);
void no_end_irq(unsigned int irq);
void cpu_ack_irq(unsigned int irq);
void cpu_end_irq(unsigned int irq);

extern int txn_alloc_irq(unsigned int nbits);
extern int txn_claim_irq(int);
extern unsigned int txn_alloc_data(unsigned int);
extern unsigned long txn_alloc_addr(unsigned int);
extern unsigned long txn_affinity_addr(unsigned int irq, int cpu);

extern int cpu_claim_irq(unsigned int irq, struct irq_chip *, void *);
extern int cpu_check_affinity(unsigned int irq, const struct cpumask *dest);

/* soft power switch support (power.c) */
extern struct tasklet_struct power_tasklet;

#endif	/* _ASM_PARISC_IRQ_H */
