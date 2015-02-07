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
 * Copyright (C) 2000 - 2003 Jeff Dike (jdike@addtoit.com)
 * Licensed under the GPL
 */

#include "linux/sched.h"
#include "linux/shm.h"
#include "linux/ipc.h"
#include "linux/syscalls.h"
#include "asm/mman.h"
#include "asm/uaccess.h"
#include "asm/unistd.h"

/*
 * Perform the select(nd, in, out, ex, tv) and mmap() system
 * calls. Linux/i386 didn't use to be able to handle more than
 * 4 system call parameters, so these system calls used a memory
 * block for parameter passing..
 */

struct mmap_arg_struct {
	unsigned long addr;
	unsigned long len;
	unsigned long prot;
	unsigned long flags;
	unsigned long fd;
	unsigned long offset;
};

extern int old_mmap(unsigned long addr, unsigned long len,
		    unsigned long prot, unsigned long flags,
		    unsigned long fd, unsigned long offset);

long old_mmap_i386(struct mmap_arg_struct __user *arg)
{
	struct mmap_arg_struct a;
	int err = -EFAULT;

	if (copy_from_user(&a, arg, sizeof(a)))
		goto out;

	err = old_mmap(a.addr, a.len, a.prot, a.flags, a.fd, a.offset);
 out:
	return err;
}

struct sel_arg_struct {
	unsigned long n;
	fd_set __user *inp;
	fd_set __user *outp;
	fd_set __user *exp;
	struct timeval __user *tvp;
};

long old_select(struct sel_arg_struct __user *arg)
{
	struct sel_arg_struct a;

	if (copy_from_user(&a, arg, sizeof(a)))
		return -EFAULT;
	/* sys_select() does the appropriate kernel locking */
	return sys_select(a.n, a.inp, a.outp, a.exp, a.tvp);
}

/*
 * The prototype on i386 is:
 *
 *     int clone(int flags, void * child_stack, int * parent_tidptr, struct user_desc * newtls, int * child_tidptr)
 *
 * and the "newtls" arg. on i386 is read by copy_thread directly from the
 * register saved on the stack.
 */
long sys_clone(unsigned long clone_flags, unsigned long newsp,
	       int __user *parent_tid, void *newtls, int __user *child_tid)
{
	long ret;

	if (!newsp)
		newsp = UPT_SP(&current->thread.regs.regs);

	current->thread.forking = 1;
	ret = do_fork(clone_flags, newsp, &current->thread.regs, 0, parent_tid,
		      child_tid);
	current->thread.forking = 0;
	return ret;
}

/*
 * sys_ipc() is the de-multiplexer for the SysV IPC calls..
 *
 * This is really horribly ugly.
 */
long sys_ipc (uint call, int first, int second,
	     int third, void __user *ptr, long fifth)
{
	int version, ret;

	version = call >> 16; /* hack for backward compatibility */
	call &= 0xffff;

	switch (call) {
	case SEMOP:
		return sys_semtimedop(first, (struct sembuf __user *) ptr,
				      second, NULL);
	case SEMTIMEDOP:
		return sys_semtimedop(first, (struct sembuf __user *) ptr,
				      second,
				      (const struct timespec __user *) fifth);
	case SEMGET:
		return sys_semget (first, second, third);
	case SEMCTL: {
		union semun fourth;
		if (!ptr)
			return -EINVAL;
		if (get_user(fourth.__pad, (void __user * __user *) ptr))
			return -EFAULT;
		return sys_semctl (first, second, third, fourth);
	}

	case MSGSND:
		return sys_msgsnd (first, (struct msgbuf *) ptr,
				   second, third);
	case MSGRCV:
		switch (version) {
		case 0: {
			struct ipc_kludge tmp;
			if (!ptr)
				return -EINVAL;

			if (copy_from_user(&tmp,
					   (struct ipc_kludge *) ptr,
					   sizeof (tmp)))
				return -EFAULT;
			return sys_msgrcv (first, tmp.msgp, second,
					   tmp.msgtyp, third);
		}
		default:
		        panic("msgrcv with version != 0");
			return sys_msgrcv (first,
					   (struct msgbuf *) ptr,
					   second, fifth, third);
		}
	case MSGGET:
		return sys_msgget ((key_t) first, second);
	case MSGCTL:
		return sys_msgctl (first, second, (struct msqid_ds *) ptr);

	case SHMAT:
		switch (version) {
		default: {
			ulong raddr;
			ret = do_shmat (first, (char *) ptr, second, &raddr);
			if (ret)
				return ret;
			return put_user (raddr, (ulong *) third);
		}
		case 1:	/* iBCS2 emulator entry point */
			if (!segment_eq(get_fs(), get_ds()))
				return -EINVAL;
			return do_shmat (first, (char *) ptr, second, (ulong *) third);
		}
	case SHMDT:
		return sys_shmdt ((char *)ptr);
	case SHMGET:
		return sys_shmget (first, second, third);
	case SHMCTL:
		return sys_shmctl (first, second,
				   (struct shmid_ds *) ptr);
	default:
		return -ENOSYS;
	}
}

long sys_sigaction(int sig, const struct old_sigaction __user *act,
			 struct old_sigaction __user *oact)
{
	struct k_sigaction new_ka, old_ka;
	int ret;

	if (act) {
		old_sigset_t mask;
		if (!access_ok(VERIFY_READ, act, sizeof(*act)) ||
		    __get_user(new_ka.sa.sa_handler, &act->sa_handler) ||
		    __get_user(new_ka.sa.sa_restorer, &act->sa_restorer))
			return -EFAULT;
		__get_user(new_ka.sa.sa_flags, &act->sa_flags);
		__get_user(mask, &act->sa_mask);
		siginitset(&new_ka.sa.sa_mask, mask);
	}

	ret = do_sigaction(sig, act ? &new_ka : NULL, oact ? &old_ka : NULL);

	if (!ret && oact) {
		if (!access_ok(VERIFY_WRITE, oact, sizeof(*oact)) ||
		    __put_user(old_ka.sa.sa_handler, &oact->sa_handler) ||
		    __put_user(old_ka.sa.sa_restorer, &oact->sa_restorer))
			return -EFAULT;
		__put_user(old_ka.sa.sa_flags, &oact->sa_flags);
		__put_user(old_ka.sa.sa_mask.sig[0], &oact->sa_mask);
	}

	return ret;
}
