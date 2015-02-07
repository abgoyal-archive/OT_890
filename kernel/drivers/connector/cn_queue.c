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
 * 	cn_queue.c
 * 
 * 2004-2005 Copyright (c) Evgeniy Polyakov <johnpol@2ka.mipt.ru>
 * All rights reserved.
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/list.h>
#include <linux/workqueue.h>
#include <linux/spinlock.h>
#include <linux/slab.h>
#include <linux/skbuff.h>
#include <linux/suspend.h>
#include <linux/connector.h>
#include <linux/delay.h>

void cn_queue_wrapper(struct work_struct *work)
{
	struct cn_callback_entry *cbq =
		container_of(work, struct cn_callback_entry, work);
	struct cn_callback_data *d = &cbq->data;

	d->callback(d->callback_priv);

	d->destruct_data(d->ddata);
	d->ddata = NULL;

	kfree(d->free);
}

static struct cn_callback_entry *cn_queue_alloc_callback_entry(char *name, struct cb_id *id, void (*callback)(void *))
{
	struct cn_callback_entry *cbq;

	cbq = kzalloc(sizeof(*cbq), GFP_KERNEL);
	if (!cbq) {
		printk(KERN_ERR "Failed to create new callback queue.\n");
		return NULL;
	}

	snprintf(cbq->id.name, sizeof(cbq->id.name), "%s", name);
	memcpy(&cbq->id.id, id, sizeof(struct cb_id));
	cbq->data.callback = callback;
	
	INIT_WORK(&cbq->work, &cn_queue_wrapper);
	return cbq;
}

static void cn_queue_free_callback(struct cn_callback_entry *cbq)
{
	flush_workqueue(cbq->pdev->cn_queue);

	kfree(cbq);
}

int cn_cb_equal(struct cb_id *i1, struct cb_id *i2)
{
	return ((i1->idx == i2->idx) && (i1->val == i2->val));
}

int cn_queue_add_callback(struct cn_queue_dev *dev, char *name, struct cb_id *id, void (*callback)(void *))
{
	struct cn_callback_entry *cbq, *__cbq;
	int found = 0;

	cbq = cn_queue_alloc_callback_entry(name, id, callback);
	if (!cbq)
		return -ENOMEM;

	atomic_inc(&dev->refcnt);
	cbq->pdev = dev;

	spin_lock_bh(&dev->queue_lock);
	list_for_each_entry(__cbq, &dev->queue_list, callback_entry) {
		if (cn_cb_equal(&__cbq->id.id, id)) {
			found = 1;
			break;
		}
	}
	if (!found)
		list_add_tail(&cbq->callback_entry, &dev->queue_list);
	spin_unlock_bh(&dev->queue_lock);

	if (found) {
		cn_queue_free_callback(cbq);
		atomic_dec(&dev->refcnt);
		return -EINVAL;
	}

	cbq->seq = 0;
	cbq->group = cbq->id.id.idx;

	return 0;
}

void cn_queue_del_callback(struct cn_queue_dev *dev, struct cb_id *id)
{
	struct cn_callback_entry *cbq, *n;
	int found = 0;

	spin_lock_bh(&dev->queue_lock);
	list_for_each_entry_safe(cbq, n, &dev->queue_list, callback_entry) {
		if (cn_cb_equal(&cbq->id.id, id)) {
			list_del(&cbq->callback_entry);
			found = 1;
			break;
		}
	}
	spin_unlock_bh(&dev->queue_lock);

	if (found) {
		cn_queue_free_callback(cbq);
		atomic_dec(&dev->refcnt);
	}
}

struct cn_queue_dev *cn_queue_alloc_dev(char *name, struct sock *nls)
{
	struct cn_queue_dev *dev;

	dev = kzalloc(sizeof(*dev), GFP_KERNEL);
	if (!dev)
		return NULL;

	snprintf(dev->name, sizeof(dev->name), "%s", name);
	atomic_set(&dev->refcnt, 0);
	INIT_LIST_HEAD(&dev->queue_list);
	spin_lock_init(&dev->queue_lock);

	dev->nls = nls;

	dev->cn_queue = create_singlethread_workqueue(dev->name);
	if (!dev->cn_queue) {
		kfree(dev);
		return NULL;
	}

	return dev;
}

void cn_queue_free_dev(struct cn_queue_dev *dev)
{
	struct cn_callback_entry *cbq, *n;

	flush_workqueue(dev->cn_queue);
	destroy_workqueue(dev->cn_queue);

	spin_lock_bh(&dev->queue_lock);
	list_for_each_entry_safe(cbq, n, &dev->queue_list, callback_entry)
		list_del(&cbq->callback_entry);
	spin_unlock_bh(&dev->queue_lock);

	while (atomic_read(&dev->refcnt)) {
		printk(KERN_INFO "Waiting for %s to become free: refcnt=%d.\n",
		       dev->name, atomic_read(&dev->refcnt));
		msleep(1000);
	}

	kfree(dev);
	dev = NULL;
}
