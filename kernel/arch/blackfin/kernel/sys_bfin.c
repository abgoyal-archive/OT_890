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
 * File:         arch/blackfin/kernel/sys_bfin.c
 * Based on:
 * Author:
 *
 * Created:
 * Description:  This file contains various random system calls that
 *               have a non-standard calling sequence on the Linux/bfin
 *               platform.
 *
 * Modified:
 *               Copyright 2004-2006 Analog Devices Inc.
 *
 * Bugs:         Enter bugs at http://blackfin.uclinux.org/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see the file COPYING, or write
 * to the Free Software Foundation, Inc.,
 * 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <linux/smp_lock.h>
#include <linux/spinlock.h>
#include <linux/sem.h>
#include <linux/msg.h>
#include <linux/shm.h>
#include <linux/syscalls.h>
#include <linux/mman.h>
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/ipc.h>
#include <linux/unistd.h>

#include <asm/cacheflush.h>
#include <asm/dma.h>

/* common code for old and new mmaps */
static inline long
do_mmap2(unsigned long addr, unsigned long len,
	 unsigned long prot, unsigned long flags,
	 unsigned long fd, unsigned long pgoff)
{
	int error = -EBADF;
	struct file *file = NULL;

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

asmlinkage long sys_mmap2(unsigned long addr, unsigned long len,
			  unsigned long prot, unsigned long flags,
			  unsigned long fd, unsigned long pgoff)
{
	return do_mmap2(addr, len, prot, flags, fd, pgoff);
}

asmlinkage int sys_getpagesize(void)
{
	return PAGE_SIZE;
}

asmlinkage void *sys_sram_alloc(size_t size, unsigned long flags)
{
	return sram_alloc_with_lsl(size, flags);
}

asmlinkage int sys_sram_free(const void *addr)
{
	return sram_free_with_lsl(addr);
}

asmlinkage void *sys_dma_memcpy(void *dest, const void *src, size_t len)
{
	return safe_dma_memcpy(dest, src, len);
}
