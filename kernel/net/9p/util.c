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
 *  net/9p/util.c
 *
 *  This file contains some helper functions
 *
 *  Copyright (C) 2007 by Latchesar Ionkov <lucho@ionkov.net>
 *  Copyright (C) 2004 by Eric Van Hensbergen <ericvh@gmail.com>
 *  Copyright (C) 2002 by Ron Minnich <rminnich@lanl.gov>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2
 *  as published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to:
 *  Free Software Foundation
 *  51 Franklin Street, Fifth Floor
 *  Boston, MA  02111-1301  USA
 *
 */

#include <linux/module.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/parser.h>
#include <linux/idr.h>
#include <net/9p/9p.h>

/**
 * struct p9_idpool - per-connection accounting for tag idpool
 * @lock: protects the pool
 * @pool: idr to allocate tag id from
 *
 */

struct p9_idpool {
	spinlock_t lock;
	struct idr pool;
};

/**
 * p9_idpool_create - create a new per-connection id pool
 *
 */

struct p9_idpool *p9_idpool_create(void)
{
	struct p9_idpool *p;

	p = kmalloc(sizeof(struct p9_idpool), GFP_KERNEL);
	if (!p)
		return ERR_PTR(-ENOMEM);

	spin_lock_init(&p->lock);
	idr_init(&p->pool);

	return p;
}
EXPORT_SYMBOL(p9_idpool_create);

/**
 * p9_idpool_destroy - create a new per-connection id pool
 * @p: idpool to destory
 */

void p9_idpool_destroy(struct p9_idpool *p)
{
	idr_destroy(&p->pool);
	kfree(p);
}
EXPORT_SYMBOL(p9_idpool_destroy);

/**
 * p9_idpool_get - allocate numeric id from pool
 * @p: pool to allocate from
 *
 * Bugs: This seems to be an awful generic function, should it be in idr.c with
 *            the lock included in struct idr?
 */

int p9_idpool_get(struct p9_idpool *p)
{
	int i = 0;
	int error;
	unsigned long flags;

retry:
	if (idr_pre_get(&p->pool, GFP_KERNEL) == 0)
		return 0;

	spin_lock_irqsave(&p->lock, flags);

	/* no need to store exactly p, we just need something non-null */
	error = idr_get_new(&p->pool, p, &i);
	spin_unlock_irqrestore(&p->lock, flags);

	if (error == -EAGAIN)
		goto retry;
	else if (error)
		return -1;

	P9_DPRINTK(P9_DEBUG_MUX, " id %d pool %p\n", i, p);
	return i;
}
EXPORT_SYMBOL(p9_idpool_get);

/**
 * p9_idpool_put - release numeric id from pool
 * @id: numeric id which is being released
 * @p: pool to release id into
 *
 * Bugs: This seems to be an awful generic function, should it be in idr.c with
 *            the lock included in struct idr?
 */

void p9_idpool_put(int id, struct p9_idpool *p)
{
	unsigned long flags;

	P9_DPRINTK(P9_DEBUG_MUX, " id %d pool %p\n", id, p);

	spin_lock_irqsave(&p->lock, flags);
	idr_remove(&p->pool, id);
	spin_unlock_irqrestore(&p->lock, flags);
}
EXPORT_SYMBOL(p9_idpool_put);

/**
 * p9_idpool_check - check if the specified id is available
 * @id: id to check
 * @p: pool to check
 */

int p9_idpool_check(int id, struct p9_idpool *p)
{
	return idr_find(&p->pool, id) != NULL;
}
EXPORT_SYMBOL(p9_idpool_check);

