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
 * namei.c
 *
 * Copyright (c) 1999 Al Smith
 *
 * Portions derived from work (c) 1995,1996 Christian Vogelgsang.
 */

#include <linux/buffer_head.h>
#include <linux/string.h>
#include <linux/smp_lock.h>
#include <linux/exportfs.h>
#include "efs.h"


static efs_ino_t efs_find_entry(struct inode *inode, const char *name, int len) {
	struct buffer_head *bh;

	int			slot, namelen;
	char			*nameptr;
	struct efs_dir		*dirblock;
	struct efs_dentry	*dirslot;
	efs_ino_t		inodenum;
	efs_block_t		block;
 
	if (inode->i_size & (EFS_DIRBSIZE-1))
		printk(KERN_WARNING "EFS: WARNING: find_entry(): directory size not a multiple of EFS_DIRBSIZE\n");

	for(block = 0; block < inode->i_blocks; block++) {

		bh = sb_bread(inode->i_sb, efs_bmap(inode, block));
		if (!bh) {
			printk(KERN_ERR "EFS: find_entry(): failed to read dir block %d\n", block);
			return 0;
		}
    
		dirblock = (struct efs_dir *) bh->b_data;

		if (be16_to_cpu(dirblock->magic) != EFS_DIRBLK_MAGIC) {
			printk(KERN_ERR "EFS: find_entry(): invalid directory block\n");
			brelse(bh);
			return(0);
		}

		for(slot = 0; slot < dirblock->slots; slot++) {
			dirslot  = (struct efs_dentry *) (((char *) bh->b_data) + EFS_SLOTAT(dirblock, slot));

			namelen  = dirslot->namelen;
			nameptr  = dirslot->name;

			if ((namelen == len) && (!memcmp(name, nameptr, len))) {
				inodenum = be32_to_cpu(dirslot->inode);
				brelse(bh);
				return(inodenum);
			}
		}
		brelse(bh);
	}
	return(0);
}

struct dentry *efs_lookup(struct inode *dir, struct dentry *dentry, struct nameidata *nd) {
	efs_ino_t inodenum;
	struct inode * inode = NULL;

	lock_kernel();
	inodenum = efs_find_entry(dir, dentry->d_name.name, dentry->d_name.len);
	if (inodenum) {
		inode = efs_iget(dir->i_sb, inodenum);
		if (IS_ERR(inode)) {
			unlock_kernel();
			return ERR_CAST(inode);
		}
	}
	unlock_kernel();

	return d_splice_alias(inode, dentry);
}

static struct inode *efs_nfs_get_inode(struct super_block *sb, u64 ino,
		u32 generation)
{
	struct inode *inode;

	if (ino == 0)
		return ERR_PTR(-ESTALE);
	inode = efs_iget(sb, ino);
	if (IS_ERR(inode))
		return ERR_CAST(inode);

	if (generation && inode->i_generation != generation) {
		iput(inode);
		return ERR_PTR(-ESTALE);
	}

	return inode;
}

struct dentry *efs_fh_to_dentry(struct super_block *sb, struct fid *fid,
		int fh_len, int fh_type)
{
	return generic_fh_to_dentry(sb, fid, fh_len, fh_type,
				    efs_nfs_get_inode);
}

struct dentry *efs_fh_to_parent(struct super_block *sb, struct fid *fid,
		int fh_len, int fh_type)
{
	return generic_fh_to_parent(sb, fid, fh_len, fh_type,
				    efs_nfs_get_inode);
}

struct dentry *efs_get_parent(struct dentry *child)
{
	struct dentry *parent = ERR_PTR(-ENOENT);
	efs_ino_t ino;

	lock_kernel();
	ino = efs_find_entry(child->d_inode, "..", 2);
	if (ino)
		parent = d_obtain_alias(efs_iget(child->d_inode->i_sb, ino));
	unlock_kernel();

	return parent;
}
