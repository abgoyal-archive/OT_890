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
 * Copyright (C) Sistina Software, Inc.  1997-2003 All rights reserved.
 * Copyright (C) 2004-2006 Red Hat, Inc.  All rights reserved.
 *
 * This copyrighted material is made available to anyone wishing to use,
 * modify, copy, or redistribute it subject to the terms and conditions
 * of the GNU General Public License version 2.
 */

#ifndef __INODE_DOT_H__
#define __INODE_DOT_H__

#include <linux/fs.h>
#include "util.h"

static inline int gfs2_is_stuffed(const struct gfs2_inode *ip)
{
	return !ip->i_height;
}

static inline int gfs2_is_jdata(const struct gfs2_inode *ip)
{
	return ip->i_diskflags & GFS2_DIF_JDATA;
}

static inline int gfs2_is_writeback(const struct gfs2_inode *ip)
{
	const struct gfs2_sbd *sdp = GFS2_SB(&ip->i_inode);
	return (sdp->sd_args.ar_data == GFS2_DATA_WRITEBACK) && !gfs2_is_jdata(ip);
}

static inline int gfs2_is_ordered(const struct gfs2_inode *ip)
{
	const struct gfs2_sbd *sdp = GFS2_SB(&ip->i_inode);
	return (sdp->sd_args.ar_data == GFS2_DATA_ORDERED) && !gfs2_is_jdata(ip);
}

static inline int gfs2_is_dir(const struct gfs2_inode *ip)
{
	return S_ISDIR(ip->i_inode.i_mode);
}

static inline void gfs2_set_inode_blocks(struct inode *inode, u64 blocks)
{
	inode->i_blocks = blocks <<
		(GFS2_SB(inode)->sd_sb.sb_bsize_shift - GFS2_BASIC_BLOCK_SHIFT);
}

static inline u64 gfs2_get_inode_blocks(const struct inode *inode)
{
	return inode->i_blocks >>
		(GFS2_SB(inode)->sd_sb.sb_bsize_shift - GFS2_BASIC_BLOCK_SHIFT);
}

static inline void gfs2_add_inode_blocks(struct inode *inode, s64 change)
{
	gfs2_assert(GFS2_SB(inode), (change >= 0 || inode->i_blocks > -change));
	change *= (GFS2_SB(inode)->sd_sb.sb_bsize/GFS2_BASIC_BLOCK);
	inode->i_blocks += change;
}

static inline int gfs2_check_inum(const struct gfs2_inode *ip, u64 no_addr,
				  u64 no_formal_ino)
{
	return ip->i_no_addr == no_addr && ip->i_no_formal_ino == no_formal_ino;
}

static inline void gfs2_inum_out(const struct gfs2_inode *ip,
				 struct gfs2_dirent *dent)
{
	dent->de_inum.no_formal_ino = cpu_to_be64(ip->i_no_formal_ino);
	dent->de_inum.no_addr = cpu_to_be64(ip->i_no_addr);
}


void gfs2_set_iop(struct inode *inode);
struct inode *gfs2_inode_lookup(struct super_block *sb, unsigned type, 
				u64 no_addr, u64 no_formal_ino,
				int skip_freeing);
struct inode *gfs2_ilookup(struct super_block *sb, u64 no_addr);

int gfs2_inode_refresh(struct gfs2_inode *ip);

int gfs2_dinode_dealloc(struct gfs2_inode *inode);
int gfs2_change_nlink(struct gfs2_inode *ip, int diff);
struct inode *gfs2_lookupi(struct inode *dir, const struct qstr *name,
			   int is_root);
struct inode *gfs2_createi(struct gfs2_holder *ghs, const struct qstr *name,
			   unsigned int mode, dev_t dev);
int gfs2_rmdiri(struct gfs2_inode *dip, const struct qstr *name,
		struct gfs2_inode *ip);
int gfs2_unlink_ok(struct gfs2_inode *dip, const struct qstr *name,
		   const struct gfs2_inode *ip);
int gfs2_permission(struct inode *inode, int mask);
int gfs2_readlinki(struct gfs2_inode *ip, char **buf, unsigned int *len);
int gfs2_setattr_simple(struct gfs2_inode *ip, struct iattr *attr);
struct inode *gfs2_lookup_simple(struct inode *dip, const char *name);
void gfs2_dinode_out(const struct gfs2_inode *ip, void *buf);
void gfs2_dinode_print(const struct gfs2_inode *ip);

extern const struct inode_operations gfs2_file_iops;
extern const struct inode_operations gfs2_dir_iops;
extern const struct inode_operations gfs2_symlink_iops;
extern const struct file_operations gfs2_file_fops;
extern const struct file_operations gfs2_dir_fops;
extern const struct file_operations gfs2_file_fops_nolock;
extern const struct file_operations gfs2_dir_fops_nolock;

extern void gfs2_set_inode_flags(struct inode *inode);

#endif /* __INODE_DOT_H__ */

