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
 * Internal header file for device mapper
 *
 * Copyright (C) 2001, 2002 Sistina Software
 * Copyright (C) 2004-2006 Red Hat, Inc. All rights reserved.
 *
 * This file is released under the LGPL.
 */

#ifndef DM_INTERNAL_H
#define DM_INTERNAL_H

#include <linux/fs.h>
#include <linux/device-mapper.h>
#include <linux/list.h>
#include <linux/blkdev.h>
#include <linux/hdreg.h>

/*
 * Suspend feature flags
 */
#define DM_SUSPEND_LOCKFS_FLAG		(1 << 0)
#define DM_SUSPEND_NOFLUSH_FLAG		(1 << 1)

/*
 * List of devices that a metadevice uses and should open/close.
 */
struct dm_dev_internal {
	struct list_head list;
	atomic_t count;
	struct dm_dev dm_dev;
};

struct dm_table;

/*-----------------------------------------------------------------
 * Internal table functions.
 *---------------------------------------------------------------*/
void dm_table_destroy(struct dm_table *t);
void dm_table_event_callback(struct dm_table *t,
			     void (*fn)(void *), void *context);
struct dm_target *dm_table_get_target(struct dm_table *t, unsigned int index);
struct dm_target *dm_table_find_target(struct dm_table *t, sector_t sector);
void dm_table_set_restrictions(struct dm_table *t, struct request_queue *q);
struct list_head *dm_table_get_devices(struct dm_table *t);
void dm_table_presuspend_targets(struct dm_table *t);
void dm_table_postsuspend_targets(struct dm_table *t);
int dm_table_resume_targets(struct dm_table *t);
int dm_table_any_congested(struct dm_table *t, int bdi_bits);

/*
 * To check the return value from dm_table_find_target().
 */
#define dm_target_is_valid(t) ((t)->table)
int dm_table_barrier_ok(struct dm_table *t);

/*-----------------------------------------------------------------
 * A registry of target types.
 *---------------------------------------------------------------*/
int dm_target_init(void);
void dm_target_exit(void);
struct target_type *dm_get_target_type(const char *name);
void dm_put_target_type(struct target_type *t);
int dm_target_iterate(void (*iter_func)(struct target_type *tt,
					void *param), void *param);

int dm_split_args(int *argc, char ***argvp, char *input);

/*
 * The device-mapper can be driven through one of two interfaces;
 * ioctl or filesystem, depending which patch you have applied.
 */
int dm_interface_init(void);
void dm_interface_exit(void);

/*
 * sysfs interface
 */
int dm_sysfs_init(struct mapped_device *md);
void dm_sysfs_exit(struct mapped_device *md);
struct kobject *dm_kobject(struct mapped_device *md);
struct mapped_device *dm_get_from_kobject(struct kobject *kobj);

/*
 * Targets for linear and striped mappings
 */
int dm_linear_init(void);
void dm_linear_exit(void);

int dm_stripe_init(void);
void dm_stripe_exit(void);

int dm_open_count(struct mapped_device *md);
int dm_lock_for_deletion(struct mapped_device *md);

void dm_kobject_uevent(struct mapped_device *md);

int dm_kcopyd_init(void);
void dm_kcopyd_exit(void);

#endif
