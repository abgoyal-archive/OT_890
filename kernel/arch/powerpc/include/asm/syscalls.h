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

#ifndef __ASM_POWERPC_SYSCALLS_H
#define __ASM_POWERPC_SYSCALLS_H
#ifdef __KERNEL__

#include <linux/compiler.h>
#include <linux/linkage.h>
#include <linux/types.h>
#include <asm/signal.h>

struct new_utsname;
struct pt_regs;
struct rtas_args;
struct sigaction;

asmlinkage unsigned long sys_mmap(unsigned long addr, size_t len,
		unsigned long prot, unsigned long flags,
		unsigned long fd, off_t offset);
asmlinkage unsigned long sys_mmap2(unsigned long addr, size_t len,
		unsigned long prot, unsigned long flags,
		unsigned long fd, unsigned long pgoff);
asmlinkage int sys_execve(unsigned long a0, unsigned long a1,
		unsigned long a2, unsigned long a3, unsigned long a4,
		unsigned long a5, struct pt_regs *regs);
asmlinkage int sys_clone(unsigned long clone_flags, unsigned long usp,
		int __user *parent_tidp, void __user *child_threadptr,
		int __user *child_tidp, int p6, struct pt_regs *regs);
asmlinkage int sys_fork(unsigned long p1, unsigned long p2,
		unsigned long p3, unsigned long p4, unsigned long p5,
		unsigned long p6, struct pt_regs *regs);
asmlinkage int sys_vfork(unsigned long p1, unsigned long p2,
		unsigned long p3, unsigned long p4, unsigned long p5,
		unsigned long p6, struct pt_regs *regs);
asmlinkage long sys_pipe(int __user *fildes);
asmlinkage long sys_pipe2(int __user *fildes, int flags);
asmlinkage long sys_rt_sigaction(int sig,
		const struct sigaction __user *act,
		struct sigaction __user *oact, size_t sigsetsize);
asmlinkage int sys_ipc(uint call, int first, unsigned long second,
		long third, void __user *ptr, long fifth);
asmlinkage long ppc64_personality(unsigned long personality);
asmlinkage int ppc_rtas(struct rtas_args __user *uargs);
asmlinkage time_t sys64_time(time_t __user * tloc);
asmlinkage long ppc_newuname(struct new_utsname __user * name);

asmlinkage long sys_rt_sigsuspend(sigset_t __user *unewset,
		size_t sigsetsize);
asmlinkage long sys_sigaltstack(const stack_t __user *uss,
		stack_t __user *uoss, unsigned long r5, unsigned long r6,
		unsigned long r7, unsigned long r8, struct pt_regs *regs);

#endif /* __KERNEL__ */
#endif /* __ASM_POWERPC_SYSCALLS_H */
