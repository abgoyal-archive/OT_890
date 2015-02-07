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
 * linux/fs/ext4/xattr_trusted.c
 * Handler for trusted extended attributes.
 *
 * Copyright (C) 2003 by Andreas Gruenbacher, <a.gruenbacher@computer.org>
 */

#include <linux/module.h>
#include <linux/string.h>
#include <linux/capability.h>
#include <linux/fs.h>
#include "ext4_jbd2.h"
#include "ext4.h"
#include "xattr.h"

static size_t
ext4_xattr_trusted_list(struct inode *inode, char *list, size_t list_size,
			const char *name, size_t name_len)
{
	const size_t prefix_len = XATTR_TRUSTED_PREFIX_LEN;
	const size_t total_len = prefix_len + name_len + 1;

	if (!capable(CAP_SYS_ADMIN))
		return 0;

	if (list && total_len <= list_size) {
		memcpy(list, XATTR_TRUSTED_PREFIX, prefix_len);
		memcpy(list+prefix_len, name, name_len);
		list[prefix_len + name_len] = '\0';
	}
	return total_len;
}

static int
ext4_xattr_trusted_get(struct inode *inode, const char *name,
		       void *buffer, size_t size)
{
	if (strcmp(name, "") == 0)
		return -EINVAL;
	return ext4_xattr_get(inode, EXT4_XATTR_INDEX_TRUSTED, name,
			      buffer, size);
}

static int
ext4_xattr_trusted_set(struct inode *inode, const char *name,
		       const void *value, size_t size, int flags)
{
	if (strcmp(name, "") == 0)
		return -EINVAL;
	return ext4_xattr_set(inode, EXT4_XATTR_INDEX_TRUSTED, name,
			      value, size, flags);
}

struct xattr_handler ext4_xattr_trusted_handler = {
	.prefix	= XATTR_TRUSTED_PREFIX,
	.list	= ext4_xattr_trusted_list,
	.get	= ext4_xattr_trusted_get,
	.set	= ext4_xattr_trusted_set,
};
