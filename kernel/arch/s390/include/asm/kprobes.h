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

#ifndef _ASM_S390_KPROBES_H
#define _ASM_S390_KPROBES_H
/*
 *  Kernel Probes (KProbes)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 * Copyright (C) IBM Corporation, 2002, 2006
 *
 * 2002-Oct	Created by Vamsi Krishna S <vamsi_krishna@in.ibm.com> Kernel
 *		Probes initial implementation ( includes suggestions from
 *		Rusty Russell).
 * 2004-Nov	Modified for PPC64 by Ananth N Mavinakayanahalli
 *		<ananth@in.ibm.com>
 * 2005-Dec	Used as a template for s390 by Mike Grundy
 *		<grundym@us.ibm.com>
 */
#include <linux/types.h>
#include <linux/ptrace.h>
#include <linux/percpu.h>

#define  __ARCH_WANT_KPROBES_INSN_SLOT
struct pt_regs;
struct kprobe;

typedef u16 kprobe_opcode_t;
#define BREAKPOINT_INSTRUCTION	0x0002

/* Maximum instruction size is 3 (16bit) halfwords: */
#define MAX_INSN_SIZE		0x0003
#define MAX_STACK_SIZE		64
#define MIN_STACK_SIZE(ADDR) (((MAX_STACK_SIZE) < \
	(((unsigned long)current_thread_info()) + THREAD_SIZE - (ADDR))) \
	? (MAX_STACK_SIZE) \
	: (((unsigned long)current_thread_info()) + THREAD_SIZE - (ADDR)))

#define kretprobe_blacklist_size 0

#define KPROBE_SWAP_INST	0x10

#define FIXUP_PSW_NORMAL	0x08
#define FIXUP_BRANCH_NOT_TAKEN	0x04
#define FIXUP_RETURN_REGISTER	0x02
#define FIXUP_NOT_REQUIRED	0x01

/* Architecture specific copy of original instruction */
struct arch_specific_insn {
	/* copy of original instruction */
	kprobe_opcode_t *insn;
	int fixup;
	int ilen;
	int reg;
};

struct ins_replace_args {
	kprobe_opcode_t *ptr;
	kprobe_opcode_t old;
	kprobe_opcode_t new;
};
struct prev_kprobe {
	struct kprobe *kp;
	unsigned long status;
	unsigned long saved_psw;
	unsigned long kprobe_saved_imask;
	unsigned long kprobe_saved_ctl[3];
};

/* per-cpu kprobe control block */
struct kprobe_ctlblk {
	unsigned long kprobe_status;
	unsigned long kprobe_saved_imask;
	unsigned long kprobe_saved_ctl[3];
	struct pt_regs jprobe_saved_regs;
	unsigned long jprobe_saved_r14;
	unsigned long jprobe_saved_r15;
	struct prev_kprobe prev_kprobe;
	kprobe_opcode_t jprobes_stack[MAX_STACK_SIZE];
};

void arch_remove_kprobe(struct kprobe *p);
void kretprobe_trampoline(void);
int  is_prohibited_opcode(kprobe_opcode_t *instruction);
void get_instruction_type(struct arch_specific_insn *ainsn);

int kprobe_fault_handler(struct pt_regs *regs, int trapnr);
int kprobe_exceptions_notify(struct notifier_block *self,
	unsigned long val, void *data);

#define flush_insn_slot(p)	do { } while (0)

#endif	/* _ASM_S390_KPROBES_H */
