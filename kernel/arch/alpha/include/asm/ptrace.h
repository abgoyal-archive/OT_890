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

#ifndef _ASMAXP_PTRACE_H
#define _ASMAXP_PTRACE_H


/*
 * This struct defines the way the registers are stored on the
 * kernel stack during a system call or other kernel entry
 *
 * NOTE! I want to minimize the overhead of system calls, so this
 * struct has as little information as possible.  I does not have
 *
 *  - floating point regs: the kernel doesn't change those
 *  - r9-15: saved by the C compiler
 *
 * This makes "fork()" and "exec()" a bit more complex, but should
 * give us low system call latency.
 */

struct pt_regs {
	unsigned long r0;
	unsigned long r1;
	unsigned long r2;
	unsigned long r3;
	unsigned long r4;
	unsigned long r5;
	unsigned long r6;
	unsigned long r7;
	unsigned long r8;
	unsigned long r19;
	unsigned long r20;
	unsigned long r21;
	unsigned long r22;
	unsigned long r23;
	unsigned long r24;
	unsigned long r25;
	unsigned long r26;
	unsigned long r27;
	unsigned long r28;
	unsigned long hae;
/* JRP - These are the values provided to a0-a2 by PALcode */
	unsigned long trap_a0;
	unsigned long trap_a1;
	unsigned long trap_a2;
/* These are saved by PAL-code: */
	unsigned long ps;
	unsigned long pc;
	unsigned long gp;
	unsigned long r16;
	unsigned long r17;
	unsigned long r18;
};

/*
 * This is the extended stack used by signal handlers and the context
 * switcher: it's pushed after the normal "struct pt_regs".
 */
struct switch_stack {
	unsigned long r9;
	unsigned long r10;
	unsigned long r11;
	unsigned long r12;
	unsigned long r13;
	unsigned long r14;
	unsigned long r15;
	unsigned long r26;
	unsigned long fp[32];	/* fp[31] is fpcr */
};

#ifdef __KERNEL__

#define user_mode(regs) (((regs)->ps & 8) != 0)
#define instruction_pointer(regs) ((regs)->pc)
#define profile_pc(regs) instruction_pointer(regs)
extern void show_regs(struct pt_regs *);

#define task_pt_regs(task) \
  ((struct pt_regs *) (task_stack_page(task) + 2*PAGE_SIZE) - 1)

#define force_successful_syscall_return() (task_pt_regs(current)->r0 = 0)

#endif

#endif
