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
 * include/asm-m68knommu/processor.h
 *
 * Copyright (C) 1995 Hamish Macdonald
 */

#ifndef __ASM_M68K_PROCESSOR_H
#define __ASM_M68K_PROCESSOR_H

/*
 * Default implementation of macro that returns current
 * instruction pointer ("program counter").
 */
#define current_text_addr() ({ __label__ _l; _l: &&_l;})

#include <linux/compiler.h>
#include <linux/threads.h>
#include <asm/types.h>
#include <asm/segment.h>
#include <asm/fpu.h>
#include <asm/ptrace.h>
#include <asm/current.h>

static inline unsigned long rdusp(void)
{
#ifdef CONFIG_COLDFIRE
	extern unsigned int sw_usp;
	return(sw_usp);
#else
  	unsigned long usp;
	__asm__ __volatile__("move %/usp,%0" : "=a" (usp));
	return usp;
#endif
}

static inline void wrusp(unsigned long usp)
{
#ifdef CONFIG_COLDFIRE
	extern unsigned int sw_usp;
	sw_usp = usp;
#else
	__asm__ __volatile__("move %0,%/usp" : : "a" (usp));
#endif
}

/*
 * User space process size: 3.75GB. This is hardcoded into a few places,
 * so don't change it unless you know what you are doing.
 */
#define TASK_SIZE	(0xF0000000UL)

/*
 * This decides where the kernel will search for a free chunk of vm
 * space during mmap's. We won't be using it
 */
#define TASK_UNMAPPED_BASE	0

/* 
 * if you change this structure, you must change the code and offsets
 * in m68k/machasm.S
 */
   
struct thread_struct {
	unsigned long  ksp;		/* kernel stack pointer */
	unsigned long  usp;		/* user stack pointer */
	unsigned short sr;		/* saved status register */
	unsigned short fs;		/* saved fs (sfc, dfc) */
	unsigned long  crp[2];		/* cpu root pointer */
	unsigned long  esp0;		/* points to SR of stack frame */
	unsigned long  fp[8*3];
	unsigned long  fpcntl[3];	/* fp control regs */
	unsigned char  fpstate[FPSTATESIZE];  /* floating point state */
};

#define INIT_THREAD  { \
	sizeof(init_stack) + (unsigned long) init_stack, 0, \
	PS_S, __KERNEL_DS, \
	{0, 0}, 0, {0,}, {0, 0, 0}, {0,}, \
}

/*
 * Coldfire stacks need to be re-aligned on trap exit, conventional
 * 68k can handle this case cleanly.
 */
#if defined(CONFIG_COLDFIRE)
#define	reformat(_regs)		do { (_regs)->format = 0x4; } while(0)
#else
#define	reformat(_regs)		do { } while (0)
#endif

/*
 * Do necessary setup to start up a newly executed thread.
 *
 * pass the data segment into user programs if it exists,
 * it can't hurt anything as far as I can tell
 */
#define start_thread(_regs, _pc, _usp)			\
do {							\
	set_fs(USER_DS); /* reads from user space */	\
	(_regs)->pc = (_pc);				\
	((struct switch_stack *)(_regs))[-1].a6 = 0;	\
	reformat(_regs);				\
	if (current->mm)				\
		(_regs)->d5 = current->mm->start_data;	\
	(_regs)->sr &= ~0x2000;				\
	wrusp(_usp);					\
} while(0)

/* Forward declaration, a strange C thing */
struct task_struct;

/* Free all resources held by a thread. */
static inline void release_thread(struct task_struct *dead_task)
{
}

/* Prepare to copy thread state - unlazy all lazy status */
#define prepare_to_copy(tsk)	do { } while (0)

extern int kernel_thread(int (*fn)(void *), void * arg, unsigned long flags);

/*
 * Free current thread data structures etc..
 */
static inline void exit_thread(void)
{
}

unsigned long thread_saved_pc(struct task_struct *tsk);
unsigned long get_wchan(struct task_struct *p);

#define	KSTK_EIP(tsk)	\
    ({			\
	unsigned long eip = 0;	 \
	if ((tsk)->thread.esp0 > PAGE_SIZE && \
	    (virt_addr_valid((tsk)->thread.esp0))) \
	      eip = ((struct pt_regs *) (tsk)->thread.esp0)->pc; \
	eip; })
#define	KSTK_ESP(tsk)	((tsk) == current ? rdusp() : (tsk)->thread.usp)

#define cpu_relax()    barrier()

#endif
