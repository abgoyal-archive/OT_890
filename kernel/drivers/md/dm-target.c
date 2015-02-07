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
 * Copyright (C) 2001 Sistina Software (UK) Limited
 *
 * This file is released under the GPL.
 */

#include "dm.h"

#include <linux/module.h>
#include <linux/init.h>
#include <linux/kmod.h>
#include <linux/bio.h>
#include <linux/slab.h>

#define DM_MSG_PREFIX "target"

struct tt_internal {
	struct target_type tt;

	struct list_head list;
	long use;
};

static LIST_HEAD(_targets);
static DECLARE_RWSEM(_lock);

#define DM_MOD_NAME_SIZE 32

static inline struct tt_internal *__find_target_type(const char *name)
{
	struct tt_internal *ti;

	list_for_each_entry (ti, &_targets, list)
		if (!strcmp(name, ti->tt.name))
			return ti;

	return NULL;
}

static struct tt_internal *get_target_type(const char *name)
{
	struct tt_internal *ti;

	down_read(&_lock);

	ti = __find_target_type(name);
	if (ti) {
		if ((ti->use == 0) && !try_module_get(ti->tt.module))
			ti = NULL;
		else
			ti->use++;
	}

	up_read(&_lock);
	return ti;
}

static void load_module(const char *name)
{
	request_module("dm-%s", name);
}

struct target_type *dm_get_target_type(const char *name)
{
	struct tt_internal *ti = get_target_type(name);

	if (!ti) {
		load_module(name);
		ti = get_target_type(name);
	}

	return ti ? &ti->tt : NULL;
}

void dm_put_target_type(struct target_type *t)
{
	struct tt_internal *ti = (struct tt_internal *) t;

	down_read(&_lock);
	if (--ti->use == 0)
		module_put(ti->tt.module);

	BUG_ON(ti->use < 0);
	up_read(&_lock);

	return;
}

static struct tt_internal *alloc_target(struct target_type *t)
{
	struct tt_internal *ti = kzalloc(sizeof(*ti), GFP_KERNEL);

	if (ti)
		ti->tt = *t;

	return ti;
}


int dm_target_iterate(void (*iter_func)(struct target_type *tt,
					void *param), void *param)
{
	struct tt_internal *ti;

	down_read(&_lock);
	list_for_each_entry (ti, &_targets, list)
		iter_func(&ti->tt, param);
	up_read(&_lock);

	return 0;
}

int dm_register_target(struct target_type *t)
{
	int rv = 0;
	struct tt_internal *ti = alloc_target(t);

	if (!ti)
		return -ENOMEM;

	down_write(&_lock);
	if (__find_target_type(t->name))
		rv = -EEXIST;
	else
		list_add(&ti->list, &_targets);

	up_write(&_lock);
	if (rv)
		kfree(ti);
	return rv;
}

void dm_unregister_target(struct target_type *t)
{
	struct tt_internal *ti;

	down_write(&_lock);
	if (!(ti = __find_target_type(t->name))) {
		DMCRIT("Unregistering unrecognised target: %s", t->name);
		BUG();
	}

	if (ti->use) {
		DMCRIT("Attempt to unregister target still in use: %s",
		       t->name);
		BUG();
	}

	list_del(&ti->list);
	kfree(ti);

	up_write(&_lock);
}

/*
 * io-err: always fails an io, useful for bringing
 * up LVs that have holes in them.
 */
static int io_err_ctr(struct dm_target *ti, unsigned int argc, char **args)
{
	return 0;
}

static void io_err_dtr(struct dm_target *ti)
{
	/* empty */
}

static int io_err_map(struct dm_target *ti, struct bio *bio,
		      union map_info *map_context)
{
	return -EIO;
}

static struct target_type error_target = {
	.name = "error",
	.version = {1, 0, 1},
	.ctr  = io_err_ctr,
	.dtr  = io_err_dtr,
	.map  = io_err_map,
};

int __init dm_target_init(void)
{
	return dm_register_target(&error_target);
}

void dm_target_exit(void)
{
	dm_unregister_target(&error_target);
}

EXPORT_SYMBOL(dm_register_target);
EXPORT_SYMBOL(dm_unregister_target);
