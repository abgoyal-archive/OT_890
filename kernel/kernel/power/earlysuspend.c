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
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

/* kernel/power/earlysuspend.c
 *
 * Copyright (C) 2005-2008 Google, Inc.
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

#include <linux/earlysuspend.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/rtc.h>
#include <linux/syscalls.h> /* sys_sync */
#include <linux/wakelock.h>
#include <linux/workqueue.h>

#include "power.h"

enum {
	DEBUG_USER_STATE = 1U << 0,
	DEBUG_SUSPEND = 1U << 2,
};
int Earlysuspend_debug_mask = DEBUG_USER_STATE;
module_param_named(Earlysuspend_debug_mask, Earlysuspend_debug_mask, int, S_IRUGO | S_IWUSR | S_IWGRP);

static DEFINE_MUTEX(early_suspend_lock);
static LIST_HEAD(early_suspend_handlers);
static void early_suspend(struct work_struct *work);
static void late_resume(struct work_struct *work);
static DECLARE_WORK(early_suspend_work, early_suspend);
static DECLARE_WORK(late_resume_work, late_resume);
static DEFINE_SPINLOCK(state_lock);
enum {
	SUSPEND_REQUESTED = 0x1,
	SUSPENDED = 0x2,
	SUSPEND_REQUESTED_AND_SUSPENDED = SUSPEND_REQUESTED | SUSPENDED,
};
static int state;

static DECLARE_COMPLETION(fb_drv_ready);

void register_early_suspend(struct early_suspend *handler)
{
	struct list_head *pos;

	mutex_lock(&early_suspend_lock);
	list_for_each(pos, &early_suspend_handlers) {
		struct early_suspend *e;
		e = list_entry(pos, struct early_suspend, link);
		if (e->level > handler->level)
			break;
	}
	list_add_tail(&handler->link, pos);
	if ((state & SUSPENDED) && handler->suspend)
		handler->suspend(handler);
	mutex_unlock(&early_suspend_lock);
}
EXPORT_SYMBOL(register_early_suspend);

void unregister_early_suspend(struct early_suspend *handler)
{
	mutex_lock(&early_suspend_lock);
	list_del(&handler->link);
	mutex_unlock(&early_suspend_lock);
}
EXPORT_SYMBOL(unregister_early_suspend);

static void early_suspend(struct work_struct *work)
{
	struct early_suspend *pos;
	unsigned long irqflags;
	int abort = 0;

	mutex_lock(&early_suspend_lock);
	spin_lock_irqsave(&state_lock, irqflags);
	if (state == SUSPEND_REQUESTED)
		state |= SUSPENDED;
	else
		abort = 1;
	spin_unlock_irqrestore(&state_lock, irqflags);

	if (abort) {
		if (Earlysuspend_debug_mask & DEBUG_SUSPEND)
			pr_info("[early_suspend]: abort, state %d\n", state);
		mutex_unlock(&early_suspend_lock);
		goto abort;
	}

	if (Earlysuspend_debug_mask & DEBUG_SUSPEND)
		pr_info("[early_suspend]: call handlers\n");
	list_for_each_entry(pos, &early_suspend_handlers, link) {
		if (pos->suspend != NULL)
			pos->suspend(pos);
	}
	mutex_unlock(&early_suspend_lock);

	if (Earlysuspend_debug_mask & DEBUG_SUSPEND)
		pr_info("[early_suspend]: sync\n");

	sys_sync();
abort:
	spin_lock_irqsave(&state_lock, irqflags);
	if (state == SUSPEND_REQUESTED_AND_SUSPENDED)
	{
		pr_info("[early_suspend]: wake_unlock(main)\n");
		wake_unlock(&main_wake_lock);
	}
	spin_unlock_irqrestore(&state_lock, irqflags);
}

static void late_resume(struct work_struct *work)
{
	struct early_suspend *pos;
	unsigned long irqflags;
	int abort = 0;
    int completed = 0;

	mutex_lock(&early_suspend_lock);
	spin_lock_irqsave(&state_lock, irqflags);

    // return back from suspend
	if (state == SUSPENDED)
		state &= ~SUSPENDED;
	else
		abort = 1;
	spin_unlock_irqrestore(&state_lock, irqflags);

	if (abort) {
		if (Earlysuspend_debug_mask & DEBUG_SUSPEND)
			pr_info("[late_resume]: abort, state %d\n", state);
		goto abort;
	}
	if (Earlysuspend_debug_mask & DEBUG_SUSPEND)
		pr_info("[late_resume]: call handlers\n");
	list_for_each_entry_reverse(pos, &early_suspend_handlers, link)
    {
        if (!completed && pos->level < EARLY_SUSPEND_LEVEL_DISABLE_FB) {
            complete(&fb_drv_ready);
            completed = 1;
        }
		if (pos->resume != NULL)
			pos->resume(pos);
    }
	if (Earlysuspend_debug_mask & DEBUG_SUSPEND)
		pr_info("[late_resume]: done\n");
abort:
    if (!completed)
        complete(&fb_drv_ready);
  	mutex_unlock(&early_suspend_lock);
}


void request_suspend_state(suspend_state_t new_state)
{
	unsigned long irqflags;
	int old_sleep;

	spin_lock_irqsave(&state_lock, irqflags);
	old_sleep = state & SUSPEND_REQUESTED;
	if (Earlysuspend_debug_mask & DEBUG_USER_STATE) {
		struct timespec ts;
		struct rtc_time tm;
		getnstimeofday(&ts);
		rtc_time_to_tm(ts.tv_sec, &tm);
		pr_info("[request_suspend_state]: %s (%d->%d) at %lld "
			"(%d-%02d-%02d %02d:%02d:%02d.%09lu UTC)\n",
			new_state != PM_SUSPEND_ON ? "sleep" : "wakeup",
			requested_suspend_state, new_state,
			ktime_to_ns(ktime_get()),
			tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
			tm.tm_hour, tm.tm_min, tm.tm_sec, ts.tv_nsec);
	}
	// ready to enter earlysuspend
	if (!old_sleep && new_state != PM_SUSPEND_ON) {
		state |= SUSPEND_REQUESTED;
		pr_info("[request_suspend_state]: suspend_work_queue early_suspend_work\n");
		queue_work(suspend_work_queue, &early_suspend_work);
	}
	// ready to enter lateresume
	else if (old_sleep && new_state == PM_SUSPEND_ON) {
		state &= ~SUSPEND_REQUESTED;
		wake_lock(&main_wake_lock);
		pr_info("[request_suspend_state]: suspend_work_queue late_resume_work\n" );
		if (queue_work(suspend_work_queue, &late_resume_work)) {
            //
            //  In order to synchronize the backlight turn on timing,
            //  block the thread and wait for fb driver late_resume()
    		//  callback function is completed
    		//
            wait_for_completion(&fb_drv_ready);
        }
	}
	requested_suspend_state = new_state;
	spin_unlock_irqrestore(&state_lock, irqflags);
}

suspend_state_t get_suspend_state(void)
{
	return requested_suspend_state;
}
