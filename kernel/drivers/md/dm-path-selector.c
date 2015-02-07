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
 * Copyright (C) 2003 Sistina Software.
 * Copyright (C) 2004 Red Hat, Inc. All rights reserved.
 *
 * Module Author: Heinz Mauelshagen
 *
 * This file is released under the GPL.
 *
 * Path selector registration.
 */

#include <linux/device-mapper.h>

#include "dm-path-selector.h"

#include <linux/slab.h>

struct ps_internal {
	struct path_selector_type pst;

	struct list_head list;
	long use;
};

#define pst_to_psi(__pst) container_of((__pst), struct ps_internal, pst)

static LIST_HEAD(_path_selectors);
static DECLARE_RWSEM(_ps_lock);

static struct ps_internal *__find_path_selector_type(const char *name)
{
	struct ps_internal *psi;

	list_for_each_entry(psi, &_path_selectors, list) {
		if (!strcmp(name, psi->pst.name))
			return psi;
	}

	return NULL;
}

static struct ps_internal *get_path_selector(const char *name)
{
	struct ps_internal *psi;

	down_read(&_ps_lock);
	psi = __find_path_selector_type(name);
	if (psi) {
		if ((psi->use == 0) && !try_module_get(psi->pst.module))
			psi = NULL;
		else
			psi->use++;
	}
	up_read(&_ps_lock);

	return psi;
}

struct path_selector_type *dm_get_path_selector(const char *name)
{
	struct ps_internal *psi;

	if (!name)
		return NULL;

	psi = get_path_selector(name);
	if (!psi) {
		request_module("dm-%s", name);
		psi = get_path_selector(name);
	}

	return psi ? &psi->pst : NULL;
}

void dm_put_path_selector(struct path_selector_type *pst)
{
	struct ps_internal *psi;

	if (!pst)
		return;

	down_read(&_ps_lock);
	psi = __find_path_selector_type(pst->name);
	if (!psi)
		goto out;

	if (--psi->use == 0)
		module_put(psi->pst.module);

	BUG_ON(psi->use < 0);

out:
	up_read(&_ps_lock);
}

static struct ps_internal *_alloc_path_selector(struct path_selector_type *pst)
{
	struct ps_internal *psi = kzalloc(sizeof(*psi), GFP_KERNEL);

	if (psi)
		psi->pst = *pst;

	return psi;
}

int dm_register_path_selector(struct path_selector_type *pst)
{
	int r = 0;
	struct ps_internal *psi = _alloc_path_selector(pst);

	if (!psi)
		return -ENOMEM;

	down_write(&_ps_lock);

	if (__find_path_selector_type(pst->name)) {
		kfree(psi);
		r = -EEXIST;
	} else
		list_add(&psi->list, &_path_selectors);

	up_write(&_ps_lock);

	return r;
}

int dm_unregister_path_selector(struct path_selector_type *pst)
{
	struct ps_internal *psi;

	down_write(&_ps_lock);

	psi = __find_path_selector_type(pst->name);
	if (!psi) {
		up_write(&_ps_lock);
		return -EINVAL;
	}

	if (psi->use) {
		up_write(&_ps_lock);
		return -ETXTBSY;
	}

	list_del(&psi->list);

	up_write(&_ps_lock);

	kfree(psi);

	return 0;
}

EXPORT_SYMBOL_GPL(dm_register_path_selector);
EXPORT_SYMBOL_GPL(dm_unregister_path_selector);
