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
  File: linux/xattr.h

  Extended attributes handling.

  Copyright (C) 2001 by Andreas Gruenbacher <a.gruenbacher@computer.org>
  Copyright (c) 2001-2002 Silicon Graphics, Inc.  All Rights Reserved.
  Copyright (c) 2004 Red Hat, Inc., James Morris <jmorris@redhat.com>
*/
#ifndef _LINUX_XATTR_H
#define _LINUX_XATTR_H

#define XATTR_CREATE	0x1	/* set value, fail if attr already exists */
#define XATTR_REPLACE	0x2	/* set value, fail if attr does not exist */

#ifdef  __KERNEL__

#include <linux/types.h>

/* Namespaces */
#define XATTR_OS2_PREFIX "os2."
#define XATTR_OS2_PREFIX_LEN (sizeof (XATTR_OS2_PREFIX) - 1)

#define XATTR_SECURITY_PREFIX	"security."
#define XATTR_SECURITY_PREFIX_LEN (sizeof (XATTR_SECURITY_PREFIX) - 1)

#define XATTR_SYSTEM_PREFIX "system."
#define XATTR_SYSTEM_PREFIX_LEN (sizeof (XATTR_SYSTEM_PREFIX) - 1)

#define XATTR_TRUSTED_PREFIX "trusted."
#define XATTR_TRUSTED_PREFIX_LEN (sizeof (XATTR_TRUSTED_PREFIX) - 1)

#define XATTR_USER_PREFIX "user."
#define XATTR_USER_PREFIX_LEN (sizeof (XATTR_USER_PREFIX) - 1)

struct inode;
struct dentry;

struct xattr_handler {
	char *prefix;
	size_t (*list)(struct inode *inode, char *list, size_t list_size,
		       const char *name, size_t name_len);
	int (*get)(struct inode *inode, const char *name, void *buffer,
		   size_t size);
	int (*set)(struct inode *inode, const char *name, const void *buffer,
		   size_t size, int flags);
};

ssize_t xattr_getsecurity(struct inode *, const char *, void *, size_t);
ssize_t vfs_getxattr(struct dentry *, const char *, void *, size_t);
ssize_t vfs_listxattr(struct dentry *d, char *list, size_t size);
int vfs_setxattr(struct dentry *, const char *, const void *, size_t, int);
int vfs_removexattr(struct dentry *, const char *);

ssize_t generic_getxattr(struct dentry *dentry, const char *name, void *buffer, size_t size);
ssize_t generic_listxattr(struct dentry *dentry, char *buffer, size_t buffer_size);
int generic_setxattr(struct dentry *dentry, const char *name, const void *value, size_t size, int flags);
int generic_removexattr(struct dentry *dentry, const char *name);

#endif  /*  __KERNEL__  */

#endif	/* _LINUX_XATTR_H */
