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

#ifndef __PARISC_MMU_CONTEXT_H
#define __PARISC_MMU_CONTEXT_H

#include <linux/mm.h>
#include <linux/sched.h>
#include <asm/atomic.h>
#include <asm/pgalloc.h>
#include <asm/pgtable.h>
#include <asm-generic/mm_hooks.h>

static inline void enter_lazy_tlb(struct mm_struct *mm, struct task_struct *tsk)
{
}

/* on PA-RISC, we actually have enough contexts to justify an allocator
 * for them.  prumpf */

extern unsigned long alloc_sid(void);
extern void free_sid(unsigned long);

static inline int
init_new_context(struct task_struct *tsk, struct mm_struct *mm)
{
	BUG_ON(atomic_read(&mm->mm_users) != 1);

	mm->context = alloc_sid();
	return 0;
}

static inline void
destroy_context(struct mm_struct *mm)
{
	free_sid(mm->context);
	mm->context = 0;
}

static inline unsigned long __space_to_prot(mm_context_t context)
{
#if SPACEID_SHIFT == 0
	return context << 1;
#else
	return context >> (SPACEID_SHIFT - 1);
#endif
}

static inline void load_context(mm_context_t context)
{
	mtsp(context, 3);
	mtctl(__space_to_prot(context), 8);
}

static inline void switch_mm(struct mm_struct *prev, struct mm_struct *next, struct task_struct *tsk)
{

	if (prev != next) {
		mtctl(__pa(next->pgd), 25);
		load_context(next->context);
	}
}

#define deactivate_mm(tsk,mm)	do { } while (0)

static inline void activate_mm(struct mm_struct *prev, struct mm_struct *next)
{
	/*
	 * Activate_mm is our one chance to allocate a space id
	 * for a new mm created in the exec path. There's also
	 * some lazy tlb stuff, which is currently dead code, but
	 * we only allocate a space id if one hasn't been allocated
	 * already, so we should be OK.
	 */

	BUG_ON(next == &init_mm); /* Should never happen */

	if (next->context == 0)
	    next->context = alloc_sid();

	switch_mm(prev,next,current);
}
#endif
