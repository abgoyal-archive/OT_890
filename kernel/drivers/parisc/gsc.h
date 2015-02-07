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
 * drivers/parisc/gsc.h
 * Declarations for functions in gsc.c
 * Copyright (c) 2000-2002 Helge Deller, Matthew Wilcox
 *
 * Distributed under the terms of the GPL, version 2
 */

#include <linux/interrupt.h>
#include <asm/hardware.h>
#include <asm/parisc-device.h>

#define OFFSET_IRR 0x0000   /* Interrupt request register */
#define OFFSET_IMR 0x0004   /* Interrupt mask register */
#define OFFSET_IPR 0x0008   /* Interrupt pending register */
#define OFFSET_ICR 0x000C   /* Interrupt control register */
#define OFFSET_IAR 0x0010   /* Interrupt address register */

/* PA I/O Architected devices support at least 5 bits in the EIM register. */
#define GSC_EIM_WIDTH 5

struct gsc_irq {
	unsigned long txn_addr;	/* IRQ "target" */
	int txn_data;		/* HW "IRQ" */
	int irq;		/* virtual IRQ */
};

struct gsc_asic {
	struct parisc_device *gsc;
	unsigned long hpa;
	char *name;
	int version;
	int type;
	int eim;
	int global_irq[32];
};

int gsc_common_setup(struct parisc_device *parent, struct gsc_asic *gsc_asic);
int gsc_alloc_irq(struct gsc_irq *dev);			/* dev needs an irq */
int gsc_claim_irq(struct gsc_irq *dev, int irq);	/* dev needs this irq */
int gsc_assign_irq(struct hw_interrupt_type *type, void *data);
int gsc_find_local_irq(unsigned int irq, int *global_irq, int limit);
void gsc_fixup_irqs(struct parisc_device *parent, void *ctrl,
		void (*choose)(struct parisc_device *child, void *ctrl));
void gsc_asic_assign_irq(struct gsc_asic *asic, int local_irq, int *irqp);

irqreturn_t gsc_asic_intr(int irq, void *dev);
