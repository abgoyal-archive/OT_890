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
 *	Network event notifiers
 *
 *	Authors:
 *      Tom Tucker             <tom@opengridcomputing.com>
 *      Steve Wise             <swise@opengridcomputing.com>
 *
 *	This program is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU General Public License
 *      as published by the Free Software Foundation; either version
 *      2 of the License, or (at your option) any later version.
 *
 *	Fixes:
 */

#include <linux/rtnetlink.h>
#include <linux/notifier.h>
#include <net/netevent.h>

static ATOMIC_NOTIFIER_HEAD(netevent_notif_chain);

/**
 *	register_netevent_notifier - register a netevent notifier block
 *	@nb: notifier
 *
 *	Register a notifier to be called when a netevent occurs.
 *	The notifier passed is linked into the kernel structures and must
 *	not be reused until it has been unregistered. A negative errno code
 *	is returned on a failure.
 */
int register_netevent_notifier(struct notifier_block *nb)
{
	int err;

	err = atomic_notifier_chain_register(&netevent_notif_chain, nb);
	return err;
}

/**
 *	netevent_unregister_notifier - unregister a netevent notifier block
 *	@nb: notifier
 *
 *	Unregister a notifier previously registered by
 *	register_neigh_notifier(). The notifier is unlinked into the
 *	kernel structures and may then be reused. A negative errno code
 *	is returned on a failure.
 */

int unregister_netevent_notifier(struct notifier_block *nb)
{
	return atomic_notifier_chain_unregister(&netevent_notif_chain, nb);
}

/**
 *	call_netevent_notifiers - call all netevent notifier blocks
 *      @val: value passed unmodified to notifier function
 *      @v:   pointer passed unmodified to notifier function
 *
 *	Call all neighbour notifier blocks.  Parameters and return value
 *	are as for notifier_call_chain().
 */

int call_netevent_notifiers(unsigned long val, void *v)
{
	return atomic_notifier_call_chain(&netevent_notif_chain, val, v);
}

EXPORT_SYMBOL_GPL(register_netevent_notifier);
EXPORT_SYMBOL_GPL(unregister_netevent_notifier);
EXPORT_SYMBOL_GPL(call_netevent_notifiers);
