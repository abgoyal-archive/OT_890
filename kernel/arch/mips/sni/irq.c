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
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1992 Linus Torvalds
 * Copyright (C) 1994 - 2000 Ralf Baechle
 * Copyright (C) 2006 Thomas Bogendoerfer
 */
#include <linux/delay.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/kernel.h>

#include <asm/i8259.h>
#include <asm/io.h>
#include <asm/sni.h>
#include <asm/irq.h>
#include <asm/irq_cpu.h>

void (*sni_hwint)(void);

asmlinkage void plat_irq_dispatch(void)
{
	sni_hwint();
}

/* ISA irq handler */
static irqreturn_t sni_isa_irq_handler(int dummy, void *p)
{
	int irq;

	irq = i8259_irq();
	if (unlikely(irq < 0))
		return IRQ_NONE;

	generic_handle_irq(irq);
	return IRQ_HANDLED;
}

struct irqaction sni_isa_irq = {
	.handler = sni_isa_irq_handler,
	.name = "ISA",
	.flags = IRQF_SHARED | IRQF_DISABLED
};

/*
 * On systems with i8259-style interrupt controllers we assume for
 * driver compatibility reasons interrupts 0 - 15 to be the i8295
 * interrupts even if the hardware uses a different interrupt numbering.
 */
void __init arch_init_irq(void)
{
	init_i8259_irqs();			/* Integrated i8259  */
	switch (sni_brd_type) {
	case SNI_BRD_10:
	case SNI_BRD_10NEW:
	case SNI_BRD_TOWER_OASIC:
	case SNI_BRD_MINITOWER:
	        sni_a20r_irq_init();
	        break;

	case SNI_BRD_PCI_TOWER:
	        sni_pcit_irq_init();
	        break;

	case SNI_BRD_PCI_TOWER_CPLUS:
	        sni_pcit_cplus_irq_init();
	        break;

	case SNI_BRD_RM200:
	        sni_rm200_irq_init();
	        break;

	case SNI_BRD_PCI_MTOWER:
	case SNI_BRD_PCI_DESKTOP:
	case SNI_BRD_PCI_MTOWER_CPLUS:
	        sni_pcimt_irq_init();
	        break;
	}
}
