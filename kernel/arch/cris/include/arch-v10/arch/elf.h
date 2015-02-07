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

#ifndef __ASMCRIS_ARCH_ELF_H
#define __ASMCRIS_ARCH_ELF_H

#define ELF_MACH EF_CRIS_VARIANT_ANY_V0_V10

/*
 * This is used to ensure we don't load something for the wrong architecture.
 */
#define elf_check_arch(x)			\
 ((x)->e_machine == EM_CRIS			\
  && ((((x)->e_flags & EF_CRIS_VARIANT_MASK) == EF_CRIS_VARIANT_ANY_V0_V10	\
      || (((x)->e_flags & EF_CRIS_VARIANT_MASK) == EF_CRIS_VARIANT_COMMON_V10_V32))))

/*
 * ELF register definitions..
 */

#include <asm/ptrace.h>

/* SVR4/i386 ABI (pages 3-31, 3-32) says that when the program
   starts (a register; assume first param register for CRIS)
   contains a pointer to a function which might be
   registered using `atexit'.  This provides a mean for the
   dynamic linker to call DT_FINI functions for shared libraries
   that have been loaded before the code runs.

   A value of 0 tells we have no such handler.  */

/* Explicitly set registers to 0 to increase determinism.  */
#define ELF_PLAT_INIT(_r, load_addr)	do { \
	(_r)->r13 = 0; (_r)->r12 = 0; (_r)->r11 = 0; (_r)->r10 = 0; \
	(_r)->r9 = 0;  (_r)->r8 = 0;  (_r)->r7 = 0;  (_r)->r6 = 0;  \
	(_r)->r5 = 0;  (_r)->r4 = 0;  (_r)->r3 = 0;  (_r)->r2 = 0;  \
	(_r)->r1 = 0;  (_r)->r0 = 0;  (_r)->mof = 0; (_r)->srp = 0; \
} while (0)

/* The additional layer below is because the stack pointer is missing in 
   the pt_regs struct, but needed in a core dump. pr_reg is a elf_gregset_t,
   and should be filled in according to the layout of the user_regs_struct
   struct; regs is a pt_regs struct. We dump all registers, though several are
   obviously unnecessary. That way there's less need for intelligence at 
   the receiving end (i.e. gdb). */
#define ELF_CORE_COPY_REGS(pr_reg, regs)                   \
	pr_reg[0] = regs->r0;                              \
	pr_reg[1] = regs->r1;                              \
	pr_reg[2] = regs->r2;                              \
	pr_reg[3] = regs->r3;                              \
	pr_reg[4] = regs->r4;                              \
	pr_reg[5] = regs->r5;                              \
	pr_reg[6] = regs->r6;                              \
	pr_reg[7] = regs->r7;                              \
	pr_reg[8] = regs->r8;                              \
	pr_reg[9] = regs->r9;                              \
	pr_reg[10] = regs->r10;                            \
	pr_reg[11] = regs->r11;                            \
	pr_reg[12] = regs->r12;                            \
	pr_reg[13] = regs->r13;                            \
	pr_reg[14] = rdusp();               /* sp */       \
	pr_reg[15] = regs->irp;             /* pc */       \
	pr_reg[16] = 0;                     /* p0 */       \
	pr_reg[17] = rdvr();                /* vr */       \
	pr_reg[18] = 0;                     /* p2 */       \
	pr_reg[19] = 0;                     /* p3 */       \
	pr_reg[20] = 0;                     /* p4 */       \
	pr_reg[21] = (regs->dccr & 0xffff); /* ccr */      \
	pr_reg[22] = 0;                     /* p6 */       \
	pr_reg[23] = regs->mof;             /* mof */      \
	pr_reg[24] = 0;                     /* p8 */       \
	pr_reg[25] = 0;                     /* ibr */      \
	pr_reg[26] = 0;                     /* irp */      \
	pr_reg[27] = regs->srp;             /* srp */      \
	pr_reg[28] = 0;                     /* bar */      \
	pr_reg[29] = regs->dccr;            /* dccr */     \
	pr_reg[30] = 0;                     /* brp */      \
	pr_reg[31] = rdusp();               /* usp */      \
	pr_reg[32] = 0;                     /* csrinstr */ \
	pr_reg[33] = 0;                     /* csraddr */  \
	pr_reg[34] = 0;                     /* csrdata */


#endif
