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
 * SPU file system -- system call stubs
 *
 * (C) Copyright IBM Deutschland Entwicklung GmbH 2005
 * (C) Copyright 2006-2007, IBM Corporation
 *
 * Author: Arnd Bergmann <arndb@de.ibm.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/syscalls.h>
#include <linux/rcupdate.h>

#include <asm/spu.h>

/* protected by rcu */
static struct spufs_calls *spufs_calls;

#ifdef CONFIG_SPU_FS_MODULE

static inline struct spufs_calls *spufs_calls_get(void)
{
	struct spufs_calls *calls = NULL;

	rcu_read_lock();
	calls = rcu_dereference(spufs_calls);
	if (calls && !try_module_get(calls->owner))
		calls = NULL;
	rcu_read_unlock();

	return calls;
}

static inline void spufs_calls_put(struct spufs_calls *calls)
{
	BUG_ON(calls != spufs_calls);

	/* we don't need to rcu this, as we hold a reference to the module */
	module_put(spufs_calls->owner);
}

#else /* !defined CONFIG_SPU_FS_MODULE */

static inline struct spufs_calls *spufs_calls_get(void)
{
	return spufs_calls;
}

static inline void spufs_calls_put(struct spufs_calls *calls) { }

#endif /* CONFIG_SPU_FS_MODULE */

asmlinkage long sys_spu_create(const char __user *name,
		unsigned int flags, mode_t mode, int neighbor_fd)
{
	long ret;
	struct file *neighbor;
	int fput_needed;
	struct spufs_calls *calls;

	calls = spufs_calls_get();
	if (!calls)
		return -ENOSYS;

	if (flags & SPU_CREATE_AFFINITY_SPU) {
		ret = -EBADF;
		neighbor = fget_light(neighbor_fd, &fput_needed);
		if (neighbor) {
			ret = calls->create_thread(name, flags, mode, neighbor);
			fput_light(neighbor, fput_needed);
		}
	} else
		ret = calls->create_thread(name, flags, mode, NULL);

	spufs_calls_put(calls);
	return ret;
}

asmlinkage long sys_spu_run(int fd, __u32 __user *unpc, __u32 __user *ustatus)
{
	long ret;
	struct file *filp;
	int fput_needed;
	struct spufs_calls *calls;

	calls = spufs_calls_get();
	if (!calls)
		return -ENOSYS;

	ret = -EBADF;
	filp = fget_light(fd, &fput_needed);
	if (filp) {
		ret = calls->spu_run(filp, unpc, ustatus);
		fput_light(filp, fput_needed);
	}

	spufs_calls_put(calls);
	return ret;
}

int elf_coredump_extra_notes_size(void)
{
	struct spufs_calls *calls;
	int ret;

	calls = spufs_calls_get();
	if (!calls)
		return 0;

	ret = calls->coredump_extra_notes_size();

	spufs_calls_put(calls);

	return ret;
}

int elf_coredump_extra_notes_write(struct file *file, loff_t *foffset)
{
	struct spufs_calls *calls;
	int ret;

	calls = spufs_calls_get();
	if (!calls)
		return 0;

	ret = calls->coredump_extra_notes_write(file, foffset);

	spufs_calls_put(calls);

	return ret;
}

void notify_spus_active(void)
{
	struct spufs_calls *calls;

	calls = spufs_calls_get();
	if (!calls)
		return;

	calls->notify_spus_active();
	spufs_calls_put(calls);

	return;
}

int register_spu_syscalls(struct spufs_calls *calls)
{
	if (spufs_calls)
		return -EBUSY;

	rcu_assign_pointer(spufs_calls, calls);
	return 0;
}
EXPORT_SYMBOL_GPL(register_spu_syscalls);

void unregister_spu_syscalls(struct spufs_calls *calls)
{
	BUG_ON(spufs_calls->owner != calls->owner);
	rcu_assign_pointer(spufs_calls, NULL);
	synchronize_rcu();
}
EXPORT_SYMBOL_GPL(unregister_spu_syscalls);
