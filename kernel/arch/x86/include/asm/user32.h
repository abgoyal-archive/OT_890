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

#ifndef _ASM_X86_USER32_H
#define _ASM_X86_USER32_H

/* IA32 compatible user structures for ptrace.
 * These should be used for 32bit coredumps too. */

struct user_i387_ia32_struct {
	u32	cwd;
	u32	swd;
	u32	twd;
	u32	fip;
	u32	fcs;
	u32	foo;
	u32	fos;
	u32	st_space[20];   /* 8*10 bytes for each FP-reg = 80 bytes */
};

/* FSAVE frame with extensions */
struct user32_fxsr_struct {
	unsigned short	cwd;
	unsigned short	swd;
	unsigned short	twd;	/* not compatible to 64bit twd */
	unsigned short	fop;
	int	fip;
	int	fcs;
	int	foo;
	int	fos;
	int	mxcsr;
	int	reserved;
	int	st_space[32];	/* 8*16 bytes for each FP-reg = 128 bytes */
	int	xmm_space[32];	/* 8*16 bytes for each XMM-reg = 128 bytes */
	int	padding[56];
};

struct user_regs_struct32 {
	__u32 ebx, ecx, edx, esi, edi, ebp, eax;
	unsigned short ds, __ds, es, __es;
	unsigned short fs, __fs, gs, __gs;
	__u32 orig_eax, eip;
	unsigned short cs, __cs;
	__u32 eflags, esp;
	unsigned short ss, __ss;
};

struct user32 {
  struct user_regs_struct32 regs; /* Where the registers are actually stored */
  int u_fpvalid;		/* True if math co-processor being used. */
				/* for this mess. Not yet used. */
  struct user_i387_ia32_struct i387;	/* Math Co-processor registers. */
/* The rest of this junk is to help gdb figure out what goes where */
  __u32 u_tsize;	/* Text segment size (pages). */
  __u32 u_dsize;	/* Data segment size (pages). */
  __u32 u_ssize;	/* Stack segment size (pages). */
  __u32 start_code;     /* Starting virtual address of text. */
  __u32 start_stack;	/* Starting virtual address of stack area.
				   This is actually the bottom of the stack,
				   the top of the stack is always found in the
				   esp register.  */
  __u32 signal;     		/* Signal that caused the core dump. */
  int reserved;			/* No __u32er used */
  __u32 u_ar0;	/* Used by gdb to help find the values for */
				/* the registers. */
  __u32 u_fpstate;	/* Math Co-processor pointer. */
  __u32 magic;		/* To uniquely identify a core file */
  char u_comm[32];		/* User command that was responsible */
  int u_debugreg[8];
};


#endif /* _ASM_X86_USER32_H */
