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

#ifndef __IPC_NAMESPACE_H__
#define __IPC_NAMESPACE_H__

#include <linux/err.h>
#include <linux/idr.h>
#include <linux/rwsem.h>
#include <linux/notifier.h>

/*
 * ipc namespace events
 */
#define IPCNS_MEMCHANGED   0x00000001   /* Notify lowmem size changed */
#define IPCNS_CREATED  0x00000002   /* Notify new ipc namespace created */
#define IPCNS_REMOVED  0x00000003   /* Notify ipc namespace removed */

#define IPCNS_CALLBACK_PRI 0


struct ipc_ids {
	int in_use;
	unsigned short seq;
	unsigned short seq_max;
	struct rw_semaphore rw_mutex;
	struct idr ipcs_idr;
};

struct ipc_namespace {
	struct kref	kref;
	struct ipc_ids	ids[3];

	int		sem_ctls[4];
	int		used_sems;

	int		msg_ctlmax;
	int		msg_ctlmnb;
	int		msg_ctlmni;
	atomic_t	msg_bytes;
	atomic_t	msg_hdrs;
	int		auto_msgmni;

	size_t		shm_ctlmax;
	size_t		shm_ctlall;
	int		shm_ctlmni;
	int		shm_tot;

	struct notifier_block ipcns_nb;
};

extern struct ipc_namespace init_ipc_ns;
extern atomic_t nr_ipc_ns;

#ifdef CONFIG_SYSVIPC
#define INIT_IPC_NS(ns)		.ns		= &init_ipc_ns,

extern int register_ipcns_notifier(struct ipc_namespace *);
extern int cond_register_ipcns_notifier(struct ipc_namespace *);
extern void unregister_ipcns_notifier(struct ipc_namespace *);
extern int ipcns_notify(unsigned long);

#else /* CONFIG_SYSVIPC */
#define INIT_IPC_NS(ns)
#endif /* CONFIG_SYSVIPC */

#if defined(CONFIG_SYSVIPC) && defined(CONFIG_IPC_NS)
extern void free_ipc_ns(struct kref *kref);
extern struct ipc_namespace *copy_ipcs(unsigned long flags,
				       struct ipc_namespace *ns);
extern void free_ipcs(struct ipc_namespace *ns, struct ipc_ids *ids,
		      void (*free)(struct ipc_namespace *,
				   struct kern_ipc_perm *));

static inline struct ipc_namespace *get_ipc_ns(struct ipc_namespace *ns)
{
	if (ns)
		kref_get(&ns->kref);
	return ns;
}

static inline void put_ipc_ns(struct ipc_namespace *ns)
{
	kref_put(&ns->kref, free_ipc_ns);
}
#else
static inline struct ipc_namespace *copy_ipcs(unsigned long flags,
		struct ipc_namespace *ns)
{
	if (flags & CLONE_NEWIPC)
		return ERR_PTR(-EINVAL);

	return ns;
}

static inline struct ipc_namespace *get_ipc_ns(struct ipc_namespace *ns)
{
	return ns;
}

static inline void put_ipc_ns(struct ipc_namespace *ns)
{
}
#endif
#endif
