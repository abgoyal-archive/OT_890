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

#ifndef _ENTRY_H
#define _ENTRY_H

#include <linux/types.h>
#include <linux/signal.h>
#include <asm/ptrace.h>

typedef void pgm_check_handler_t(struct pt_regs *, long);
extern pgm_check_handler_t *pgm_check_table[128];
pgm_check_handler_t do_protection_exception;
pgm_check_handler_t do_dat_exception;

extern int sysctl_userprocess_debug;

void do_single_step(struct pt_regs *regs);
void syscall_trace(struct pt_regs *regs, int entryexit);
void kernel_stack_overflow(struct pt_regs * regs);
void do_signal(struct pt_regs *regs);
int handle_signal32(unsigned long sig, struct k_sigaction *ka,
		    siginfo_t *info, sigset_t *oldset, struct pt_regs *regs);

void do_extint(struct pt_regs *regs, unsigned short code);
int __cpuinit start_secondary(void *cpuvoid);
void __init startup_init(void);
void die(const char * str, struct pt_regs * regs, long err);

struct new_utsname;
struct mmap_arg_struct;
struct fadvise64_64_args;
struct old_sigaction;
struct sel_arg_struct;

long sys_mmap2(struct mmap_arg_struct __user  *arg);
long sys_s390_old_mmap(struct mmap_arg_struct __user *arg);
long sys_ipc(uint call, int first, unsigned long second,
	     unsigned long third, void __user *ptr);
long sys_s390_newuname(struct new_utsname __user *name);
long sys_s390_personality(unsigned long personality);
long sys_s390_fadvise64(int fd, u32 offset_high, u32 offset_low,
		    size_t len, int advice);
long sys_s390_fadvise64_64(struct fadvise64_64_args __user *args);
long sys_s390_fallocate(int fd, int mode, loff_t offset, u32 len_high,
			u32 len_low);
long sys_fork(void);
long sys_clone(void);
long sys_vfork(void);
void execve_tail(void);
long sys_execve(void);
long sys_sigsuspend(int history0, int history1, old_sigset_t mask);
long sys_sigaction(int sig, const struct old_sigaction __user *act,
		   struct old_sigaction __user *oact);
long sys_sigaltstack(const stack_t __user *uss, stack_t __user *uoss);
long sys_sigreturn(void);
long sys_rt_sigreturn(void);
long sys32_sigreturn(void);
long sys32_rt_sigreturn(void);

#endif /* _ENTRY_H */
