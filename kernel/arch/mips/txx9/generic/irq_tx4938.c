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
 * linux/arch/mips/tx4938/common/irq.c
 *
 * Common tx4938 irq handler
 * Copyright (C) 2000-2001 Toshiba Corporation
 *
 * 2003-2005 (c) MontaVista Software, Inc. This file is licensed under the
 * terms of the GNU General Public License version 2. This program is
 * licensed "as is" without any warranty of any kind, whether express
 * or implied.
 *
 * Support for TX4938 in 2.6 - Manish Lachwani (mlachwani@mvista.com)
 */
#include <linux/init.h>
#include <linux/interrupt.h>
#include <asm/irq_cpu.h>
#include <asm/txx9/tx4938.h>

void __init tx4938_irq_init(void)
{
	int i;

	mips_cpu_irq_init();
	txx9_irq_init(TX4938_IRC_REG & 0xfffffffffULL);
	set_irq_chained_handler(MIPS_CPU_IRQ_BASE + TX4938_IRC_INT,
				handle_simple_irq);
	/* raise priority for errors, timers, SIO */
	txx9_irq_set_pri(TX4938_IR_ECCERR, 7);
	txx9_irq_set_pri(TX4938_IR_WTOERR, 7);
	txx9_irq_set_pri(TX4938_IR_PCIERR, 7);
	txx9_irq_set_pri(TX4938_IR_PCIPME, 7);
	for (i = 0; i < TX4938_NUM_IR_TMR; i++)
		txx9_irq_set_pri(TX4938_IR_TMR(i), 6);
	for (i = 0; i < TX4938_NUM_IR_SIO; i++)
		txx9_irq_set_pri(TX4938_IR_SIO(i), 5);
}
