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
 * Copyright (C) 2002 MontaVista Software Inc.
 * Author: Jun Sun, jsun@mvista.com or jsun@junsun.net
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */
#ifndef _ASM_FPU_H
#define _ASM_FPU_H

#include <linux/sched.h>
#include <linux/thread_info.h>
#include <linux/bitops.h>

#include <asm/mipsregs.h>
#include <asm/cpu.h>
#include <asm/cpu-features.h>
#include <asm/hazards.h>
#include <asm/processor.h>
#include <asm/current.h>

#ifdef CONFIG_MIPS_MT_FPAFF
#include <asm/mips_mt.h>
#endif

struct sigcontext;
struct sigcontext32;

extern asmlinkage int (*save_fp_context)(struct sigcontext __user *sc);
extern asmlinkage int (*restore_fp_context)(struct sigcontext __user *sc);

extern asmlinkage int (*save_fp_context32)(struct sigcontext32 __user *sc);
extern asmlinkage int (*restore_fp_context32)(struct sigcontext32 __user *sc);

extern void fpu_emulator_init_fpu(void);
extern int fpu_emulator_save_context(struct sigcontext __user *sc);
extern int fpu_emulator_restore_context(struct sigcontext __user *sc);
extern void _init_fpu(void);
extern void _save_fp(struct task_struct *);
extern void _restore_fp(struct task_struct *);

#define __enable_fpu()							\
do {									\
        set_c0_status(ST0_CU1);						\
        enable_fpu_hazard();						\
} while (0)

#define __disable_fpu()							\
do {									\
	clear_c0_status(ST0_CU1);					\
        disable_fpu_hazard();						\
} while (0)

#define enable_fpu()							\
do {									\
	if (cpu_has_fpu)						\
		__enable_fpu();						\
} while (0)

#define disable_fpu()							\
do {									\
	if (cpu_has_fpu)						\
		__disable_fpu();					\
} while (0)


#define clear_fpu_owner()	clear_thread_flag(TIF_USEDFPU)

static inline int __is_fpu_owner(void)
{
	return test_thread_flag(TIF_USEDFPU);
}

static inline int is_fpu_owner(void)
{
	return cpu_has_fpu && __is_fpu_owner();
}

static inline void __own_fpu(void)
{
	__enable_fpu();
	KSTK_STATUS(current) |= ST0_CU1;
	set_thread_flag(TIF_USEDFPU);
}

static inline void own_fpu_inatomic(int restore)
{
	if (cpu_has_fpu && !__is_fpu_owner()) {
		__own_fpu();
		if (restore)
			_restore_fp(current);
	}
}

static inline void own_fpu(int restore)
{
	preempt_disable();
	own_fpu_inatomic(restore);
	preempt_enable();
}

static inline void lose_fpu(int save)
{
	preempt_disable();
	if (is_fpu_owner()) {
		if (save)
			_save_fp(current);
		KSTK_STATUS(current) &= ~ST0_CU1;
		clear_thread_flag(TIF_USEDFPU);
		__disable_fpu();
	}
	preempt_enable();
}

static inline void init_fpu(void)
{
	preempt_disable();
	if (cpu_has_fpu) {
		__own_fpu();
		_init_fpu();
	} else {
		fpu_emulator_init_fpu();
	}
	preempt_enable();
}

static inline void save_fp(struct task_struct *tsk)
{
	if (cpu_has_fpu)
		_save_fp(tsk);
}

static inline void restore_fp(struct task_struct *tsk)
{
	if (cpu_has_fpu)
		_restore_fp(tsk);
}

static inline fpureg_t *get_fpu_regs(struct task_struct *tsk)
{
	if (tsk == current) {
		preempt_disable();
		if (is_fpu_owner())
			_save_fp(current);
		preempt_enable();
	}

	return tsk->thread.fpu.fpr;
}

#endif /* _ASM_FPU_H */
