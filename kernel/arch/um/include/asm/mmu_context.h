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
 * Copyright (C) 2002 - 2007 Jeff Dike (jdike@{addtoit,linux.intel}.com)
 * Licensed under the GPL
 */

#ifndef __UM_MMU_CONTEXT_H
#define __UM_MMU_CONTEXT_H

#include "linux/sched.h"
#include "um_mmu.h"

extern void arch_dup_mmap(struct mm_struct *oldmm, struct mm_struct *mm);
extern void arch_exit_mmap(struct mm_struct *mm);

#define get_mmu_context(task) do ; while(0)
#define activate_context(tsk) do ; while(0)

#define deactivate_mm(tsk,mm)	do { } while (0)

extern void force_flush_all(void);

static inline void activate_mm(struct mm_struct *old, struct mm_struct *new)
{
	/*
	 * This is called by fs/exec.c and sys_unshare()
	 * when the new ->mm is used for the first time.
	 */
	__switch_mm(&new->context.id);
	arch_dup_mmap(old, new);
}

static inline void switch_mm(struct mm_struct *prev, struct mm_struct *next, 
			     struct task_struct *tsk)
{
	unsigned cpu = smp_processor_id();

	if(prev != next){
		cpu_clear(cpu, prev->cpu_vm_mask);
		cpu_set(cpu, next->cpu_vm_mask);
		if(next != &init_mm)
			__switch_mm(&next->context.id);
	}
}

static inline void enter_lazy_tlb(struct mm_struct *mm, 
				  struct task_struct *tsk)
{
}

extern int init_new_context(struct task_struct *task, struct mm_struct *mm);

extern void destroy_context(struct mm_struct *mm);

#endif
