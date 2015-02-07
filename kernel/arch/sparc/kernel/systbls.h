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

#ifndef _SYSTBLS_H
#define _SYSTBLS_H

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/utsname.h>
#include <asm/utrap.h>
#include <asm/signal.h>

extern asmlinkage unsigned long sys_getpagesize(void);
extern asmlinkage unsigned long sparc_brk(unsigned long brk);
extern asmlinkage long sparc_pipe(struct pt_regs *regs);
extern asmlinkage long sys_ipc(unsigned int call, int first,
			       unsigned long second,
			       unsigned long third,
			       void __user *ptr, long fifth);
extern asmlinkage long sparc64_newuname(struct new_utsname __user *name);
extern asmlinkage long sparc64_personality(unsigned long personality);
extern asmlinkage long sys64_munmap(unsigned long addr, size_t len);
extern asmlinkage unsigned long sys64_mremap(unsigned long addr,
					     unsigned long old_len,
					     unsigned long new_len,
					     unsigned long flags,
					     unsigned long new_addr);
extern asmlinkage unsigned long c_sys_nis_syscall(struct pt_regs *regs);
extern asmlinkage long sys_getdomainname(char __user *name, int len);
extern asmlinkage long sys_utrap_install(utrap_entry_t type,
					 utrap_handler_t new_p,
					 utrap_handler_t new_d,
					 utrap_handler_t __user *old_p,
					 utrap_handler_t __user *old_d);
extern asmlinkage long sparc_memory_ordering(unsigned long model,
					     struct pt_regs *regs);
extern asmlinkage long sys_rt_sigaction(int sig,
					const struct sigaction __user *act,
					struct sigaction __user *oact,
					void __user *restorer,
					size_t sigsetsize);
extern asmlinkage long sys_perfctr(int opcode, unsigned long arg0,
				   unsigned long arg1, unsigned long arg2);

extern asmlinkage void sparc64_set_context(struct pt_regs *regs);
extern asmlinkage void sparc64_get_context(struct pt_regs *regs);
extern asmlinkage long sys_sigpause(unsigned int set);
extern asmlinkage long sys_sigsuspend(old_sigset_t set);
extern void do_rt_sigreturn(struct pt_regs *regs);

#endif /* _SYSTBLS_H */
