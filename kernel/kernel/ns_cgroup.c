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
 * ns_cgroup.c - namespace cgroup subsystem
 *
 * Copyright 2006, 2007 IBM Corp
 */

#include <linux/module.h>
#include <linux/cgroup.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/slab.h>
#include <linux/nsproxy.h>

struct ns_cgroup {
	struct cgroup_subsys_state css;
};

struct cgroup_subsys ns_subsys;

static inline struct ns_cgroup *cgroup_to_ns(
		struct cgroup *cgroup)
{
	return container_of(cgroup_subsys_state(cgroup, ns_subsys_id),
			    struct ns_cgroup, css);
}

int ns_cgroup_clone(struct task_struct *task, struct pid *pid)
{
	char name[PROC_NUMBUF];

	snprintf(name, PROC_NUMBUF, "%d", pid_vnr(pid));
	return cgroup_clone(task, &ns_subsys, name);
}

/*
 * Rules:
 *   1. you can only enter a cgroup which is a child of your current
 *     cgroup
 *   2. you can only place another process into a cgroup if
 *     a. you have CAP_SYS_ADMIN
 *     b. your cgroup is an ancestor of task's destination cgroup
 *       (hence either you are in the same cgroup as task, or in an
 *        ancestor cgroup thereof)
 */
static int ns_can_attach(struct cgroup_subsys *ss,
		struct cgroup *new_cgroup, struct task_struct *task)
{
	struct cgroup *orig;

	if (current != task) {
		if (!capable(CAP_SYS_ADMIN))
			return -EPERM;

		if (!cgroup_is_descendant(new_cgroup))
			return -EPERM;
	}

	if (atomic_read(&new_cgroup->count) != 0)
		return -EPERM;

	orig = task_cgroup(task, ns_subsys_id);
	if (orig && orig != new_cgroup->parent)
		return -EPERM;

	return 0;
}

/*
 * Rules: you can only create a cgroup if
 *     1. you are capable(CAP_SYS_ADMIN)
 *     2. the target cgroup is a descendant of your own cgroup
 */
static struct cgroup_subsys_state *ns_create(struct cgroup_subsys *ss,
						struct cgroup *cgroup)
{
	struct ns_cgroup *ns_cgroup;

	if (!capable(CAP_SYS_ADMIN))
		return ERR_PTR(-EPERM);
	if (!cgroup_is_descendant(cgroup))
		return ERR_PTR(-EPERM);

	ns_cgroup = kzalloc(sizeof(*ns_cgroup), GFP_KERNEL);
	if (!ns_cgroup)
		return ERR_PTR(-ENOMEM);
	return &ns_cgroup->css;
}

static void ns_destroy(struct cgroup_subsys *ss,
			struct cgroup *cgroup)
{
	struct ns_cgroup *ns_cgroup;

	ns_cgroup = cgroup_to_ns(cgroup);
	kfree(ns_cgroup);
}

struct cgroup_subsys ns_subsys = {
	.name = "ns",
	.can_attach = ns_can_attach,
	.create = ns_create,
	.destroy  = ns_destroy,
	.subsys_id = ns_subsys_id,
};
