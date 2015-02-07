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
 *    Copyright (c) 2007 Benjamin Herrenschmidt, IBM Coproration
 *    Extracted from signal_32.c and signal_64.c
 *
 * This file is subject to the terms and conditions of the GNU General
 * Public License.  See the file README.legal in the main directory of
 * this archive for more details.
 */

#ifndef _POWERPC_ARCH_SIGNAL_H
#define _POWERPC_ARCH_SIGNAL_H

#define _BLOCKABLE (~(sigmask(SIGKILL) | sigmask(SIGSTOP)))

extern void do_signal(struct pt_regs *regs, unsigned long thread_info_flags);

extern void __user * get_sigframe(struct k_sigaction *ka, struct pt_regs *regs,
				  size_t frame_size);
extern void restore_sigmask(sigset_t *set);

extern int handle_signal32(unsigned long sig, struct k_sigaction *ka,
			   siginfo_t *info, sigset_t *oldset,
			   struct pt_regs *regs);

extern int handle_rt_signal32(unsigned long sig, struct k_sigaction *ka,
			      siginfo_t *info, sigset_t *oldset,
			      struct pt_regs *regs);

extern unsigned long copy_fpr_to_user(void __user *to,
				      struct task_struct *task);
extern unsigned long copy_fpr_from_user(struct task_struct *task,
					void __user *from);
#ifdef CONFIG_VSX
extern unsigned long copy_vsx_to_user(void __user *to,
				      struct task_struct *task);
extern unsigned long copy_vsx_from_user(struct task_struct *task,
					void __user *from);
#endif

#ifdef CONFIG_PPC64

static inline int is_32bit_task(void)
{
	return test_thread_flag(TIF_32BIT);
}

extern int handle_rt_signal64(int signr, struct k_sigaction *ka,
			      siginfo_t *info, sigset_t *set,
			      struct pt_regs *regs);

#else /* CONFIG_PPC64 */

static inline int is_32bit_task(void)
{
	return 1;
}

static inline int handle_rt_signal64(int signr, struct k_sigaction *ka,
				     siginfo_t *info, sigset_t *set,
				     struct pt_regs *regs)
{
	return -EFAULT;
}

#endif /* !defined(CONFIG_PPC64) */

#endif  /* _POWERPC_ARCH_SIGNAL_H */
