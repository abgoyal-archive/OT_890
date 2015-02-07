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
 * linux/arch/sh/boards/renesas/systemh/irq.c
 *
 * Copyright (C) 2000  Kazumoto Kojima
 *
 * Hitachi SystemH Support.
 *
 * Modified for 7751 SystemH by
 * Jonathan Short.
 */

#include <linux/init.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/io.h>

#include <mach/systemh7751.h>
#include <asm/smc37c93x.h>

/* address of external interrupt mask register
 * address must be set prior to use these (maybe in init_XXX_irq())
 * XXX : is it better to use .config than specifying it in code? */
static unsigned long *systemh_irq_mask_register = (unsigned long *)0xB3F10004;
static unsigned long *systemh_irq_request_register = (unsigned long *)0xB3F10000;

/* forward declaration */
static void enable_systemh_irq(unsigned int irq);
static void disable_systemh_irq(unsigned int irq);
static void mask_and_ack_systemh(unsigned int);

static struct irq_chip systemh_irq_type = {
	.name = " SystemH Register",
	.unmask = enable_systemh_irq,
	.mask = disable_systemh_irq,
	.ack = mask_and_ack_systemh,
};

static void disable_systemh_irq(unsigned int irq)
{
	if (systemh_irq_mask_register) {
		unsigned long val, mask = 0x01 << 1;

		/* Clear the "irq"th bit in the mask and set it in the request */
		val = ctrl_inl((unsigned long)systemh_irq_mask_register);
		val &= ~mask;
		ctrl_outl(val, (unsigned long)systemh_irq_mask_register);

		val = ctrl_inl((unsigned long)systemh_irq_request_register);
		val |= mask;
		ctrl_outl(val, (unsigned long)systemh_irq_request_register);
	}
}

static void enable_systemh_irq(unsigned int irq)
{
	if (systemh_irq_mask_register) {
		unsigned long val, mask = 0x01 << 1;

		/* Set "irq"th bit in the mask register */
		val = ctrl_inl((unsigned long)systemh_irq_mask_register);
		val |= mask;
		ctrl_outl(val, (unsigned long)systemh_irq_mask_register);
	}
}

static void mask_and_ack_systemh(unsigned int irq)
{
	disable_systemh_irq(irq);
}

void make_systemh_irq(unsigned int irq)
{
	disable_irq_nosync(irq);
	set_irq_chip_and_handler(irq, &systemh_irq_type, handle_level_irq);
	disable_systemh_irq(irq);
}
