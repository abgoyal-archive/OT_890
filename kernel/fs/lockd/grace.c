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
 * Common code for control of lockd and nfsv4 grace periods.
 */

#include <linux/module.h>
#include <linux/lockd/bind.h>

static LIST_HEAD(grace_list);
static DEFINE_SPINLOCK(grace_lock);

/**
 * locks_start_grace
 * @lm: who this grace period is for
 *
 * A grace period is a period during which locks should not be given
 * out.  Currently grace periods are only enforced by the two lock
 * managers (lockd and nfsd), using the locks_in_grace() function to
 * check when they are in a grace period.
 *
 * This function is called to start a grace period.
 */
void locks_start_grace(struct lock_manager *lm)
{
	spin_lock(&grace_lock);
	list_add(&lm->list, &grace_list);
	spin_unlock(&grace_lock);
}
EXPORT_SYMBOL_GPL(locks_start_grace);

/**
 * locks_end_grace
 * @lm: who this grace period is for
 *
 * Call this function to state that the given lock manager is ready to
 * resume regular locking.  The grace period will not end until all lock
 * managers that called locks_start_grace() also call locks_end_grace().
 * Note that callers count on it being safe to call this more than once,
 * and the second call should be a no-op.
 */
void locks_end_grace(struct lock_manager *lm)
{
	spin_lock(&grace_lock);
	list_del_init(&lm->list);
	spin_unlock(&grace_lock);
}
EXPORT_SYMBOL_GPL(locks_end_grace);

/**
 * locks_in_grace
 *
 * Lock managers call this function to determine when it is OK for them
 * to answer ordinary lock requests, and when they should accept only
 * lock reclaims.
 */
int locks_in_grace(void)
{
	return !list_empty(&grace_list);
}
EXPORT_SYMBOL_GPL(locks_in_grace);
