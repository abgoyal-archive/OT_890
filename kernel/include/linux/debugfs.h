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
 *  debugfs.h - a tiny little debug file system
 *
 *  Copyright (C) 2004 Greg Kroah-Hartman <greg@kroah.com>
 *  Copyright (C) 2004 IBM Inc.
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License version
 *	2 as published by the Free Software Foundation.
 *
 *  debugfs is for people to use instead of /proc or /sys.
 *  See Documentation/DocBook/kernel-api for more details.
 */

#ifndef _DEBUGFS_H_
#define _DEBUGFS_H_

#include <linux/fs.h>

#include <linux/types.h>

struct file_operations;

struct debugfs_blob_wrapper {
	void *data;
	unsigned long size;
};

extern struct dentry *arch_debugfs_dir;

#if defined(CONFIG_DEBUG_FS)

/* declared over in file.c */
extern const struct file_operations debugfs_file_operations;
extern const struct inode_operations debugfs_link_operations;

struct dentry *debugfs_create_file(const char *name, mode_t mode,
				   struct dentry *parent, void *data,
				   const struct file_operations *fops);

struct dentry *debugfs_create_dir(const char *name, struct dentry *parent);

struct dentry *debugfs_create_symlink(const char *name, struct dentry *parent,
				      const char *dest);

void debugfs_remove(struct dentry *dentry);
void debugfs_remove_recursive(struct dentry *dentry);

struct dentry *debugfs_rename(struct dentry *old_dir, struct dentry *old_dentry,
                struct dentry *new_dir, const char *new_name);

struct dentry *debugfs_create_u8(const char *name, mode_t mode,
				 struct dentry *parent, u8 *value);
struct dentry *debugfs_create_u16(const char *name, mode_t mode,
				  struct dentry *parent, u16 *value);
struct dentry *debugfs_create_u32(const char *name, mode_t mode,
				  struct dentry *parent, u32 *value);
struct dentry *debugfs_create_u64(const char *name, mode_t mode,
				  struct dentry *parent, u64 *value);
struct dentry *debugfs_create_x8(const char *name, mode_t mode,
				 struct dentry *parent, u8 *value);
struct dentry *debugfs_create_x16(const char *name, mode_t mode,
				  struct dentry *parent, u16 *value);
struct dentry *debugfs_create_x32(const char *name, mode_t mode,
				  struct dentry *parent, u32 *value);
struct dentry *debugfs_create_size_t(const char *name, mode_t mode,
				     struct dentry *parent, size_t *value);
struct dentry *debugfs_create_bool(const char *name, mode_t mode,
				  struct dentry *parent, u32 *value);

struct dentry *debugfs_create_blob(const char *name, mode_t mode,
				  struct dentry *parent,
				  struct debugfs_blob_wrapper *blob);
#else

#include <linux/err.h>

/* 
 * We do not return NULL from these functions if CONFIG_DEBUG_FS is not enabled
 * so users have a chance to detect if there was a real error or not.  We don't
 * want to duplicate the design decision mistakes of procfs and devfs again.
 */

static inline struct dentry *debugfs_create_file(const char *name, mode_t mode,
					struct dentry *parent, void *data,
					const struct file_operations *fops)
{
	return ERR_PTR(-ENODEV);
}

static inline struct dentry *debugfs_create_dir(const char *name,
						struct dentry *parent)
{
	return ERR_PTR(-ENODEV);
}

static inline struct dentry *debugfs_create_symlink(const char *name,
						    struct dentry *parent,
						    const char *dest)
{
	return ERR_PTR(-ENODEV);
}

static inline void debugfs_remove(struct dentry *dentry)
{ }

static inline void debugfs_remove_recursive(struct dentry *dentry)
{ }

static inline struct dentry *debugfs_rename(struct dentry *old_dir, struct dentry *old_dentry,
                struct dentry *new_dir, char *new_name)
{
	return ERR_PTR(-ENODEV);
}

static inline struct dentry *debugfs_create_u8(const char *name, mode_t mode,
					       struct dentry *parent,
					       u8 *value)
{
	return ERR_PTR(-ENODEV);
}

static inline struct dentry *debugfs_create_u16(const char *name, mode_t mode,
						struct dentry *parent,
						u16 *value)
{
	return ERR_PTR(-ENODEV);
}

static inline struct dentry *debugfs_create_u32(const char *name, mode_t mode,
						struct dentry *parent,
						u32 *value)
{
	return ERR_PTR(-ENODEV);
}

static inline struct dentry *debugfs_create_u64(const char *name, mode_t mode,
						struct dentry *parent,
						u64 *value)
{
	return ERR_PTR(-ENODEV);
}

static inline struct dentry *debugfs_create_x8(const char *name, mode_t mode,
					       struct dentry *parent,
					       u8 *value)
{
	return ERR_PTR(-ENODEV);
}

static inline struct dentry *debugfs_create_x16(const char *name, mode_t mode,
						struct dentry *parent,
						u16 *value)
{
	return ERR_PTR(-ENODEV);
}

static inline struct dentry *debugfs_create_x32(const char *name, mode_t mode,
						struct dentry *parent,
						u32 *value)
{
	return ERR_PTR(-ENODEV);
}

static inline struct dentry *debugfs_create_size_t(const char *name, mode_t mode,
				     struct dentry *parent,
				     size_t *value)
{
	return ERR_PTR(-ENODEV);
}

static inline struct dentry *debugfs_create_bool(const char *name, mode_t mode,
						 struct dentry *parent,
						 u32 *value)
{
	return ERR_PTR(-ENODEV);
}

static inline struct dentry *debugfs_create_blob(const char *name, mode_t mode,
				  struct dentry *parent,
				  struct debugfs_blob_wrapper *blob)
{
	return ERR_PTR(-ENODEV);
}

#endif

#endif
