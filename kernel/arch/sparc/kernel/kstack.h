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

#ifndef _KSTACK_H
#define _KSTACK_H

#include <linux/thread_info.h>
#include <linux/sched.h>
#include <asm/ptrace.h>
#include <asm/irq.h>

/* SP must be STACK_BIAS adjusted already.  */
static inline bool kstack_valid(struct thread_info *tp, unsigned long sp)
{
	unsigned long base = (unsigned long) tp;

	if (sp >= (base + sizeof(struct thread_info)) &&
	    sp <= (base + THREAD_SIZE - sizeof(struct sparc_stackf)))
		return true;

	if (hardirq_stack[tp->cpu]) {
		base = (unsigned long) hardirq_stack[tp->cpu];
		if (sp >= base &&
		    sp <= (base + THREAD_SIZE - sizeof(struct sparc_stackf)))
			return true;
		base = (unsigned long) softirq_stack[tp->cpu];
		if (sp >= base &&
		    sp <= (base + THREAD_SIZE - sizeof(struct sparc_stackf)))
			return true;
	}
	return false;
}

/* Does "regs" point to a valid pt_regs trap frame?  */
static inline bool kstack_is_trap_frame(struct thread_info *tp, struct pt_regs *regs)
{
	unsigned long base = (unsigned long) tp;
	unsigned long addr = (unsigned long) regs;

	if (addr >= base &&
	    addr <= (base + THREAD_SIZE - sizeof(*regs)))
		goto check_magic;

	if (hardirq_stack[tp->cpu]) {
		base = (unsigned long) hardirq_stack[tp->cpu];
		if (addr >= base &&
		    addr <= (base + THREAD_SIZE - sizeof(*regs)))
			goto check_magic;
		base = (unsigned long) softirq_stack[tp->cpu];
		if (addr >= base &&
		    addr <= (base + THREAD_SIZE - sizeof(*regs)))
			goto check_magic;
	}
	return false;

check_magic:
	if ((regs->magic & ~0x1ff) == PT_REGS_MAGIC)
		return true;
	return false;

}

#endif /* _KSTACK_H */
