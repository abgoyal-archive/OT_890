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

#ifndef __ASM_SH_SYSCALLS_32_H
#define __ASM_SH_SYSCALLS_32_H

#ifdef __KERNEL__

#include <linux/compiler.h>
#include <linux/linkage.h>
#include <linux/types.h>

struct pt_regs;

asmlinkage int sys_fork(unsigned long r4, unsigned long r5,
			unsigned long r6, unsigned long r7,
			struct pt_regs __regs);
asmlinkage int sys_clone(unsigned long clone_flags, unsigned long newsp,
			 unsigned long parent_tidptr,
			 unsigned long child_tidptr,
			 struct pt_regs __regs);
asmlinkage int sys_vfork(unsigned long r4, unsigned long r5,
			 unsigned long r6, unsigned long r7,
			 struct pt_regs __regs);
asmlinkage int sys_execve(char __user *ufilename, char __user * __user *uargv,
			  char __user * __user *uenvp, unsigned long r7,
			  struct pt_regs __regs);
asmlinkage int sys_sigsuspend(old_sigset_t mask, unsigned long r5,
			      unsigned long r6, unsigned long r7,
			      struct pt_regs __regs);
asmlinkage int sys_sigaction(int sig, const struct old_sigaction __user *act,
			     struct old_sigaction __user *oact);
asmlinkage int sys_sigaltstack(const stack_t __user *uss, stack_t __user *uoss,
			       unsigned long r6, unsigned long r7,
			       struct pt_regs __regs);
asmlinkage int sys_sigreturn(unsigned long r4, unsigned long r5,
			     unsigned long r6, unsigned long r7,
			     struct pt_regs __regs);
asmlinkage int sys_rt_sigreturn(unsigned long r4, unsigned long r5,
				unsigned long r6, unsigned long r7,
				struct pt_regs __regs);
asmlinkage int sys_sh_pipe(unsigned long r4, unsigned long r5,
			   unsigned long r6, unsigned long r7,
			   struct pt_regs __regs);
asmlinkage ssize_t sys_pread_wrapper(unsigned int fd, char __user *buf,
				     size_t count, long dummy, loff_t pos);
asmlinkage ssize_t sys_pwrite_wrapper(unsigned int fd, const char __user *buf,
				      size_t count, long dummy, loff_t pos);
asmlinkage int sys_fadvise64_64_wrapper(int fd, u32 offset0, u32 offset1,
					u32 len0, u32 len1, int advice);

/* Misc syscall related bits */
asmlinkage long do_syscall_trace_enter(struct pt_regs *regs);
asmlinkage void do_syscall_trace_leave(struct pt_regs *regs);
asmlinkage void do_notify_resume(struct pt_regs *regs, unsigned int save_r0,
				 unsigned long thread_info_flags);

#endif /* __KERNEL__ */
#endif /* __ASM_SH_SYSCALLS_32_H */
