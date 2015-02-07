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
 * arch/xtensa/kernel/syscall.c
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2001 - 2005 Tensilica Inc.
 * Copyright (C) 2000 Silicon Graphics, Inc.
 * Copyright (C) 1995 - 2000 by Ralf Baechle
 *
 * Joe Taylor <joe@tensilica.com, joetylr@yahoo.com>
 * Marc Gauthier <marc@tensilica.com, marc@alumni.uwaterloo.ca>
 * Chris Zankel <chris@zankel.net>
 * Kevin Chea
 *
 */
#include <asm/uaccess.h>
#include <asm/syscall.h>
#include <asm/unistd.h>
#include <linux/linkage.h>
#include <linux/stringify.h>
#include <linux/errno.h>
#include <linux/syscalls.h>
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/mman.h>
#include <linux/shm.h>

typedef void (*syscall_t)(void);

syscall_t sys_call_table[__NR_syscall_count] /* FIXME __cacheline_aligned */= {
	[0 ... __NR_syscall_count - 1] = (syscall_t)&sys_ni_syscall,

#undef __SYSCALL
#define __SYSCALL(nr,symbol,nargs) [ nr ] = (syscall_t)symbol,
#undef _XTENSA_UNISTD_H
#undef  __KERNEL_SYSCALLS__
#include <asm/unistd.h>
};

/*
 * xtensa_pipe() is the normal C calling standard for creating a pipe. It's not
 * the way unix traditional does this, though.
 */

asmlinkage long xtensa_pipe(int __user *userfds)
{
	int fd[2];
	int error;

	error = do_pipe_flags(fd, 0);
	if (!error) {
		if (copy_to_user(userfds, fd, 2 * sizeof(int)))
			error = -EFAULT;
	}
	return error;
}


asmlinkage long xtensa_mmap2(unsigned long addr, unsigned long len,
   			     unsigned long prot, unsigned long flags,
			     unsigned long fd, unsigned long pgoff)
{
	int error = -EBADF;
	struct file * file = NULL;

	flags &= ~(MAP_EXECUTABLE | MAP_DENYWRITE);
	if (!(flags & MAP_ANONYMOUS)) {
		file = fget(fd);
		if (!file)
			goto out;
	}

	down_write(&current->mm->mmap_sem);
	error = do_mmap_pgoff(file, addr, len, prot, flags, pgoff);
	up_write(&current->mm->mmap_sem);

	if (file)
		fput(file);
out:
	return error;
}

asmlinkage long xtensa_shmat(int shmid, char __user *shmaddr, int shmflg)
{
	unsigned long ret;
	long err;

	err = do_shmat(shmid, shmaddr, shmflg, &ret);
	if (err)
		return err;
	return (long)ret;
}

asmlinkage long xtensa_fadvise64_64(int fd, int advice, unsigned long long offset, unsigned long long len)
{
	return sys_fadvise64_64(fd, offset, len, advice);
}

