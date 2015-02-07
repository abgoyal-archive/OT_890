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

#ifndef __ASM_BFIN_PROCESSOR_H
#define __ASM_BFIN_PROCESSOR_H

/*
 * Default implementation of macro that returns current
 * instruction pointer ("program counter").
 */
#define current_text_addr() ({ __label__ _l; _l: &&_l;})

#include <asm/blackfin.h>
#include <asm/segment.h>
#include <linux/compiler.h>

static inline unsigned long rdusp(void)
{
	unsigned long usp;

	__asm__ __volatile__("%0 = usp;\n\t":"=da"(usp));
	return usp;
}

static inline void wrusp(unsigned long usp)
{
	__asm__ __volatile__("usp = %0;\n\t"::"da"(usp));
}

static inline unsigned long __get_SP(void)
{
	unsigned long sp;

	__asm__ __volatile__("%0 = sp;\n\t" : "=da"(sp));
	return sp;
}

/*
 * User space process size: 1st byte beyond user address space.
 * Fairly meaningless on nommu.  Parts of user programs can be scattered
 * in a lot of places, so just disable this by setting it to 0xFFFFFFFF.
 */
#define TASK_SIZE	0xFFFFFFFF

#ifdef __KERNEL__
#define STACK_TOP	TASK_SIZE
#endif

#define TASK_UNMAPPED_BASE	0

struct thread_struct {
	unsigned long ksp;	/* kernel stack pointer */
	unsigned long usp;	/* user stack pointer */
	unsigned short seqstat;	/* saved status register */
	unsigned long esp0;	/* points to SR of stack frame pt_regs */
	unsigned long pc;	/* instruction pointer */
	void *        debuggerinfo;
};

#define INIT_THREAD  {						\
	sizeof(init_stack) + (unsigned long) init_stack, 0,	\
	PS_S, 0, 0						\
}

/*
 * Do necessary setup to start up a newly executed thread.
 *
 * pass the data segment into user programs if it exists,
 * it can't hurt anything as far as I can tell
 */
#ifndef CONFIG_SMP
#define start_thread(_regs, _pc, _usp)					\
do {									\
	set_fs(USER_DS);						\
	(_regs)->pc = (_pc);						\
	if (current->mm)						\
		(_regs)->p5 = current->mm->start_data;			\
	task_thread_info(current)->l1_task_info.stack_start		\
		= (void *)current->mm->context.stack_start;		\
	task_thread_info(current)->l1_task_info.lowest_sp = (void *)(_usp); \
	memcpy(L1_SCRATCH_TASK_INFO, &task_thread_info(current)->l1_task_info, \
		sizeof(*L1_SCRATCH_TASK_INFO));				\
	wrusp(_usp);							\
} while(0)
#else
#define start_thread(_regs, _pc, _usp)					\
do {									\
	set_fs(USER_DS);						\
	(_regs)->pc = (_pc);						\
	if (current->mm)						\
		(_regs)->p5 = current->mm->start_data;			\
	wrusp(_usp);							\
} while (0)
#endif

/* Forward declaration, a strange C thing */
struct task_struct;

/* Free all resources held by a thread. */
static inline void release_thread(struct task_struct *dead_task)
{
}

#define prepare_to_copy(tsk)	do { } while (0)

extern int kernel_thread(int (*fn) (void *), void *arg, unsigned long flags);

/*
 * Free current thread data structures etc..
 */
static inline void exit_thread(void)
{
}

/*
 * Return saved PC of a blocked thread.
 */
#define thread_saved_pc(tsk)	(tsk->thread.pc)

unsigned long get_wchan(struct task_struct *p);

#define	KSTK_EIP(tsk)							\
    ({									\
	unsigned long eip = 0;						\
	if ((tsk)->thread.esp0 > PAGE_SIZE &&				\
	    MAP_NR((tsk)->thread.esp0) < max_mapnr)			\
	      eip = ((struct pt_regs *) (tsk)->thread.esp0)->pc;	\
	eip; })
#define	KSTK_ESP(tsk)	((tsk) == current ? rdusp() : (tsk)->thread.usp)

#define cpu_relax()    	smp_mb()


/* Get the Silicon Revision of the chip */
static inline uint32_t __pure bfin_revid(void)
{
	/* stored in the upper 4 bits */
	uint32_t revid = bfin_read_CHIPID() >> 28;

#ifdef CONFIG_BF52x
	/* ANOMALY_05000357
	 * Incorrect Revision Number in DSPID Register
	 */
	if (revid == 0)
		switch (bfin_read16(_BOOTROM_GET_DXE_ADDRESS_TWI)) {
		case 0x0010:
			revid = 0;
			break;
		case 0x2796:
			revid = 1;
			break;
		default:
			revid = 0xFFFF;
			break;
		}
#endif
	return revid;
}

static inline uint16_t __pure bfin_cpuid(void)
{
	return (bfin_read_CHIPID() & CHIPID_FAMILY) >> 12;
}

static inline uint32_t __pure bfin_dspid(void)
{
	return bfin_read_DSPID();
}

static inline uint32_t __pure bfin_compiled_revid(void)
{
#if defined(CONFIG_BF_REV_0_0)
	return 0;
#elif defined(CONFIG_BF_REV_0_1)
	return 1;
#elif defined(CONFIG_BF_REV_0_2)
	return 2;
#elif defined(CONFIG_BF_REV_0_3)
	return 3;
#elif defined(CONFIG_BF_REV_0_4)
	return 4;
#elif defined(CONFIG_BF_REV_0_5)
	return 5;
#elif defined(CONFIG_BF_REV_0_6)
	return 6;
#elif defined(CONFIG_BF_REV_ANY)
	return 0xffff;
#else
	return -1;
#endif
}

#endif
