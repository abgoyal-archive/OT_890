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
 * arch/arm/include/asm/kprobes.h
 *
 * Copyright (C) 2006, 2007 Motorola Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 */

#ifndef _ARM_KPROBES_H
#define _ARM_KPROBES_H

#include <linux/types.h>
#include <linux/ptrace.h>
#include <linux/percpu.h>

#define __ARCH_WANT_KPROBES_INSN_SLOT
#define MAX_INSN_SIZE			2
#define MAX_STACK_SIZE			64	/* 32 would probably be OK */

/*
 * This undefined instruction must be unique and
 * reserved solely for kprobes' use.
 */
#define KPROBE_BREAKPOINT_INSTRUCTION	0xe7f001f8

#define regs_return_value(regs)		((regs)->ARM_r0)
#define flush_insn_slot(p)		do { } while (0)
#define kretprobe_blacklist_size	0

typedef u32 kprobe_opcode_t;

struct kprobe;
typedef void (kprobe_insn_handler_t)(struct kprobe *, struct pt_regs *);

/* Architecture specific copy of original instruction. */
struct arch_specific_insn {
	kprobe_opcode_t		*insn;
	kprobe_insn_handler_t	*insn_handler;
};

struct prev_kprobe {
	struct kprobe *kp;
	unsigned int status;
};

/* per-cpu kprobe control block */
struct kprobe_ctlblk {
	unsigned int kprobe_status;
	struct prev_kprobe prev_kprobe;
	struct pt_regs jprobe_saved_regs;
	char jprobes_stack[MAX_STACK_SIZE];
};

void arch_remove_kprobe(struct kprobe *);
void kretprobe_trampoline(void);

int kprobe_fault_handler(struct pt_regs *regs, unsigned int fsr);
int kprobe_exceptions_notify(struct notifier_block *self,
			     unsigned long val, void *data);

enum kprobe_insn {
	INSN_REJECTED,
	INSN_GOOD,
	INSN_GOOD_NO_SLOT
};

enum kprobe_insn arm_kprobe_decode_insn(kprobe_opcode_t,
					struct arch_specific_insn *);
void __init arm_kprobe_decode_init(void);

#endif /* _ARM_KPROBES_H */
