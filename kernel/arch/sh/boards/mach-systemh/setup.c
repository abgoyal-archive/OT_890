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
 * linux/arch/sh/boards/renesas/systemh/setup.c
 *
 * Copyright (C) 2000  Kazumoto Kojima
 * Copyright (C) 2003  Paul Mundt
 *
 * Hitachi SystemH Support.
 *
 * Modified for 7751 SystemH by Jonathan Short.
 *
 * Rewritten for 2.6 by Paul Mundt.
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */
#include <linux/init.h>
#include <asm/machvec.h>
#include <mach/systemh7751.h>

extern void make_systemh_irq(unsigned int irq);

/*
 * Initialize IRQ setting
 */
static void __init sh7751systemh_init_irq(void)
{
	make_systemh_irq(0xb);	/* Ethernet interrupt */
}

static struct sh_machine_vector mv_7751systemh __initmv = {
	.mv_name		= "7751 SystemH",
	.mv_nr_irqs		= 72,

	.mv_inb			= sh7751systemh_inb,
	.mv_inw			= sh7751systemh_inw,
	.mv_inl			= sh7751systemh_inl,
	.mv_outb		= sh7751systemh_outb,
	.mv_outw		= sh7751systemh_outw,
	.mv_outl		= sh7751systemh_outl,

	.mv_inb_p		= sh7751systemh_inb_p,
	.mv_inw_p		= sh7751systemh_inw,
	.mv_inl_p		= sh7751systemh_inl,
	.mv_outb_p		= sh7751systemh_outb_p,
	.mv_outw_p		= sh7751systemh_outw,
	.mv_outl_p		= sh7751systemh_outl,

	.mv_insb		= sh7751systemh_insb,
	.mv_insw		= sh7751systemh_insw,
	.mv_insl		= sh7751systemh_insl,
	.mv_outsb		= sh7751systemh_outsb,
	.mv_outsw		= sh7751systemh_outsw,
	.mv_outsl		= sh7751systemh_outsl,

	.mv_init_irq		= sh7751systemh_init_irq,
};
