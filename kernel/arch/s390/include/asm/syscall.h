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
 * Access to user system call parameters and results
 *
 *  Copyright IBM Corp. 2008
 *  Author(s): Martin Schwidefsky (schwidefsky@de.ibm.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License (version 2 only)
 * as published by the Free Software Foundation.
 */

#ifndef _ASM_SYSCALL_H
#define _ASM_SYSCALL_H	1

#include <asm/ptrace.h>

static inline long syscall_get_nr(struct task_struct *task,
				  struct pt_regs *regs)
{
	return regs->svcnr ? regs->svcnr : -1;
}

static inline void syscall_rollback(struct task_struct *task,
				    struct pt_regs *regs)
{
	regs->gprs[2] = regs->orig_gpr2;
}

static inline long syscall_get_error(struct task_struct *task,
				     struct pt_regs *regs)
{
	return (regs->gprs[2] >= -4096UL) ? -regs->gprs[2] : 0;
}

static inline long syscall_get_return_value(struct task_struct *task,
					    struct pt_regs *regs)
{
	return regs->gprs[2];
}

static inline void syscall_set_return_value(struct task_struct *task,
					    struct pt_regs *regs,
					    int error, long val)
{
	regs->gprs[2] = error ? -error : val;
}

static inline void syscall_get_arguments(struct task_struct *task,
					 struct pt_regs *regs,
					 unsigned int i, unsigned int n,
					 unsigned long *args)
{
	unsigned long mask = -1UL;

	BUG_ON(i + n > 6);
#ifdef CONFIG_COMPAT
	if (test_tsk_thread_flag(task, TIF_31BIT))
		mask = 0xffffffff;
#endif
	if (i + n == 6)
		args[--n] = regs->args[0] & mask;
	while (n-- > 0)
		if (i + n > 0)
			args[n] = regs->gprs[2 + i + n] & mask;
	if (i == 0)
		args[0] = regs->orig_gpr2 & mask;
}

static inline void syscall_set_arguments(struct task_struct *task,
					 struct pt_regs *regs,
					 unsigned int i, unsigned int n,
					 const unsigned long *args)
{
	BUG_ON(i + n > 6);
	if (i + n == 6)
		regs->args[0] = args[--n];
	while (n-- > 0)
		if (i + n > 0)
			regs->gprs[2 + i + n] = args[n];
	if (i == 0)
		regs->orig_gpr2 = args[0];
}

#endif	/* _ASM_SYSCALL_H */
