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
 * linux/ipc/namespace.c
 * Copyright (C) 2006 Pavel Emelyanov <xemul@openvz.org> OpenVZ, SWsoft Inc.
 */

#include <linux/ipc.h>
#include <linux/msg.h>
#include <linux/ipc_namespace.h>
#include <linux/rcupdate.h>
#include <linux/nsproxy.h>
#include <linux/slab.h>

#include "util.h"

static struct ipc_namespace *clone_ipc_ns(struct ipc_namespace *old_ns)
{
	struct ipc_namespace *ns;

	ns = kmalloc(sizeof(struct ipc_namespace), GFP_KERNEL);
	if (ns == NULL)
		return ERR_PTR(-ENOMEM);

	atomic_inc(&nr_ipc_ns);

	sem_init_ns(ns);
	msg_init_ns(ns);
	shm_init_ns(ns);

	/*
	 * msgmni has already been computed for the new ipc ns.
	 * Thus, do the ipcns creation notification before registering that
	 * new ipcns in the chain.
	 */
	ipcns_notify(IPCNS_CREATED);
	register_ipcns_notifier(ns);

	kref_init(&ns->kref);
	return ns;
}

struct ipc_namespace *copy_ipcs(unsigned long flags, struct ipc_namespace *ns)
{
	struct ipc_namespace *new_ns;

	BUG_ON(!ns);
	get_ipc_ns(ns);

	if (!(flags & CLONE_NEWIPC))
		return ns;

	new_ns = clone_ipc_ns(ns);

	put_ipc_ns(ns);
	return new_ns;
}

/*
 * free_ipcs - free all ipcs of one type
 * @ns:   the namespace to remove the ipcs from
 * @ids:  the table of ipcs to free
 * @free: the function called to free each individual ipc
 *
 * Called for each kind of ipc when an ipc_namespace exits.
 */
void free_ipcs(struct ipc_namespace *ns, struct ipc_ids *ids,
	       void (*free)(struct ipc_namespace *, struct kern_ipc_perm *))
{
	struct kern_ipc_perm *perm;
	int next_id;
	int total, in_use;

	down_write(&ids->rw_mutex);

	in_use = ids->in_use;

	for (total = 0, next_id = 0; total < in_use; next_id++) {
		perm = idr_find(&ids->ipcs_idr, next_id);
		if (perm == NULL)
			continue;
		ipc_lock_by_ptr(perm);
		free(ns, perm);
		total++;
	}
	up_write(&ids->rw_mutex);
}

void free_ipc_ns(struct kref *kref)
{
	struct ipc_namespace *ns;

	ns = container_of(kref, struct ipc_namespace, kref);
	/*
	 * Unregistering the hotplug notifier at the beginning guarantees
	 * that the ipc namespace won't be freed while we are inside the
	 * callback routine. Since the blocking_notifier_chain_XXX routines
	 * hold a rw lock on the notifier list, unregister_ipcns_notifier()
	 * won't take the rw lock before blocking_notifier_call_chain() has
	 * released the rd lock.
	 */
	unregister_ipcns_notifier(ns);
	sem_exit_ns(ns);
	msg_exit_ns(ns);
	shm_exit_ns(ns);
	kfree(ns);
	atomic_dec(&nr_ipc_ns);

	/*
	 * Do the ipcns removal notification after decrementing nr_ipc_ns in
	 * order to have a correct value when recomputing msgmni.
	 */
	ipcns_notify(IPCNS_REMOVED);
}
