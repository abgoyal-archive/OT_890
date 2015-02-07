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

#ifndef _H8300_PTRACE_H
#define _H8300_PTRACE_H

#ifndef __ASSEMBLY__

#define PT_ER1	   0
#define PT_ER2	   1
#define PT_ER3	   2
#define PT_ER4	   3
#define PT_ER5	   4
#define PT_ER6	   5
#define PT_ER0	   6
#define PT_ORIG_ER0	   7
#define PT_CCR	   8
#define PT_PC	   9
#define PT_USP	   10
#define PT_EXR     12

/* this struct defines the way the registers are stored on the
   stack during a system call. */

struct pt_regs {
	long     retpc;
	long     er4;
	long     er5;
	long     er6;
	long     er3;
	long     er2;
	long     er1;
	long     orig_er0;
	unsigned short ccr;
	long     er0;
	long     vector;
#if defined(CONFIG_CPU_H8S)
	unsigned short exr;
#endif
	unsigned long  pc;
} __attribute__((aligned(2),packed));

#define PTRACE_GETREGS            12
#define PTRACE_SETREGS            13

#ifdef __KERNEL__
#ifndef PS_S
#define PS_S  (0x10)
#endif

#if defined(__H8300H__)
#define H8300_REGS_NO 11
#endif
#if defined(__H8300S__)
#define H8300_REGS_NO 12
#endif

/* Find the stack offset for a register, relative to thread.esp0. */
#define PT_REG(reg)	((long)&((struct pt_regs *)0)->reg)

#define user_mode(regs) (!((regs)->ccr & PS_S))
#define instruction_pointer(regs) ((regs)->pc)
#define profile_pc(regs) instruction_pointer(regs)
extern void show_regs(struct pt_regs *);
#endif /* __KERNEL__ */
#endif /* __ASSEMBLY__ */
#endif /* _H8300_PTRACE_H */
