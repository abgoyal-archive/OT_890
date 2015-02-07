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
  File: linux/posix_acl_xattr.h

  Extended attribute system call representation of Access Control Lists.

  Copyright (C) 2000 by Andreas Gruenbacher <a.gruenbacher@computer.org>
  Copyright (C) 2002 SGI - Silicon Graphics, Inc <linux-xfs@oss.sgi.com>
 */
#ifndef _POSIX_ACL_XATTR_H
#define _POSIX_ACL_XATTR_H

#include <linux/posix_acl.h>

/* Extended attribute names */
#define POSIX_ACL_XATTR_ACCESS	"system.posix_acl_access"
#define POSIX_ACL_XATTR_DEFAULT	"system.posix_acl_default"

/* Supported ACL a_version fields */
#define POSIX_ACL_XATTR_VERSION	0x0002


/* An undefined entry e_id value */
#define ACL_UNDEFINED_ID	(-1)

typedef struct {
	__le16			e_tag;
	__le16			e_perm;
	__le32			e_id;
} posix_acl_xattr_entry;

typedef struct {
	__le32			a_version;
	posix_acl_xattr_entry	a_entries[0];
} posix_acl_xattr_header;


static inline size_t
posix_acl_xattr_size(int count)
{
	return (sizeof(posix_acl_xattr_header) +
		(count * sizeof(posix_acl_xattr_entry)));
}

static inline int
posix_acl_xattr_count(size_t size)
{
	if (size < sizeof(posix_acl_xattr_header))
		return -1;
	size -= sizeof(posix_acl_xattr_header);
	if (size % sizeof(posix_acl_xattr_entry))
		return -1;
	return size / sizeof(posix_acl_xattr_entry);
}

struct posix_acl *posix_acl_from_xattr(const void *value, size_t size);
int posix_acl_to_xattr(const struct posix_acl *acl, void *buffer, size_t size);

#endif	/* _POSIX_ACL_XATTR_H */
