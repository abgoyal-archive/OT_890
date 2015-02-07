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

#ifndef __ASM_SH_FPU_H
#define __ASM_SH_FPU_H

#ifndef __ASSEMBLY__
#include <linux/preempt.h>
#include <asm/ptrace.h>

#ifdef CONFIG_SH_FPU
static inline void release_fpu(struct pt_regs *regs)
{
	regs->sr |= SR_FD;
}

static inline void grab_fpu(struct pt_regs *regs)
{
	regs->sr &= ~SR_FD;
}

struct task_struct;

extern void save_fpu(struct task_struct *__tsk, struct pt_regs *regs);
#else

#define release_fpu(regs)	do { } while (0)
#define grab_fpu(regs)		do { } while (0)

static inline void save_fpu(struct task_struct *tsk, struct pt_regs *regs)
{
	clear_tsk_thread_flag(tsk, TIF_USEDFPU);
}
#endif

struct user_regset;

extern int do_fpu_inst(unsigned short, struct pt_regs *);

extern int fpregs_get(struct task_struct *target,
		      const struct user_regset *regset,
		      unsigned int pos, unsigned int count,
		      void *kbuf, void __user *ubuf);

static inline void unlazy_fpu(struct task_struct *tsk, struct pt_regs *regs)
{
	preempt_disable();
	if (test_tsk_thread_flag(tsk, TIF_USEDFPU))
		save_fpu(tsk, regs);
	preempt_enable();
}

static inline void clear_fpu(struct task_struct *tsk, struct pt_regs *regs)
{
	preempt_disable();
	if (test_tsk_thread_flag(tsk, TIF_USEDFPU)) {
		clear_tsk_thread_flag(tsk, TIF_USEDFPU);
		release_fpu(regs);
	}
	preempt_enable();
}

static inline int init_fpu(struct task_struct *tsk)
{
	if (tsk_used_math(tsk)) {
		if ((boot_cpu_data.flags & CPU_HAS_FPU) && tsk == current)
			unlazy_fpu(tsk, task_pt_regs(tsk));
		return 0;
	}

	set_stopped_child_used_math(tsk);
	return 0;
}

#endif /* __ASSEMBLY__ */

#endif /* __ASM_SH_FPU_H */
