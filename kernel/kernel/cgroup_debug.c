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
 * kernel/cgroup_debug.c - Example cgroup subsystem that
 * exposes debug info
 *
 * Copyright (C) Google Inc, 2007
 *
 * Developed by Paul Menage (menage@google.com)
 *
 */

#include <linux/cgroup.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/rcupdate.h>

#include <asm/atomic.h>

static struct cgroup_subsys_state *debug_create(struct cgroup_subsys *ss,
						   struct cgroup *cont)
{
	struct cgroup_subsys_state *css = kzalloc(sizeof(*css), GFP_KERNEL);

	if (!css)
		return ERR_PTR(-ENOMEM);

	return css;
}

static void debug_destroy(struct cgroup_subsys *ss, struct cgroup *cont)
{
	kfree(cont->subsys[debug_subsys_id]);
}

static u64 cgroup_refcount_read(struct cgroup *cont, struct cftype *cft)
{
	return atomic_read(&cont->count);
}

static u64 taskcount_read(struct cgroup *cont, struct cftype *cft)
{
	u64 count;

	cgroup_lock();
	count = cgroup_task_count(cont);
	cgroup_unlock();
	return count;
}

static u64 current_css_set_read(struct cgroup *cont, struct cftype *cft)
{
	return (u64)(long)current->cgroups;
}

static u64 current_css_set_refcount_read(struct cgroup *cont,
					   struct cftype *cft)
{
	u64 count;

	rcu_read_lock();
	count = atomic_read(&current->cgroups->refcount);
	rcu_read_unlock();
	return count;
}

static u64 releasable_read(struct cgroup *cgrp, struct cftype *cft)
{
	return test_bit(CGRP_RELEASABLE, &cgrp->flags);
}

static struct cftype files[] =  {
	{
		.name = "cgroup_refcount",
		.read_u64 = cgroup_refcount_read,
	},
	{
		.name = "taskcount",
		.read_u64 = taskcount_read,
	},

	{
		.name = "current_css_set",
		.read_u64 = current_css_set_read,
	},

	{
		.name = "current_css_set_refcount",
		.read_u64 = current_css_set_refcount_read,
	},

	{
		.name = "releasable",
		.read_u64 = releasable_read,
	},
};

static int debug_populate(struct cgroup_subsys *ss, struct cgroup *cont)
{
	return cgroup_add_files(cont, ss, files, ARRAY_SIZE(files));
}

struct cgroup_subsys debug_subsys = {
	.name = "debug",
	.create = debug_create,
	.destroy = debug_destroy,
	.populate = debug_populate,
	.subsys_id = debug_subsys_id,
};
