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

#ifndef __LINUX_DEBUG_LOCKING_H
#define __LINUX_DEBUG_LOCKING_H

#include <linux/kernel.h>

struct task_struct;

extern int debug_locks;
extern int debug_locks_silent;

/*
 * Generic 'turn off all lock debugging' function:
 */
extern int debug_locks_off(void);

#define DEBUG_LOCKS_WARN_ON(c)						\
({									\
	int __ret = 0;							\
									\
	if (!oops_in_progress && unlikely(c)) {				\
		if (debug_locks_off() && !debug_locks_silent)		\
			WARN_ON(1);					\
		__ret = 1;						\
	}								\
	__ret;								\
})

#ifdef CONFIG_SMP
# define SMP_DEBUG_LOCKS_WARN_ON(c)			DEBUG_LOCKS_WARN_ON(c)
#else
# define SMP_DEBUG_LOCKS_WARN_ON(c)			do { } while (0)
#endif

#ifdef CONFIG_DEBUG_LOCKING_API_SELFTESTS
  extern void locking_selftest(void);
#else
# define locking_selftest()	do { } while (0)
#endif

struct task_struct;

#ifdef CONFIG_LOCKDEP
extern void debug_show_all_locks(void);
extern void __debug_show_held_locks(struct task_struct *task);
extern void debug_show_held_locks(struct task_struct *task);
extern void debug_check_no_locks_freed(const void *from, unsigned long len);
extern void debug_check_no_locks_held(struct task_struct *task);
#else
static inline void debug_show_all_locks(void)
{
}

static inline void __debug_show_held_locks(struct task_struct *task)
{
}

static inline void debug_show_held_locks(struct task_struct *task)
{
}

static inline void
debug_check_no_locks_freed(const void *from, unsigned long len)
{
}

static inline void
debug_check_no_locks_held(struct task_struct *task)
{
}
#endif

#endif
