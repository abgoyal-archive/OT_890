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

/* delayacct.h - per-task delay accounting
 *
 * Copyright (C) Shailabh Nagar, IBM Corp. 2006
 *
 * This program is free software;  you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY;  without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
 * the GNU General Public License for more details.
 *
 */

#ifndef _LINUX_DELAYACCT_H
#define _LINUX_DELAYACCT_H

#include <linux/sched.h>
#include <linux/taskstats_kern.h>

/*
 * Per-task flags relevant to delay accounting
 * maintained privately to avoid exhausting similar flags in sched.h:PF_*
 * Used to set current->delays->flags
 */
#define DELAYACCT_PF_SWAPIN	0x00000001	/* I am doing a swapin */
#define DELAYACCT_PF_BLKIO	0x00000002	/* I am waiting on IO */

#ifdef CONFIG_TASK_DELAY_ACCT

extern int delayacct_on;	/* Delay accounting turned on/off */
extern struct kmem_cache *delayacct_cache;
extern void delayacct_init(void);
extern void __delayacct_tsk_init(struct task_struct *);
extern void __delayacct_tsk_exit(struct task_struct *);
extern void __delayacct_blkio_start(void);
extern void __delayacct_blkio_end(void);
extern int __delayacct_add_tsk(struct taskstats *, struct task_struct *);
extern __u64 __delayacct_blkio_ticks(struct task_struct *);
extern void __delayacct_freepages_start(void);
extern void __delayacct_freepages_end(void);

static inline int delayacct_is_task_waiting_on_io(struct task_struct *p)
{
	if (p->delays)
		return (p->delays->flags & DELAYACCT_PF_BLKIO);
	else
		return 0;
}

static inline void delayacct_set_flag(int flag)
{
	if (current->delays)
		current->delays->flags |= flag;
}

static inline void delayacct_clear_flag(int flag)
{
	if (current->delays)
		current->delays->flags &= ~flag;
}

static inline void delayacct_tsk_init(struct task_struct *tsk)
{
	/* reinitialize in case parent's non-null pointer was dup'ed*/
	tsk->delays = NULL;
	if (delayacct_on)
		__delayacct_tsk_init(tsk);
}

/* Free tsk->delays. Called from bad fork and __put_task_struct
 * where there's no risk of tsk->delays being accessed elsewhere
 */
static inline void delayacct_tsk_free(struct task_struct *tsk)
{
	if (tsk->delays)
		kmem_cache_free(delayacct_cache, tsk->delays);
	tsk->delays = NULL;
}

static inline void delayacct_blkio_start(void)
{
	delayacct_set_flag(DELAYACCT_PF_BLKIO);
	if (current->delays)
		__delayacct_blkio_start();
}

static inline void delayacct_blkio_end(void)
{
	if (current->delays)
		__delayacct_blkio_end();
	delayacct_clear_flag(DELAYACCT_PF_BLKIO);
}

static inline int delayacct_add_tsk(struct taskstats *d,
					struct task_struct *tsk)
{
	if (!delayacct_on || !tsk->delays)
		return 0;
	return __delayacct_add_tsk(d, tsk);
}

static inline __u64 delayacct_blkio_ticks(struct task_struct *tsk)
{
	if (tsk->delays)
		return __delayacct_blkio_ticks(tsk);
	return 0;
}

static inline void delayacct_freepages_start(void)
{
	if (current->delays)
		__delayacct_freepages_start();
}

static inline void delayacct_freepages_end(void)
{
	if (current->delays)
		__delayacct_freepages_end();
}

#else
static inline void delayacct_set_flag(int flag)
{}
static inline void delayacct_clear_flag(int flag)
{}
static inline void delayacct_init(void)
{}
static inline void delayacct_tsk_init(struct task_struct *tsk)
{}
static inline void delayacct_tsk_free(struct task_struct *tsk)
{}
static inline void delayacct_blkio_start(void)
{}
static inline void delayacct_blkio_end(void)
{}
static inline int delayacct_add_tsk(struct taskstats *d,
					struct task_struct *tsk)
{ return 0; }
static inline __u64 delayacct_blkio_ticks(struct task_struct *tsk)
{ return 0; }
static inline int delayacct_is_task_waiting_on_io(struct task_struct *p)
{ return 0; }
static inline void delayacct_freepages_start(void)
{}
static inline void delayacct_freepages_end(void)
{}

#endif /* CONFIG_TASK_DELAY_ACCT */

#endif
