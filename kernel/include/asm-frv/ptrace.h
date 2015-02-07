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

/* ptrace.h: ptrace() relevant definitions
 *
 * Copyright (C) 2003 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */
#ifndef _ASM_PTRACE_H
#define _ASM_PTRACE_H

#include <asm/registers.h>
#ifdef __KERNEL__
#include <asm/irq_regs.h>

#define in_syscall(regs) (((regs)->tbr & TBR_TT) == TBR_TT_TRAP0)
#endif


#define PT_PSR		0
#define	PT_ISR		1
#define PT_CCR		2
#define PT_CCCR		3
#define PT_LR		4
#define PT_LCR		5
#define PT_PC		6

#define PT__STATUS	7	/* exception status */
#define PT_SYSCALLNO	8	/* syscall number or -1 */
#define PT_ORIG_GR8	9	/* saved GR8 for signal handling */
#define PT_GNER0	10
#define PT_GNER1	11
#define PT_IACC0H	12
#define PT_IACC0L	13

#define PT_GR(j)	( 14 + (j))	/* GRj for 0<=j<=63 */
#define PT_FR(j)	( 78 + (j))	/* FRj for 0<=j<=63 */
#define PT_FNER(j)	(142 + (j))	/* FNERj for 0<=j<=1 */
#define PT_MSR(j)	(144 + (j))	/* MSRj for 0<=j<=2 */
#define PT_ACC(j)	(146 + (j))	/* ACCj for 0<=j<=7 */
#define PT_ACCG(jklm)	(154 + (jklm))	/* ACCGjklm for 0<=jklm<=1 (reads four regs per slot) */
#define PT_FSR(j)	(156 + (j))	/* FSRj for 0<=j<=0 */
#define PT__GPEND	78
#define PT__END		157

#define PT_TBR		PT_GR(0)
#define PT_SP		PT_GR(1)
#define PT_FP		PT_GR(2)
#define PT_PREV_FRAME	PT_GR(28)	/* previous exception frame pointer (old gr28 value) */
#define PT_CURR_TASK	PT_GR(29)	/* current task */


/* Arbitrarily choose the same ptrace numbers as used by the Sparc code. */
#define PTRACE_GETREGS		12
#define PTRACE_SETREGS		13
#define PTRACE_GETFPREGS	14
#define PTRACE_SETFPREGS	15
#define PTRACE_GETFDPIC		31	/* get the ELF fdpic loadmap address */

#define PTRACE_GETFDPIC_EXEC	0	/* [addr] request the executable loadmap */
#define PTRACE_GETFDPIC_INTERP	1	/* [addr] request the interpreter loadmap */

#ifdef __KERNEL__
#ifndef __ASSEMBLY__

/*
 * we dedicate GR28 to keeping a pointer to the current exception frame
 * - gr28 is destroyed on entry to the kernel from userspace
 */
register struct pt_regs *__frame asm("gr28");

#define user_mode(regs)			(!((regs)->psr & PSR_S))
#define instruction_pointer(regs)	((regs)->pc)

extern unsigned long user_stack(const struct pt_regs *);
extern void show_regs(struct pt_regs *);
#define profile_pc(regs) ((regs)->pc)
#endif

#endif /* !__ASSEMBLY__ */
#endif /* _ASM_PTRACE_H */
