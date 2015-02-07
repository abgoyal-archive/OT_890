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
 * Copyright (C) Sistina Software, Inc.  1997-2003 All rights reserved.
 * Copyright (C) 2004-2005 Red Hat, Inc.  All rights reserved.
 *
 * This copyrighted material is made available to anyone wishing to use,
 * modify, copy, or redistribute it subject to the terms and conditions
 * of the GNU General Public License version 2.
 */

#include <linux/ctype.h>
#include <linux/stat.h>

#include "lock_dlm.h"

static ssize_t proto_name_show(struct gdlm_ls *ls, char *buf)
{
	return sprintf(buf, "%s\n", gdlm_ops.lm_proto_name);
}

static ssize_t block_show(struct gdlm_ls *ls, char *buf)
{
	ssize_t ret;
	int val = 0;

	if (test_bit(DFL_BLOCK_LOCKS, &ls->flags))
		val = 1;
	ret = sprintf(buf, "%d\n", val);
	return ret;
}

static ssize_t block_store(struct gdlm_ls *ls, const char *buf, size_t len)
{
	ssize_t ret = len;
	int val;

	val = simple_strtol(buf, NULL, 0);

	if (val == 1)
		set_bit(DFL_BLOCK_LOCKS, &ls->flags);
	else if (val == 0) {
		clear_bit(DFL_BLOCK_LOCKS, &ls->flags);
		gdlm_submit_delayed(ls);
	} else {
		ret = -EINVAL;
	}
	return ret;
}

static ssize_t withdraw_show(struct gdlm_ls *ls, char *buf)
{
	ssize_t ret;
	int val = 0;

	if (test_bit(DFL_WITHDRAW, &ls->flags))
		val = 1;
	ret = sprintf(buf, "%d\n", val);
	return ret;
}

static ssize_t withdraw_store(struct gdlm_ls *ls, const char *buf, size_t len)
{
	ssize_t ret = len;
	int val;

	val = simple_strtol(buf, NULL, 0);

	if (val == 1)
		set_bit(DFL_WITHDRAW, &ls->flags);
	else
		ret = -EINVAL;
	wake_up(&ls->wait_control);
	return ret;
}

static ssize_t id_show(struct gdlm_ls *ls, char *buf)
{
	return sprintf(buf, "%u\n", ls->id);
}

static ssize_t jid_show(struct gdlm_ls *ls, char *buf)
{
	return sprintf(buf, "%d\n", ls->jid);
}

static ssize_t first_show(struct gdlm_ls *ls, char *buf)
{
	return sprintf(buf, "%d\n", ls->first);
}

static ssize_t first_done_show(struct gdlm_ls *ls, char *buf)
{
	return sprintf(buf, "%d\n", ls->first_done);
}

static ssize_t recover_show(struct gdlm_ls *ls, char *buf)
{
	return sprintf(buf, "%d\n", ls->recover_jid);
}

static ssize_t recover_store(struct gdlm_ls *ls, const char *buf, size_t len)
{
	ls->recover_jid = simple_strtol(buf, NULL, 0);
	ls->fscb(ls->sdp, LM_CB_NEED_RECOVERY, &ls->recover_jid);
	return len;
}

static ssize_t recover_done_show(struct gdlm_ls *ls, char *buf)
{
	return sprintf(buf, "%d\n", ls->recover_jid_done);
}

static ssize_t recover_status_show(struct gdlm_ls *ls, char *buf)
{
	return sprintf(buf, "%d\n", ls->recover_jid_status);
}

struct gdlm_attr {
	struct attribute attr;
	ssize_t (*show)(struct gdlm_ls *, char *);
	ssize_t (*store)(struct gdlm_ls *, const char *, size_t);
};

#define GDLM_ATTR(_name,_mode,_show,_store) \
static struct gdlm_attr gdlm_attr_##_name = __ATTR(_name,_mode,_show,_store)

GDLM_ATTR(proto_name,     0444, proto_name_show,     NULL);
GDLM_ATTR(block,          0644, block_show,          block_store);
GDLM_ATTR(withdraw,       0644, withdraw_show,       withdraw_store);
GDLM_ATTR(id,             0444, id_show,             NULL);
GDLM_ATTR(jid,            0444, jid_show,            NULL);
GDLM_ATTR(first,          0444, first_show,          NULL);
GDLM_ATTR(first_done,     0444, first_done_show,     NULL);
GDLM_ATTR(recover,        0644, recover_show,        recover_store);
GDLM_ATTR(recover_done,   0444, recover_done_show,   NULL);
GDLM_ATTR(recover_status, 0444, recover_status_show, NULL);

static struct attribute *gdlm_attrs[] = {
	&gdlm_attr_proto_name.attr,
	&gdlm_attr_block.attr,
	&gdlm_attr_withdraw.attr,
	&gdlm_attr_id.attr,
	&gdlm_attr_jid.attr,
	&gdlm_attr_first.attr,
	&gdlm_attr_first_done.attr,
	&gdlm_attr_recover.attr,
	&gdlm_attr_recover_done.attr,
	&gdlm_attr_recover_status.attr,
	NULL,
};

static ssize_t gdlm_attr_show(struct kobject *kobj, struct attribute *attr,
			      char *buf)
{
	struct gdlm_ls *ls = container_of(kobj, struct gdlm_ls, kobj);
	struct gdlm_attr *a = container_of(attr, struct gdlm_attr, attr);
	return a->show ? a->show(ls, buf) : 0;
}

static ssize_t gdlm_attr_store(struct kobject *kobj, struct attribute *attr,
			       const char *buf, size_t len)
{
	struct gdlm_ls *ls = container_of(kobj, struct gdlm_ls, kobj);
	struct gdlm_attr *a = container_of(attr, struct gdlm_attr, attr);
	return a->store ? a->store(ls, buf, len) : len;
}

static struct sysfs_ops gdlm_attr_ops = {
	.show  = gdlm_attr_show,
	.store = gdlm_attr_store,
};

static struct kobj_type gdlm_ktype = {
	.default_attrs = gdlm_attrs,
	.sysfs_ops     = &gdlm_attr_ops,
};

static struct kset *gdlm_kset;

int gdlm_kobject_setup(struct gdlm_ls *ls, struct kobject *fskobj)
{
	int error;

	ls->kobj.kset = gdlm_kset;
	error = kobject_init_and_add(&ls->kobj, &gdlm_ktype, fskobj,
				     "lock_module");
	if (error)
		log_error("can't register kobj %d", error);
	kobject_uevent(&ls->kobj, KOBJ_ADD);

	return error;
}

void gdlm_kobject_release(struct gdlm_ls *ls)
{
	kobject_put(&ls->kobj);
}

static int gdlm_uevent(struct kset *kset, struct kobject *kobj,
		       struct kobj_uevent_env *env)
{
        struct gdlm_ls *ls = container_of(kobj, struct gdlm_ls, kobj);
        add_uevent_var(env, "LOCKTABLE=%s:%s", ls->clustername, ls->fsname);
        add_uevent_var(env, "LOCKPROTO=lock_dlm");
        return 0;
}

static struct kset_uevent_ops gdlm_uevent_ops = {
	.uevent = gdlm_uevent,
};


int gdlm_sysfs_init(void)
{
	gdlm_kset = kset_create_and_add("lock_dlm", &gdlm_uevent_ops, kernel_kobj);
	if (!gdlm_kset) {
		printk(KERN_WARNING "%s: can not create kset\n", __func__);
		return -ENOMEM;
	}
	return 0;
}

void gdlm_sysfs_exit(void)
{
	kset_unregister(gdlm_kset);
}

