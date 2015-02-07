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
 * This control block defines the PACA which defines the processor
 * specific data for each logical processor on the system.
 * There are some pointers defined that are utilized by PLIC.
 *
 * C 2001 PPC 64 Team, IBM Corp
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */
#ifndef _ASM_POWERPC_PACA_H
#define _ASM_POWERPC_PACA_H
#ifdef __KERNEL__

#include	<asm/types.h>
#include	<asm/lppaca.h>
#include	<asm/mmu.h>

register struct paca_struct *local_paca asm("r13");

#if defined(CONFIG_DEBUG_PREEMPT) && defined(CONFIG_SMP)
extern unsigned int debug_smp_processor_id(void); /* from linux/smp.h */
/*
 * Add standard checks that preemption cannot occur when using get_paca():
 * otherwise the paca_struct it points to may be the wrong one just after.
 */
#define get_paca()	((void) debug_smp_processor_id(), local_paca)
#else
#define get_paca()	local_paca
#endif

#define get_lppaca()	(get_paca()->lppaca_ptr)
#define get_slb_shadow()	(get_paca()->slb_shadow_ptr)

struct task_struct;

/*
 * Defines the layout of the paca.
 *
 * This structure is not directly accessed by firmware or the service
 * processor.
 */
struct paca_struct {
	/*
	 * Because hw_cpu_id, unlike other paca fields, is accessed
	 * routinely from other CPUs (from the IRQ code), we stick to
	 * read-only (after boot) fields in the first cacheline to
	 * avoid cacheline bouncing.
	 */

	struct lppaca *lppaca_ptr;	/* Pointer to LpPaca for PLIC */

	/*
	 * MAGIC: the spinlock functions in arch/powerpc/lib/locks.c 
	 * load lock_token and paca_index with a single lwz
	 * instruction.  They must travel together and be properly
	 * aligned.
	 */
	u16 lock_token;			/* Constant 0x8000, used in locks */
	u16 paca_index;			/* Logical processor number */

	u64 kernel_toc;			/* Kernel TOC address */
	u64 kernelbase;			/* Base address of kernel */
	u64 kernel_msr;			/* MSR while running in kernel */
	u64 stab_real;			/* Absolute address of segment table */
	u64 stab_addr;			/* Virtual address of segment table */
	void *emergency_sp;		/* pointer to emergency stack */
	u64 data_offset;		/* per cpu data offset */
	s16 hw_cpu_id;			/* Physical processor number */
	u8 cpu_start;			/* At startup, processor spins until */
					/* this becomes non-zero. */
	struct slb_shadow *slb_shadow_ptr;

	/*
	 * Now, starting in cacheline 2, the exception save areas
	 */
	/* used for most interrupts/exceptions */
	u64 exgen[10] __attribute__((aligned(0x80)));
	u64 exmc[10];		/* used for machine checks */
	u64 exslb[10];		/* used for SLB/segment table misses
 				 * on the linear mapping */

	mm_context_t context;
	u16 vmalloc_sllp;
	u16 slb_cache_ptr;
	u16 slb_cache[SLB_CACHE_ENTRIES];

	/*
	 * then miscellaneous read-write fields
	 */
	struct task_struct *__current;	/* Pointer to current */
	u64 kstack;			/* Saved Kernel stack addr */
	u64 stab_rr;			/* stab/slb round-robin counter */
	u64 saved_r1;			/* r1 save for RTAS calls */
	u64 saved_msr;			/* MSR saved here by enter_rtas */
	u16 trap_save;			/* Used when bad stack is encountered */
	u8 soft_enabled;		/* irq soft-enable flag */
	u8 hard_enabled;		/* set if irqs are enabled in MSR */
	u8 io_sync;			/* writel() needs spin_unlock sync */

	/* Stuff for accurate time accounting */
	u64 user_time;			/* accumulated usermode TB ticks */
	u64 system_time;		/* accumulated system TB ticks */
	u64 startpurr;			/* PURR/TB value snapshot */
	u64 startspurr;			/* SPURR value snapshot */
};

extern struct paca_struct paca[];
extern void initialise_pacas(void);

#endif /* __KERNEL__ */
#endif /* _ASM_POWERPC_PACA_H */
