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
 * linux/arch/sh/boards/se/7780/irq.c
 *
 * Copyright (C) 2006,2007  Nobuhiro Iwamatsu
 *
 * Hitachi UL SolutionEngine 7780 Support.
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */
#include <linux/init.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <mach-se/mach/se7780.h>

/*
 * Initialize IRQ setting
 */
void __init init_se7780_IRQ(void)
{
	/* enable all interrupt at FPGA */
	ctrl_outw(0, FPGA_INTMSK1);
	/* mask SM501 interrupt */
	ctrl_outw((ctrl_inw(FPGA_INTMSK1) | 0x0002), FPGA_INTMSK1);
	/* enable all interrupt at FPGA */
	ctrl_outw(0, FPGA_INTMSK2);

	/* set FPGA INTSEL register */
	/* FPGA + 0x06 */
	ctrl_outw( ((IRQPIN_SM501 << IRQPOS_SM501) |
		(IRQPIN_SMC91CX << IRQPOS_SMC91CX)), FPGA_INTSEL1);

	/* FPGA + 0x08 */
	ctrl_outw(((IRQPIN_EXTINT4 << IRQPOS_EXTINT4) |
		(IRQPIN_EXTINT3 << IRQPOS_EXTINT3) |
		(IRQPIN_EXTINT2 << IRQPOS_EXTINT2) |
		(IRQPIN_EXTINT1 << IRQPOS_EXTINT1)), FPGA_INTSEL2);

	/* FPGA + 0x0A */
	ctrl_outw((IRQPIN_PCCPW << IRQPOS_PCCPW), FPGA_INTSEL3);

	plat_irq_setup_pins(IRQ_MODE_IRQ); /* install handlers for IRQ0-7 */
}
