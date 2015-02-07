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

#ifndef __LINUX_SPINLOCK_API_UP_H
#define __LINUX_SPINLOCK_API_UP_H

#ifndef __LINUX_SPINLOCK_H
# error "please don't include this file directly"
#endif

/*
 * include/linux/spinlock_api_up.h
 *
 * spinlock API implementation on UP-nondebug (inlined implementation)
 *
 * portions Copyright 2005, Red Hat, Inc., Ingo Molnar
 * Released under the General Public License (GPL).
 */

#define in_lock_functions(ADDR)		0

#define assert_spin_locked(lock)	do { (void)(lock); } while (0)

/*
 * In the UP-nondebug case there's no real locking going on, so the
 * only thing we have to do is to keep the preempt counts and irq
 * flags straight, to suppress compiler warnings of unused lock
 * variables, and to add the proper checker annotations:
 */
#define __LOCK(lock) \
  do { preempt_disable(); __acquire(lock); (void)(lock); } while (0)

#define __LOCK_BH(lock) \
  do { local_bh_disable(); __LOCK(lock); } while (0)

#define __LOCK_IRQ(lock) \
  do { local_irq_disable(); __LOCK(lock); } while (0)

#define __LOCK_IRQSAVE(lock, flags) \
  do { local_irq_save(flags); __LOCK(lock); } while (0)

#define __UNLOCK(lock) \
  do { preempt_enable(); __release(lock); (void)(lock); } while (0)

#define __UNLOCK_BH(lock) \
  do { preempt_enable_no_resched(); local_bh_enable(); __release(lock); (void)(lock); } while (0)

#define __UNLOCK_IRQ(lock) \
  do { local_irq_enable(); __UNLOCK(lock); } while (0)

#define __UNLOCK_IRQRESTORE(lock, flags) \
  do { local_irq_restore(flags); __UNLOCK(lock); } while (0)

#define _spin_lock(lock)			__LOCK(lock)
#define _spin_lock_nested(lock, subclass)	__LOCK(lock)
#define _read_lock(lock)			__LOCK(lock)
#define _write_lock(lock)			__LOCK(lock)
#define _spin_lock_bh(lock)			__LOCK_BH(lock)
#define _read_lock_bh(lock)			__LOCK_BH(lock)
#define _write_lock_bh(lock)			__LOCK_BH(lock)
#define _spin_lock_irq(lock)			__LOCK_IRQ(lock)
#define _read_lock_irq(lock)			__LOCK_IRQ(lock)
#define _write_lock_irq(lock)			__LOCK_IRQ(lock)
#define _spin_lock_irqsave(lock, flags)		__LOCK_IRQSAVE(lock, flags)
#define _read_lock_irqsave(lock, flags)		__LOCK_IRQSAVE(lock, flags)
#define _write_lock_irqsave(lock, flags)	__LOCK_IRQSAVE(lock, flags)
#define _spin_trylock(lock)			({ __LOCK(lock); 1; })
#define _read_trylock(lock)			({ __LOCK(lock); 1; })
#define _write_trylock(lock)			({ __LOCK(lock); 1; })
#define _spin_trylock_bh(lock)			({ __LOCK_BH(lock); 1; })
#define _spin_unlock(lock)			__UNLOCK(lock)
#define _read_unlock(lock)			__UNLOCK(lock)
#define _write_unlock(lock)			__UNLOCK(lock)
#define _spin_unlock_bh(lock)			__UNLOCK_BH(lock)
#define _write_unlock_bh(lock)			__UNLOCK_BH(lock)
#define _read_unlock_bh(lock)			__UNLOCK_BH(lock)
#define _spin_unlock_irq(lock)			__UNLOCK_IRQ(lock)
#define _read_unlock_irq(lock)			__UNLOCK_IRQ(lock)
#define _write_unlock_irq(lock)			__UNLOCK_IRQ(lock)
#define _spin_unlock_irqrestore(lock, flags)	__UNLOCK_IRQRESTORE(lock, flags)
#define _read_unlock_irqrestore(lock, flags)	__UNLOCK_IRQRESTORE(lock, flags)
#define _write_unlock_irqrestore(lock, flags)	__UNLOCK_IRQRESTORE(lock, flags)

#endif /* __LINUX_SPINLOCK_API_UP_H */
