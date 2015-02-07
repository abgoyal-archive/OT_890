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
 * include/asm-xtensa/syscall.h
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2001 - 2007 Tensilica Inc.
 */

struct pt_regs;
struct sigaction;
asmlinkage long xtensa_execve(char*, char**, char**, struct pt_regs*);
asmlinkage long xtensa_clone(unsigned long, unsigned long, struct pt_regs*);
asmlinkage long xtensa_pipe(int __user *);
asmlinkage long xtensa_mmap2(unsigned long, unsigned long, unsigned long,
    			     unsigned long, unsigned long, unsigned long);
asmlinkage long xtensa_ptrace(long, long, long, long);
asmlinkage long xtensa_sigreturn(struct pt_regs*);
asmlinkage long xtensa_rt_sigreturn(struct pt_regs*);
asmlinkage long xtensa_sigsuspend(struct pt_regs*);
asmlinkage long xtensa_rt_sigsuspend(struct pt_regs*);
asmlinkage long xtensa_sigaction(int, const struct old_sigaction*,
				 struct old_sigaction*);
asmlinkage long xtensa_sigaltstack(struct pt_regs *regs);
asmlinkage long sys_rt_sigaction(int,
				 const struct sigaction __user *,
				 struct sigaction __user *,
				 size_t);
asmlinkage long xtensa_shmat(int, char __user *, int);
asmlinkage long xtensa_fadvise64_64(int, int,
				    unsigned long long, unsigned long long);

/* Should probably move to linux/syscalls.h */
struct pollfd;
asmlinkage long sys_pselect6(int n, fd_set __user *inp, fd_set __user *outp,
	fd_set __user *exp, struct timespec __user *tsp, void __user *sig);
asmlinkage long sys_ppoll(struct pollfd __user *ufds, unsigned int nfds,
	struct timespec __user *tsp, const sigset_t __user *sigmask,
	size_t sigsetsize);


