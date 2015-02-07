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
 *   Copyright (C) International Business Machines Corp., 2000-2001
 *
 *   This program is free software;  you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY;  without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
 *   the GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program;  if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */
#ifndef	_H_JFS_INODE
#define _H_JFS_INODE

struct fid;

extern struct inode *ialloc(struct inode *, umode_t);
extern int jfs_fsync(struct file *, struct dentry *, int);
extern long jfs_ioctl(struct file *, unsigned int, unsigned long);
extern long jfs_compat_ioctl(struct file *, unsigned int, unsigned long);
extern struct inode *jfs_iget(struct super_block *, unsigned long);
extern int jfs_commit_inode(struct inode *, int);
extern int jfs_write_inode(struct inode*, int);
extern void jfs_delete_inode(struct inode *);
extern void jfs_dirty_inode(struct inode *);
extern void jfs_truncate(struct inode *);
extern void jfs_truncate_nolock(struct inode *, loff_t);
extern void jfs_free_zero_link(struct inode *);
extern struct dentry *jfs_get_parent(struct dentry *dentry);
extern void jfs_get_inode_flags(struct jfs_inode_info *);
extern struct dentry *jfs_fh_to_dentry(struct super_block *sb, struct fid *fid,
	int fh_len, int fh_type);
extern struct dentry *jfs_fh_to_parent(struct super_block *sb, struct fid *fid,
	int fh_len, int fh_type);
extern void jfs_set_inode_flags(struct inode *);
extern int jfs_get_block(struct inode *, sector_t, struct buffer_head *, int);

extern const struct address_space_operations jfs_aops;
extern const struct inode_operations jfs_dir_inode_operations;
extern const struct file_operations jfs_dir_operations;
extern const struct inode_operations jfs_file_inode_operations;
extern const struct file_operations jfs_file_operations;
extern const struct inode_operations jfs_symlink_inode_operations;
extern struct dentry_operations jfs_ci_dentry_operations;
#endif				/* _H_JFS_INODE */
