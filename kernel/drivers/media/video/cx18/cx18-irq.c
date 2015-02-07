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
 *  cx18 interrupt handling
 *
 *  Copyright (C) 2007  Hans Verkuil <hverkuil@xs4all.nl>
 *  Copyright (C) 2008  Andy Walls <awalls@radix.net>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *  02111-1307  USA
 */

#include "cx18-driver.h"
#include "cx18-io.h"
#include "cx18-irq.h"
#include "cx18-mailbox.h"
#include "cx18-scb.h"

static void xpu_ack(struct cx18 *cx, u32 sw2)
{
	if (sw2 & IRQ_CPU_TO_EPU_ACK)
		wake_up(&cx->mb_cpu_waitq);
	if (sw2 & IRQ_APU_TO_EPU_ACK)
		wake_up(&cx->mb_apu_waitq);
}

static void epu_cmd(struct cx18 *cx, u32 sw1)
{
	if (sw1 & IRQ_CPU_TO_EPU)
		cx18_api_epu_cmd_irq(cx, CPU);
	if (sw1 & IRQ_APU_TO_EPU)
		cx18_api_epu_cmd_irq(cx, APU);
}

irqreturn_t cx18_irq_handler(int irq, void *dev_id)
{
	struct cx18 *cx = (struct cx18 *)dev_id;
	u32 sw1, sw2, hw2;

	sw1 = cx18_read_reg(cx, SW1_INT_STATUS) & cx->sw1_irq_mask;
	sw2 = cx18_read_reg(cx, SW2_INT_STATUS) & cx->sw2_irq_mask;
	hw2 = cx18_read_reg(cx, HW2_INT_CLR_STATUS) & cx->hw2_irq_mask;

	if (sw1)
		cx18_write_reg_expect(cx, sw1, SW1_INT_STATUS, ~sw1, sw1);
	if (sw2)
		cx18_write_reg_expect(cx, sw2, SW2_INT_STATUS, ~sw2, sw2);
	if (hw2)
		cx18_write_reg_expect(cx, hw2, HW2_INT_CLR_STATUS, ~hw2, hw2);

	if (sw1 || sw2 || hw2)
		CX18_DEBUG_HI_IRQ("received interrupts "
				  "SW1: %x  SW2: %x  HW2: %x\n", sw1, sw2, hw2);

	/*
	 * SW1 responses have to happen first.  The sending XPU times out the
	 * incoming mailboxes on us rather rapidly.
	 */
	if (sw1)
		epu_cmd(cx, sw1);

	/* To do: interrupt-based I2C handling
	if (hw2 & (HW2_I2C1_INT|HW2_I2C2_INT)) {
	}
	*/

	if (sw2)
		xpu_ack(cx, sw2);

	return (sw1 || sw2 || hw2) ? IRQ_HANDLED : IRQ_NONE;
}
