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

/* kernel/rwsem.c: R/W semaphores, public implementation
 *
 * Written by David Howells (dhowells@redhat.com).
 * Derived from asm-i386/semaphore.h
 */

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/rwsem.h>

#include <asm/system.h>
#include <asm/atomic.h>

/*
 * lock for reading
 */
void __sched down_read(struct rw_semaphore *sem)
{
	might_sleep();
	rwsem_acquire_read(&sem->dep_map, 0, 0, _RET_IP_);

	LOCK_CONTENDED(sem, __down_read_trylock, __down_read);
}

EXPORT_SYMBOL(down_read);

/*
 * trylock for reading -- returns 1 if successful, 0 if contention
 */
int down_read_trylock(struct rw_semaphore *sem)
{
	int ret = __down_read_trylock(sem);

	if (ret == 1)
		rwsem_acquire_read(&sem->dep_map, 0, 1, _RET_IP_);
	return ret;
}

EXPORT_SYMBOL(down_read_trylock);

/*
 * lock for writing
 */
void __sched down_write(struct rw_semaphore *sem)
{
	might_sleep();
	rwsem_acquire(&sem->dep_map, 0, 0, _RET_IP_);

	LOCK_CONTENDED(sem, __down_write_trylock, __down_write);
}

EXPORT_SYMBOL(down_write);

/*
 * trylock for writing -- returns 1 if successful, 0 if contention
 */
int down_write_trylock(struct rw_semaphore *sem)
{
	int ret = __down_write_trylock(sem);

	if (ret == 1)
		rwsem_acquire(&sem->dep_map, 0, 1, _RET_IP_);
	return ret;
}

EXPORT_SYMBOL(down_write_trylock);

/*
 * release a read lock
 */
void up_read(struct rw_semaphore *sem)
{
	rwsem_release(&sem->dep_map, 1, _RET_IP_);

	__up_read(sem);
}

EXPORT_SYMBOL(up_read);

/*
 * release a write lock
 */
void up_write(struct rw_semaphore *sem)
{
	rwsem_release(&sem->dep_map, 1, _RET_IP_);

	__up_write(sem);
}

EXPORT_SYMBOL(up_write);

/*
 * downgrade write lock to read lock
 */
void downgrade_write(struct rw_semaphore *sem)
{
	/*
	 * lockdep: a downgraded write will live on as a write
	 * dependency.
	 */
	__downgrade_write(sem);
}

EXPORT_SYMBOL(downgrade_write);

#ifdef CONFIG_DEBUG_LOCK_ALLOC

void down_read_nested(struct rw_semaphore *sem, int subclass)
{
	might_sleep();
	rwsem_acquire_read(&sem->dep_map, subclass, 0, _RET_IP_);

	LOCK_CONTENDED(sem, __down_read_trylock, __down_read);
}

EXPORT_SYMBOL(down_read_nested);

void down_read_non_owner(struct rw_semaphore *sem)
{
	might_sleep();

	__down_read(sem);
}

EXPORT_SYMBOL(down_read_non_owner);

void down_write_nested(struct rw_semaphore *sem, int subclass)
{
	might_sleep();
	rwsem_acquire(&sem->dep_map, subclass, 0, _RET_IP_);

	LOCK_CONTENDED(sem, __down_write_trylock, __down_write);
}

EXPORT_SYMBOL(down_write_nested);

void up_read_non_owner(struct rw_semaphore *sem)
{
	__up_read(sem);
}

EXPORT_SYMBOL(up_read_non_owner);

#endif


