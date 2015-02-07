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
 * Licensed under the GPL
 */

#ifndef __SYS_PTRACE_PPC_H
#define __SYS_PTRACE_PPC_H

#include "linux/types.h"

/* the following taken from <asm-ppc/ptrace.h> */

#ifdef CONFIG_PPC64
#define PPC_REG unsigned long /*long*/
#else
#define PPC_REG unsigned long
#endif
struct sys_pt_regs_s {
	PPC_REG gpr[32];
	PPC_REG nip;
	PPC_REG msr;
	PPC_REG orig_gpr3;	/* Used for restarting system calls */
	PPC_REG ctr;
	PPC_REG link;
	PPC_REG xer;
	PPC_REG ccr;
	PPC_REG mq;		/* 601 only (not used at present) */
				/* Used on APUS to hold IPL value. */
	PPC_REG trap;		/* Reason for being here */
	PPC_REG dar;		/* Fault registers */
	PPC_REG dsisr;
	PPC_REG result; 	/* Result of a system call */
};

#define NUM_REGS (sizeof(struct sys_pt_regs_s) / sizeof(PPC_REG))

struct sys_pt_regs {
    PPC_REG regs[sizeof(struct sys_pt_regs_s) / sizeof(PPC_REG)];
};

#define UM_MAX_REG (PT_FPR0)
#define UM_MAX_REG_OFFSET (UM_MAX_REG * sizeof(PPC_REG))

#define EMPTY_REGS { { [ 0 ... NUM_REGS - 1] = 0 } }

#define UM_REG(r, n) ((r)->regs[n])

#define UM_SYSCALL_RET(r) UM_REG(r, PT_R3)
#define UM_SP(r) UM_REG(r, PT_R1)
#define UM_IP(r) UM_REG(r, PT_NIP)
#define UM_ELF_ZERO(r) UM_REG(r, PT_FPSCR)
#define UM_SYSCALL_NR(r) UM_REG(r, PT_R0)
#define UM_SYSCALL_ARG1(r) UM_REG(r, PT_ORIG_R3)
#define UM_SYSCALL_ARG2(r) UM_REG(r, PT_R4)
#define UM_SYSCALL_ARG3(r) UM_REG(r, PT_R5)
#define UM_SYSCALL_ARG4(r) UM_REG(r, PT_R6)
#define UM_SYSCALL_ARG5(r) UM_REG(r, PT_R7)
#define UM_SYSCALL_ARG6(r) UM_REG(r, PT_R8)

#define UM_SYSCALL_NR_OFFSET (PT_R0 * sizeof(PPC_REG))
#define UM_SYSCALL_RET_OFFSET (PT_R3 * sizeof(PPC_REG))
#define UM_SYSCALL_ARG1_OFFSET (PT_R3 * sizeof(PPC_REG))
#define UM_SYSCALL_ARG2_OFFSET (PT_R4 * sizeof(PPC_REG))
#define UM_SYSCALL_ARG3_OFFSET (PT_R5 * sizeof(PPC_REG))
#define UM_SYSCALL_ARG4_OFFSET (PT_R6 * sizeof(PPC_REG))
#define UM_SYSCALL_ARG5_OFFSET (PT_R7 * sizeof(PPC_REG))
#define UM_SYSCALL_ARG6_OFFSET (PT_R8 * sizeof(PPC_REG))
#define UM_SP_OFFSET (PT_R1 * sizeof(PPC_REG))
#define UM_IP_OFFSET (PT_NIP * sizeof(PPC_REG))
#define UM_ELF_ZERO_OFFSET (PT_R3 * sizeof(PPC_REG))

#define UM_SET_SYSCALL_RETURN(_regs, result)	        \
do {                                                    \
        if (result < 0) {				\
		(_regs)->regs[PT_CCR] |= 0x10000000;	\
		UM_SYSCALL_RET((_regs)) = -result;	\
        } else {					\
		UM_SYSCALL_RET((_regs)) = result;	\
        }                                               \
} while(0)

extern void shove_aux_table(unsigned long sp);
#define UM_FIX_EXEC_STACK(sp) shove_aux_table(sp);

/* These aren't actually defined.  The undefs are just to make sure
 * everyone's clear on the concept.
 */
#undef UML_HAVE_GETREGS
#undef UML_HAVE_GETFPREGS
#undef UML_HAVE_SETREGS
#undef UML_HAVE_SETFPREGS

#endif

/*
 * Overrides for Emacs so that we follow Linus's tabbing style.
 * Emacs will notice this stuff at the end of the file and automatically
 * adjust the settings for this buffer only.  This must remain at the end
 * of the file.
 * ---------------------------------------------------------------------------
 * Local variables:
 * c-file-style: "linux"
 * End:
 */
