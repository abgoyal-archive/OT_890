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

/* MN10300 FPU definitions
 *
 * Copyright (C) 2007 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 * Derived from include/asm-i386/i387.h: Copyright (C) 1994 Linus Torvalds
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version
 * 2 of the Licence, or (at your option) any later version.
 */
#ifndef _ASM_FPU_H
#define _ASM_FPU_H

#include <asm/processor.h>
#include <asm/sigcontext.h>
#include <asm/user.h>

#ifdef __KERNEL__

/* the task that owns the FPU state */
extern struct task_struct *fpu_state_owner;

#define set_using_fpu(tsk)				\
do {							\
	(tsk)->thread.fpu_flags |= THREAD_USING_FPU;	\
} while (0)

#define clear_using_fpu(tsk)				\
do {							\
	(tsk)->thread.fpu_flags &= ~THREAD_USING_FPU;	\
} while (0)

#define is_using_fpu(tsk) ((tsk)->thread.fpu_flags & THREAD_USING_FPU)

#define unlazy_fpu(tsk)					\
do {							\
	preempt_disable();				\
	if (fpu_state_owner == (tsk))			\
		fpu_save(&tsk->thread.fpu_state);	\
	preempt_enable();				\
} while (0)

#define exit_fpu()				\
do {						\
	struct task_struct *__tsk = current;	\
	preempt_disable();			\
	if (fpu_state_owner == __tsk)		\
		fpu_state_owner = NULL;		\
	preempt_enable();			\
} while (0)

#define flush_fpu()					\
do {							\
	struct task_struct *__tsk = current;		\
	preempt_disable();				\
	if (fpu_state_owner == __tsk) {			\
		fpu_state_owner = NULL;			\
		__tsk->thread.uregs->epsw &= ~EPSW_FE;	\
	}						\
	preempt_enable();				\
	clear_using_fpu(__tsk);				\
} while (0)

extern asmlinkage void fpu_init_state(void);
extern asmlinkage void fpu_kill_state(struct task_struct *);
extern asmlinkage void fpu_disabled(struct pt_regs *, enum exception_code);
extern asmlinkage void fpu_exception(struct pt_regs *, enum exception_code);

#ifdef CONFIG_FPU
extern asmlinkage void fpu_save(struct fpu_state_struct *);
extern asmlinkage void fpu_restore(struct fpu_state_struct *);
#else
#define fpu_save(a)
#define fpu_restore(a)
#endif /* CONFIG_FPU  */

/*
 * signal frame handlers
 */
extern int fpu_setup_sigcontext(struct fpucontext *buf);
extern int fpu_restore_sigcontext(struct fpucontext *buf);

#endif /* __KERNEL__ */
#endif /* _ASM_FPU_H */
