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
 * Copyright (C) 2000 - 2007 Jeff Dike (jdike@{addtoit,linux.intel}.com)
 * Licensed under the GPL
 */

#ifndef __UM_PTRACE_GENERIC_H
#define __UM_PTRACE_GENERIC_H

#ifndef __ASSEMBLY__

#include <asm/ptrace-abi.h>
#include <asm/user.h>
#include "sysdep/ptrace.h"

struct pt_regs {
	struct uml_pt_regs regs;
};

#define EMPTY_REGS { .regs = EMPTY_UML_PT_REGS }

#define PT_REGS_IP(r) UPT_IP(&(r)->regs)
#define PT_REGS_SP(r) UPT_SP(&(r)->regs)

#define PT_REG(r, reg) UPT_REG(&(r)->regs, reg)
#define PT_REGS_SET(r, reg, val) UPT_SET(&(r)->regs, reg, val)

#define PT_REGS_SET_SYSCALL_RETURN(r, res) \
	UPT_SET_SYSCALL_RETURN(&(r)->regs, res)
#define PT_REGS_RESTART_SYSCALL(r) UPT_RESTART_SYSCALL(&(r)->regs)

#define PT_REGS_SYSCALL_NR(r) UPT_SYSCALL_NR(&(r)->regs)

#define PT_REGS_SC(r) UPT_SC(&(r)->regs)

#define instruction_pointer(regs) PT_REGS_IP(regs)

struct task_struct;

extern long subarch_ptrace(struct task_struct *child, long request, long addr,
			   long data);
extern unsigned long getreg(struct task_struct *child, int regno);
extern int putreg(struct task_struct *child, int regno, unsigned long value);
extern int get_fpregs(struct user_i387_struct __user *buf,
		      struct task_struct *child);
extern int set_fpregs(struct user_i387_struct __user *buf,
		      struct task_struct *child);

extern void show_regs(struct pt_regs *regs);

extern int arch_copy_tls(struct task_struct *new);
extern void clear_flushed_tls(struct task_struct *task);

#endif

#endif
