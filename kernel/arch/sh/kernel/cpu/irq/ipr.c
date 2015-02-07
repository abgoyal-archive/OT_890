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
 * Interrupt handling for IPR-based IRQ.
 *
 * Copyright (C) 1999  Niibe Yutaka & Takeshi Yaegashi
 * Copyright (C) 2000  Kazumoto Kojima
 * Copyright (C) 2003  Takashi Kusuda <kusuda-takashi@hitachi-ul.co.jp>
 * Copyright (C) 2006  Paul Mundt
 *
 * Supported system:
 *	On-chip supporting modules (TMU, RTC, etc.).
 *	On-chip supporting modules for SH7709/SH7709A/SH7729.
 *	Hitachi SolutionEngine external I/O:
 *		MS7709SE01, MS7709ASE01, and MS7750SE01
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */
#include <linux/init.h>
#include <linux/irq.h>
#include <linux/module.h>
#include <linux/io.h>
#include <linux/interrupt.h>

static inline struct ipr_desc *get_ipr_desc(unsigned int irq)
{
	struct irq_chip *chip = get_irq_chip(irq);
	return (void *)((char *)chip - offsetof(struct ipr_desc, chip));
}

static void disable_ipr_irq(unsigned int irq)
{
	struct ipr_data *p = get_irq_chip_data(irq);
	unsigned long addr = get_ipr_desc(irq)->ipr_offsets[p->ipr_idx];
	/* Set the priority in IPR to 0 */
	__raw_writew(__raw_readw(addr) & (0xffff ^ (0xf << p->shift)), addr);
}

static void enable_ipr_irq(unsigned int irq)
{
	struct ipr_data *p = get_irq_chip_data(irq);
	unsigned long addr = get_ipr_desc(irq)->ipr_offsets[p->ipr_idx];
	/* Set priority in IPR back to original value */
	__raw_writew(__raw_readw(addr) | (p->priority << p->shift), addr);
}

/*
 * The shift value is now the number of bits to shift, not the number of
 * bits/4. This is to make it easier to read the value directly from the
 * datasheets. The IPR address is calculated using the ipr_offset table.
 */
void register_ipr_controller(struct ipr_desc *desc)
{
	int i;

	desc->chip.mask = disable_ipr_irq;
	desc->chip.unmask = enable_ipr_irq;
	desc->chip.mask_ack = disable_ipr_irq;

	for (i = 0; i < desc->nr_irqs; i++) {
		struct ipr_data *p = desc->ipr_data + i;

		BUG_ON(p->ipr_idx >= desc->nr_offsets);
		BUG_ON(!desc->ipr_offsets[p->ipr_idx]);

		disable_irq_nosync(p->irq);
		set_irq_chip_and_handler_name(p->irq, &desc->chip,
				      handle_level_irq, "level");
		set_irq_chip_data(p->irq, p);
		disable_ipr_irq(p->irq);
	}
}
EXPORT_SYMBOL(register_ipr_controller);
