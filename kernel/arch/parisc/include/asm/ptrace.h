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

#ifndef _PARISC_PTRACE_H
#define _PARISC_PTRACE_H

/* written by Philipp Rumpf, Copyright (C) 1999 SuSE GmbH Nuernberg
** Copyright (C) 2000 Grant Grundler, Hewlett-Packard
*/

#include <linux/types.h>

/* This struct defines the way the registers are stored on the 
 * stack during a system call.
 *
 * N.B. gdb/strace care about the size and offsets within this
 * structure. If you change things, you may break object compatibility
 * for those applications.
 */

struct pt_regs {
	unsigned long gr[32];	/* PSW is in gr[0] */
	__u64 fr[32];
	unsigned long sr[ 8];
	unsigned long iasq[2];
	unsigned long iaoq[2];
	unsigned long cr27;
	unsigned long pad0;     /* available for other uses */
	unsigned long orig_r28;
	unsigned long ksp;
	unsigned long kpc;
	unsigned long sar;	/* CR11 */
	unsigned long iir;	/* CR19 */
	unsigned long isr;	/* CR20 */
	unsigned long ior;	/* CR21 */
	unsigned long ipsw;	/* CR22 */
};

/*
 * The numbers chosen here are somewhat arbitrary but absolutely MUST
 * not overlap with any of the number assigned in <linux/ptrace.h>.
 *
 * These ones are taken from IA-64 on the assumption that theirs are
 * the most correct (and we also want to support PTRACE_SINGLEBLOCK
 * since we have taken branch traps too)
 */
#define PTRACE_SINGLEBLOCK	12	/* resume execution until next branch */

#ifdef __KERNEL__

#define task_regs(task) ((struct pt_regs *) ((char *)(task) + TASK_REGS))

struct task_struct;
#define arch_has_single_step()	1
void user_disable_single_step(struct task_struct *task);
void user_enable_single_step(struct task_struct *task);

#define arch_has_block_step()	1
void user_enable_block_step(struct task_struct *task);

/* XXX should we use iaoq[1] or iaoq[0] ? */
#define user_mode(regs)			(((regs)->iaoq[0] & 3) ? 1 : 0)
#define user_space(regs)		(((regs)->iasq[1] != 0) ? 1 : 0)
#define instruction_pointer(regs)	((regs)->iaoq[0] & ~3)
unsigned long profile_pc(struct pt_regs *);
extern void show_regs(struct pt_regs *);
#endif

#endif
