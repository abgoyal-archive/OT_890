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

#ifndef _ASM_IA64_IA32_H
#define _ASM_IA64_IA32_H


#include <asm/ptrace.h>
#include <asm/signal.h>

#define IA32_NR_syscalls		285	/* length of syscall table */
#define IA32_PAGE_SHIFT			12	/* 4KB pages */

#ifndef __ASSEMBLY__

# ifdef CONFIG_IA32_SUPPORT

#define IA32_PAGE_OFFSET	0xc0000000

extern void ia32_cpu_init (void);
extern void ia32_mem_init (void);
extern void ia32_gdt_init (void);
extern int ia32_exception (struct pt_regs *regs, unsigned long isr);
extern int ia32_intercept (struct pt_regs *regs, unsigned long isr);
extern int ia32_clone_tls (struct task_struct *child, struct pt_regs *childregs);

# endif /* !CONFIG_IA32_SUPPORT */

/* Declare this unconditionally, so we don't get warnings for unreachable code.  */
extern int ia32_setup_frame1 (int sig, struct k_sigaction *ka, siginfo_t *info,
			      sigset_t *set, struct pt_regs *regs);
#if PAGE_SHIFT > IA32_PAGE_SHIFT
extern int ia32_copy_ia64_partial_page_list(struct task_struct *,
					unsigned long);
extern void ia32_drop_ia64_partial_page_list(struct task_struct *);
#else
# define ia32_copy_ia64_partial_page_list(a1, a2)	0
# define ia32_drop_ia64_partial_page_list(a1)	do { ; } while (0)
#endif

#endif /* !__ASSEMBLY__ */

#endif /* _ASM_IA64_IA32_H */
