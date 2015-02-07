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
 * Shared SH3 Setup code
 *
 *  Copyright (C) 2008  Magnus Damm
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */

#include <linux/init.h>
#include <linux/irq.h>
#include <linux/io.h>

/* All SH3 devices are equipped with IRQ0->5 (except sh7708) */

enum {
	UNUSED = 0,

	/* interrupt sources */
	IRQ0, IRQ1, IRQ2, IRQ3, IRQ4, IRQ5,
};

static struct intc_vect vectors_irq0123[] __initdata = {
	INTC_VECT(IRQ0, 0x600), INTC_VECT(IRQ1, 0x620),
	INTC_VECT(IRQ2, 0x640), INTC_VECT(IRQ3, 0x660),
};

static struct intc_vect vectors_irq45[] __initdata = {
	INTC_VECT(IRQ4, 0x680), INTC_VECT(IRQ5, 0x6a0),
};

static struct intc_prio_reg prio_registers[] __initdata = {
	{ 0xa4000016, 0, 16, 4, /* IPRC */ { IRQ3, IRQ2, IRQ1, IRQ0 } },
	{ 0xa4000018, 0, 16, 4, /* IPRD */ { 0, 0, IRQ5, IRQ4 } },
};

static struct intc_mask_reg ack_registers[] __initdata = {
	{ 0xa4000004, 0, 8, /* IRR0 */
	  { 0, 0, IRQ5, IRQ4, IRQ3, IRQ2, IRQ1, IRQ0 } },
};

static struct intc_sense_reg sense_registers[] __initdata = {
	{ 0xa4000010, 16, 2, { 0, 0, IRQ5, IRQ4, IRQ3, IRQ2, IRQ1, IRQ0 } },
};

static DECLARE_INTC_DESC_ACK(intc_desc_irq0123, "sh3-irq0123",
			     vectors_irq0123, NULL, NULL,
			     prio_registers, sense_registers, ack_registers);

static DECLARE_INTC_DESC_ACK(intc_desc_irq45, "sh3-irq45",
			     vectors_irq45, NULL, NULL,
			     prio_registers, sense_registers, ack_registers);

#define INTC_ICR1		0xa4000010UL
#define INTC_ICR1_IRQLVL	(1<<14)

void __init plat_irq_setup_pins(int mode)
{
	if (mode == IRQ_MODE_IRQ) {
		ctrl_outw(ctrl_inw(INTC_ICR1) & ~INTC_ICR1_IRQLVL, INTC_ICR1);
		register_intc_controller(&intc_desc_irq0123);
		return;
	}
	BUG();
}

void __init plat_irq_setup_sh3(void)
{
	register_intc_controller(&intc_desc_irq45);
}
