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
 * lib/kernel_lock.c
 *
 * This is the traditional BKL - big kernel lock. Largely
 * relegated to obsolescence, but used by various less
 * important (or lazy) subsystems.
 */
#include <linux/smp_lock.h>
#include <linux/module.h>
#include <linux/kallsyms.h>
#include <linux/semaphore.h>

/*
 * The 'big kernel lock'
 *
 * This spinlock is taken and released recursively by lock_kernel()
 * and unlock_kernel().  It is transparently dropped and reacquired
 * over schedule().  It is used to protect legacy code that hasn't
 * been migrated to a proper locking design yet.
 *
 * Don't use in new code.
 */
static  __cacheline_aligned_in_smp DEFINE_SPINLOCK(kernel_flag);


/*
 * Acquire/release the underlying lock from the scheduler.
 *
 * This is called with preemption disabled, and should
 * return an error value if it cannot get the lock and
 * TIF_NEED_RESCHED gets set.
 *
 * If it successfully gets the lock, it should increment
 * the preemption count like any spinlock does.
 *
 * (This works on UP too - _raw_spin_trylock will never
 * return false in that case)
 */
int __lockfunc __reacquire_kernel_lock(void)
{
	while (!_raw_spin_trylock(&kernel_flag)) {
		if (test_thread_flag(TIF_NEED_RESCHED))
			return -EAGAIN;
		cpu_relax();
	}
	preempt_disable();
	return 0;
}

void __lockfunc __release_kernel_lock(void)
{
	_raw_spin_unlock(&kernel_flag);
	preempt_enable_no_resched();
}

/*
 * These are the BKL spinlocks - we try to be polite about preemption.
 * If SMP is not on (ie UP preemption), this all goes away because the
 * _raw_spin_trylock() will always succeed.
 */
#ifdef CONFIG_PREEMPT
static inline void __lock_kernel(void)
{
	preempt_disable();
	if (unlikely(!_raw_spin_trylock(&kernel_flag))) {
		/*
		 * If preemption was disabled even before this
		 * was called, there's nothing we can be polite
		 * about - just spin.
		 */
		if (preempt_count() > 1) {
			_raw_spin_lock(&kernel_flag);
			return;
		}

		/*
		 * Otherwise, let's wait for the kernel lock
		 * with preemption enabled..
		 */
		do {
			preempt_enable();
			while (spin_is_locked(&kernel_flag))
				cpu_relax();
			preempt_disable();
		} while (!_raw_spin_trylock(&kernel_flag));
	}
}

#else

/*
 * Non-preemption case - just get the spinlock
 */
static inline void __lock_kernel(void)
{
	_raw_spin_lock(&kernel_flag);
}
#endif

static inline void __unlock_kernel(void)
{
	/*
	 * the BKL is not covered by lockdep, so we open-code the
	 * unlocking sequence (and thus avoid the dep-chain ops):
	 */
	_raw_spin_unlock(&kernel_flag);
	preempt_enable();
}

/*
 * Getting the big kernel lock.
 *
 * This cannot happen asynchronously, so we only need to
 * worry about other CPU's.
 */
void __lockfunc lock_kernel(void)
{
	int depth = current->lock_depth+1;
	if (likely(!depth))
		__lock_kernel();
	current->lock_depth = depth;
}

void __lockfunc unlock_kernel(void)
{
	BUG_ON(current->lock_depth < 0);
	if (likely(--current->lock_depth < 0))
		__unlock_kernel();
}

EXPORT_SYMBOL(lock_kernel);
EXPORT_SYMBOL(unlock_kernel);

