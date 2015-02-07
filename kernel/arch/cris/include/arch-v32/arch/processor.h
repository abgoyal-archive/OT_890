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

#ifndef _ASM_CRIS_ARCH_PROCESSOR_H
#define _ASM_CRIS_ARCH_PROCESSOR_H


/* Return current instruction pointer. */
#define current_text_addr() \
	({void *pc; __asm__ __volatile__ ("lapcq .,%0" : "=rm" (pc)); pc;})

/*
 * Since CRIS doesn't do hardware task-switching this hasn't really anything to
 * do with the proccessor itself, it's just here for legacy reasons. This is
 * used when task-switching using _resume defined in entry.S. The offsets here
 * are hardcoded into _resume, so if this struct is changed, entry.S needs to be
 * changed as well.
 */
struct thread_struct {
	unsigned long ksp;	/* Kernel stack pointer. */
	unsigned long usp;	/* User stack pointer. */
	unsigned long ccs;	/* Saved flags register. */
};

/*
 * User-space process size. This is hardcoded into a few places, so don't
 * changed it unless everything's clear!
 */
#ifndef CONFIG_ETRAX_VCS_SIM
#define TASK_SIZE	(0xB0000000UL)
#else
#define TASK_SIZE	(0xA0000000UL)
#endif

/* CCS I=1, enable interrupts. */
#define INIT_THREAD { 0, 0, (1 << I_CCS_BITNR) }

#define KSTK_EIP(tsk)		\
({				\
	unsigned long eip = 0;	\
	unsigned long regs = (unsigned long)task_pt_regs(tsk); \
	if (regs > PAGE_SIZE && virt_addr_valid(regs))	    \
		eip = ((struct pt_regs *)regs)->erp;	    \
	eip; \
})

/*
 * Give the thread a program location, set user-mode and switch user
 * stackpointer.
 */
#define start_thread(regs, ip, usp) \
do { \
	set_fs(USER_DS); \
	regs->erp = ip; \
	regs->ccs |= 1 << (U_CCS_BITNR + CCS_SHIFT); \
	wrusp(usp); \
} while(0)

/* Nothing special to do for v32 when handling a kernel bus fault fixup. */
#define arch_fixup(regs) {};

#endif /* _ASM_CRIS_ARCH_PROCESSOR_H */
