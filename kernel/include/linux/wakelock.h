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

/* include/linux/wakelock.h
 *
 * Copyright (C) 2007-2008 Google, Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef _LINUX_WAKELOCK_H
#define _LINUX_WAKELOCK_H

#include <linux/list.h>
#include <linux/ktime.h>

/* A wake_lock prevents the system from entering suspend or other low power
 * states when active. If the type is set to WAKE_LOCK_SUSPEND, the wake_lock
 * prevents a full system suspend. If the type is WAKE_LOCK_IDLE, low power
 * states that cause large interrupt latencies or that disable a set of
 * interrupts will not entered from idle until the wake_locks are released.
 */

enum {
	WAKE_LOCK_SUSPEND, /* Prevent suspend */
	WAKE_LOCK_IDLE,    /* Prevent low power idle */
	WAKE_LOCK_TYPE_COUNT
};

struct wake_lock {
#ifdef CONFIG_HAS_WAKELOCK
	struct list_head    link;
	int                 flags;
	const char         *name;
	unsigned long       expires;
#ifdef CONFIG_WAKELOCK_STAT
	struct {
		int             count;
		int             expire_count;
		int             wakeup_count;
		ktime_t         total_time;
		ktime_t         prevent_suspend_time;
		ktime_t         max_time;
		ktime_t         last_time;
	} stat;
#endif
#endif
};

#ifdef CONFIG_HAS_WAKELOCK

void wake_lock_init(struct wake_lock *lock, int type, const char *name);
void wake_lock_destroy(struct wake_lock *lock);
void wake_lock(struct wake_lock *lock);
void wake_lock_timeout(struct wake_lock *lock, long timeout);
void wake_unlock(struct wake_lock *lock);

/* wake_lock_active returns a non-zero value if the wake_lock is currently
 * locked. If the wake_lock has a timeout, it does not check the timeout
 * but if the timeout had aready been checked it will return 0.
 */
int wake_lock_active(struct wake_lock *lock);

/* has_wake_lock returns 0 if no wake locks of the specified type are active,
 * and non-zero if one or more wake locks are held. Specifically it returns
 * -1 if one or more wake locks with no timeout are active or the
 * number of jiffies until all active wake locks time out.
 */
long has_wake_lock(int type);

#else

static inline void wake_lock_init(struct wake_lock *lock, int type,
					const char *name) {}
static inline void wake_lock_destroy(struct wake_lock *lock) {}
static inline void wake_lock(struct wake_lock *lock) {}
static inline void wake_lock_timeout(struct wake_lock *lock, long timeout) {}
static inline void wake_unlock(struct wake_lock *lock) {}

static inline int wake_lock_active(struct wake_lock *lock) { return 0; }
static inline long has_wake_lock(int type) { return 0; }

#endif

#endif

