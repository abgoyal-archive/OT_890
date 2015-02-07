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

/**
 * arch/s390/oprofile/backtrace.c
 *
 * S390 Version
 *   Copyright (C) 2005 IBM Corporation, IBM Deutschland Entwicklung GmbH.
 *   Author(s): Andreas Krebbel <Andreas.Krebbel@de.ibm.com>
 */

#include <linux/oprofile.h>

#include <asm/processor.h> /* for struct stack_frame */

static unsigned long
__show_trace(unsigned int *depth, unsigned long sp,
	     unsigned long low, unsigned long high)
{
	struct stack_frame *sf;
	struct pt_regs *regs;

	while (*depth) {
		sp = sp & PSW_ADDR_INSN;
		if (sp < low || sp > high - sizeof(*sf))
			return sp;
		sf = (struct stack_frame *) sp;
		(*depth)--;
		oprofile_add_trace(sf->gprs[8] & PSW_ADDR_INSN);

		/* Follow the backchain.  */
		while (*depth) {
			low = sp;
			sp = sf->back_chain & PSW_ADDR_INSN;
			if (!sp)
				break;
			if (sp <= low || sp > high - sizeof(*sf))
				return sp;
			sf = (struct stack_frame *) sp;
			(*depth)--;
			oprofile_add_trace(sf->gprs[8] & PSW_ADDR_INSN);

		}

		if (*depth == 0)
			break;

		/* Zero backchain detected, check for interrupt frame.  */
		sp = (unsigned long) (sf + 1);
		if (sp <= low || sp > high - sizeof(*regs))
			return sp;
		regs = (struct pt_regs *) sp;
		(*depth)--;
		oprofile_add_trace(sf->gprs[8] & PSW_ADDR_INSN);
		low = sp;
		sp = regs->gprs[15];
	}
	return sp;
}

void s390_backtrace(struct pt_regs * const regs, unsigned int depth)
{
	unsigned long head;
	struct stack_frame* head_sf;

	if (user_mode (regs))
		return;

	head = regs->gprs[15];
	head_sf = (struct stack_frame*)head;

	if (!head_sf->back_chain)
		return;

	head = head_sf->back_chain;

	head = __show_trace(&depth, head, S390_lowcore.async_stack - ASYNC_SIZE,
			    S390_lowcore.async_stack);

	__show_trace(&depth, head, S390_lowcore.thread_info,
		     S390_lowcore.thread_info + THREAD_SIZE);
}
