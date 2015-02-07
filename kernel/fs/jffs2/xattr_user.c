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
 * JFFS2 -- Journalling Flash File System, Version 2.
 *
 * Copyright © 2006  NEC Corporation
 *
 * Created by KaiGai Kohei <kaigai@ak.jp.nec.com>
 *
 * For licensing information, see the file 'LICENCE' in this directory.
 *
 */

#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/jffs2.h>
#include <linux/xattr.h>
#include <linux/mtd/mtd.h>
#include "nodelist.h"

static int jffs2_user_getxattr(struct inode *inode, const char *name,
			       void *buffer, size_t size)
{
	if (!strcmp(name, ""))
		return -EINVAL;
	return do_jffs2_getxattr(inode, JFFS2_XPREFIX_USER, name, buffer, size);
}

static int jffs2_user_setxattr(struct inode *inode, const char *name, const void *buffer,
			       size_t size, int flags)
{
	if (!strcmp(name, ""))
		return -EINVAL;
	return do_jffs2_setxattr(inode, JFFS2_XPREFIX_USER, name, buffer, size, flags);
}

static size_t jffs2_user_listxattr(struct inode *inode, char *list, size_t list_size,
				   const char *name, size_t name_len)
{
	size_t retlen = XATTR_USER_PREFIX_LEN + name_len + 1;

	if (list && retlen <= list_size) {
		strcpy(list, XATTR_USER_PREFIX);
		strcpy(list + XATTR_USER_PREFIX_LEN, name);
	}

	return retlen;
}

struct xattr_handler jffs2_user_xattr_handler = {
	.prefix = XATTR_USER_PREFIX,
	.list = jffs2_user_listxattr,
	.set = jffs2_user_setxattr,
	.get = jffs2_user_getxattr
};
