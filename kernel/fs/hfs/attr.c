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
 *  linux/fs/hfs/attr.c
 *
 * (C) 2003 Ardis Technologies <roman@ardistech.com>
 *
 * Export hfs data via xattr
 */


#include <linux/fs.h>
#include <linux/xattr.h>

#include "hfs_fs.h"
#include "btree.h"

int hfs_setxattr(struct dentry *dentry, const char *name,
		 const void *value, size_t size, int flags)
{
	struct inode *inode = dentry->d_inode;
	struct hfs_find_data fd;
	hfs_cat_rec rec;
	struct hfs_cat_file *file;
	int res;

	if (!S_ISREG(inode->i_mode) || HFS_IS_RSRC(inode))
		return -EOPNOTSUPP;

	res = hfs_find_init(HFS_SB(inode->i_sb)->cat_tree, &fd);
	if (res)
		return res;
	fd.search_key->cat = HFS_I(inode)->cat_key;
	res = hfs_brec_find(&fd);
	if (res)
		goto out;
	hfs_bnode_read(fd.bnode, &rec, fd.entryoffset,
			sizeof(struct hfs_cat_file));
	file = &rec.file;

	if (!strcmp(name, "hfs.type")) {
		if (size == 4)
			memcpy(&file->UsrWds.fdType, value, 4);
		else
			res = -ERANGE;
	} else if (!strcmp(name, "hfs.creator")) {
		if (size == 4)
			memcpy(&file->UsrWds.fdCreator, value, 4);
		else
			res = -ERANGE;
	} else
		res = -EOPNOTSUPP;
	if (!res)
		hfs_bnode_write(fd.bnode, &rec, fd.entryoffset,
				sizeof(struct hfs_cat_file));
out:
	hfs_find_exit(&fd);
	return res;
}

ssize_t hfs_getxattr(struct dentry *dentry, const char *name,
			 void *value, size_t size)
{
	struct inode *inode = dentry->d_inode;
	struct hfs_find_data fd;
	hfs_cat_rec rec;
	struct hfs_cat_file *file;
	ssize_t res = 0;

	if (!S_ISREG(inode->i_mode) || HFS_IS_RSRC(inode))
		return -EOPNOTSUPP;

	if (size) {
		res = hfs_find_init(HFS_SB(inode->i_sb)->cat_tree, &fd);
		if (res)
			return res;
		fd.search_key->cat = HFS_I(inode)->cat_key;
		res = hfs_brec_find(&fd);
		if (res)
			goto out;
		hfs_bnode_read(fd.bnode, &rec, fd.entryoffset,
				sizeof(struct hfs_cat_file));
	}
	file = &rec.file;

	if (!strcmp(name, "hfs.type")) {
		if (size >= 4) {
			memcpy(value, &file->UsrWds.fdType, 4);
			res = 4;
		} else
			res = size ? -ERANGE : 4;
	} else if (!strcmp(name, "hfs.creator")) {
		if (size >= 4) {
			memcpy(value, &file->UsrWds.fdCreator, 4);
			res = 4;
		} else
			res = size ? -ERANGE : 4;
	} else
		res = -ENODATA;
out:
	if (size)
		hfs_find_exit(&fd);
	return res;
}

#define HFS_ATTRLIST_SIZE (sizeof("hfs.creator")+sizeof("hfs.type"))

ssize_t hfs_listxattr(struct dentry *dentry, char *buffer, size_t size)
{
	struct inode *inode = dentry->d_inode;

	if (!S_ISREG(inode->i_mode) || HFS_IS_RSRC(inode))
		return -EOPNOTSUPP;

	if (!buffer || !size)
		return HFS_ATTRLIST_SIZE;
	if (size < HFS_ATTRLIST_SIZE)
		return -ERANGE;
	strcpy(buffer, "hfs.type");
	strcpy(buffer + sizeof("hfs.type"), "hfs.creator");

	return HFS_ATTRLIST_SIZE;
}
