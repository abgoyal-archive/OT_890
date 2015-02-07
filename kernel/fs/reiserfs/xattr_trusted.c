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

#include <linux/reiserfs_fs.h>
#include <linux/capability.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/pagemap.h>
#include <linux/xattr.h>
#include <linux/reiserfs_xattr.h>
#include <asm/uaccess.h>

static int
trusted_get(struct inode *inode, const char *name, void *buffer, size_t size)
{
	if (strlen(name) < sizeof(XATTR_TRUSTED_PREFIX))
		return -EINVAL;

	if (!reiserfs_xattrs(inode->i_sb))
		return -EOPNOTSUPP;

	if (!(capable(CAP_SYS_ADMIN) || is_reiserfs_priv_object(inode)))
		return -EPERM;

	return reiserfs_xattr_get(inode, name, buffer, size);
}

static int
trusted_set(struct inode *inode, const char *name, const void *buffer,
	    size_t size, int flags)
{
	if (strlen(name) < sizeof(XATTR_TRUSTED_PREFIX))
		return -EINVAL;

	if (!reiserfs_xattrs(inode->i_sb))
		return -EOPNOTSUPP;

	if (!(capable(CAP_SYS_ADMIN) || is_reiserfs_priv_object(inode)))
		return -EPERM;

	return reiserfs_xattr_set(inode, name, buffer, size, flags);
}

static int trusted_del(struct inode *inode, const char *name)
{
	if (strlen(name) < sizeof(XATTR_TRUSTED_PREFIX))
		return -EINVAL;

	if (!reiserfs_xattrs(inode->i_sb))
		return -EOPNOTSUPP;

	if (!(capable(CAP_SYS_ADMIN) || is_reiserfs_priv_object(inode)))
		return -EPERM;

	return 0;
}

static int
trusted_list(struct inode *inode, const char *name, int namelen, char *out)
{
	int len = namelen;

	if (!reiserfs_xattrs(inode->i_sb))
		return 0;

	if (!(capable(CAP_SYS_ADMIN) || is_reiserfs_priv_object(inode)))
		return 0;

	if (out)
		memcpy(out, name, len);

	return len;
}

struct reiserfs_xattr_handler trusted_handler = {
	.prefix = XATTR_TRUSTED_PREFIX,
	.get = trusted_get,
	.set = trusted_set,
	.del = trusted_del,
	.list = trusted_list,
};
