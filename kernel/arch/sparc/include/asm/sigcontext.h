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

#ifndef __SPARC_SIGCONTEXT_H
#define __SPARC_SIGCONTEXT_H

#ifdef __KERNEL__
#include <asm/ptrace.h>

#ifndef __ASSEMBLY__

#define __SUNOS_MAXWIN   31

/* This is what SunOS does, so shall I unless we use new 32bit signals or rt signals. */
struct sigcontext32 {
	int sigc_onstack;      /* state to restore */
	int sigc_mask;         /* sigmask to restore */
	int sigc_sp;           /* stack pointer */
	int sigc_pc;           /* program counter */
	int sigc_npc;          /* next program counter */
	int sigc_psr;          /* for condition codes etc */
	int sigc_g1;           /* User uses these two registers */
	int sigc_o0;           /* within the trampoline code. */

	/* Now comes information regarding the users window set
	 * at the time of the signal.
	 */
	int sigc_oswins;       /* outstanding windows */

	/* stack ptrs for each regwin buf */
	unsigned sigc_spbuf[__SUNOS_MAXWIN];

	/* Windows to restore after signal */
	struct reg_window32 sigc_wbuf[__SUNOS_MAXWIN];
};


/* This is what we use for 32bit new non-rt signals. */

typedef struct {
	struct {
		unsigned int psr;
		unsigned int pc;
		unsigned int npc;
		unsigned int y;
		unsigned int u_regs[16]; /* globals and ins */
	}			si_regs;
	int			si_mask;
} __siginfo32_t;

#ifdef CONFIG_SPARC64
typedef struct {
	unsigned   int si_float_regs [64];
	unsigned   long si_fsr;
	unsigned   long si_gsr;
	unsigned   long si_fprs;
} __siginfo_fpu_t;

/* This is what SunOS doesn't, so we have to write this alone
   and do it properly. */
struct sigcontext {
	/* The size of this array has to match SI_MAX_SIZE from siginfo.h */
	char			sigc_info[128];
	struct {
		unsigned long	u_regs[16]; /* globals and ins */
		unsigned long	tstate;
		unsigned long	tpc;
		unsigned long	tnpc;
		unsigned int	y;
		unsigned int	fprs;
	}			sigc_regs;
	__siginfo_fpu_t *	sigc_fpu_save;
	struct {
		void	*	ss_sp;
		int		ss_flags;
		unsigned long	ss_size;
	}			sigc_stack;
	unsigned long		sigc_mask;
};

#else

typedef struct {
	unsigned long si_float_regs [32];
	unsigned long si_fsr;
	unsigned long si_fpqdepth;
	struct {
		unsigned long *insn_addr;
		unsigned long insn;
	} si_fpqueue [16];
} __siginfo_fpu_t;
#endif /* (CONFIG_SPARC64) */


#endif /* !(__ASSEMBLY__) */

#endif /* (__KERNEL__) */

#endif /* !(__SPARC_SIGCONTEXT_H) */
