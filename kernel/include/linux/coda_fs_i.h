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
 *  coda_fs_i.h
 *
 *  Copyright (C) 1998 Carnegie Mellon University
 *
 */

#ifndef _LINUX_CODA_FS_I
#define _LINUX_CODA_FS_I

#include <linux/types.h>
#include <linux/list.h>
#include <linux/coda.h>

/*
 * coda fs inode data
 */
struct coda_inode_info {
        struct CodaFid	   c_fid;	/* Coda identifier */
        u_short	           c_flags;     /* flags (see below) */
	struct list_head   c_cilist;    /* list of all coda inodes */
	unsigned int	   c_mapcount;  /* nr of times this inode is mapped */
	unsigned int	   c_cached_epoch; /* epoch for cached permissions */
	vuid_t		   c_uid;	/* fsuid for cached permissions */
        unsigned int       c_cached_perm; /* cached access permissions */
	struct inode	   vfs_inode;
};

/*
 * coda fs file private data
 */
#define CODA_MAGIC 0xC0DAC0DA
struct coda_file_info {
	int		   cfi_magic;	  /* magic number */
	struct file	  *cfi_container; /* container file for this cnode */
	unsigned int	   cfi_mapcount;  /* nr of times this file is mapped */
};

#define CODA_FTOC(file) ((struct coda_file_info *)((file)->private_data))

/* flags */
#define C_VATTR       0x1   /* Validity of vattr in inode */
#define C_FLUSH       0x2   /* used after a flush */
#define C_DYING       0x4   /* from venus (which died) */
#define C_PURGE       0x8

int coda_cnode_make(struct inode **, struct CodaFid *, struct super_block *);
struct inode *coda_iget(struct super_block *sb, struct CodaFid *fid, struct coda_vattr *attr);
int coda_cnode_makectl(struct inode **inode, struct super_block *sb);
struct inode *coda_fid_to_inode(struct CodaFid *fid, struct super_block *sb);
void coda_replace_fid(struct inode *, struct CodaFid *, struct CodaFid *);

#endif
