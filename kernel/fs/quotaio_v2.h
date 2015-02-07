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
 *	Definitions of structures for vfsv0 quota format
 */

#ifndef _LINUX_QUOTAIO_V2_H
#define _LINUX_QUOTAIO_V2_H

#include <linux/types.h>
#include <linux/quota.h>

/*
 * Definitions of magics and versions of current quota files
 */
#define V2_INITQMAGICS {\
	0xd9c01f11,	/* USRQUOTA */\
	0xd9c01927	/* GRPQUOTA */\
}

#define V2_INITQVERSIONS {\
	0,		/* USRQUOTA */\
	0		/* GRPQUOTA */\
}

/* First generic header */
struct v2_disk_dqheader {
	__le32 dqh_magic;	/* Magic number identifying file */
	__le32 dqh_version;	/* File version */
};

/*
 * The following structure defines the format of the disk quota file
 * (as it appears on disk) - the file is a radix tree whose leaves point
 * to blocks of these structures.
 */
struct v2_disk_dqblk {
	__le32 dqb_id;		/* id this quota applies to */
	__le32 dqb_ihardlimit;	/* absolute limit on allocated inodes */
	__le32 dqb_isoftlimit;	/* preferred inode limit */
	__le32 dqb_curinodes;	/* current # allocated inodes */
	__le32 dqb_bhardlimit;	/* absolute limit on disk space (in QUOTABLOCK_SIZE) */
	__le32 dqb_bsoftlimit;	/* preferred limit on disk space (in QUOTABLOCK_SIZE) */
	__le64 dqb_curspace;	/* current space occupied (in bytes) */
	__le64 dqb_btime;	/* time limit for excessive disk use */
	__le64 dqb_itime;	/* time limit for excessive inode use */
};

/* Header with type and version specific information */
struct v2_disk_dqinfo {
	__le32 dqi_bgrace;	/* Time before block soft limit becomes hard limit */
	__le32 dqi_igrace;	/* Time before inode soft limit becomes hard limit */
	__le32 dqi_flags;	/* Flags for quotafile (DQF_*) */
	__le32 dqi_blocks;	/* Number of blocks in file */
	__le32 dqi_free_blk;	/* Number of first free block in the list */
	__le32 dqi_free_entry;	/* Number of block with at least one free entry */
};

#define V2_DQINFOOFF	sizeof(struct v2_disk_dqheader)	/* Offset of info header in file */
#define V2_DQBLKSIZE_BITS 10				/* Size of leaf block in tree */

#endif /* _LINUX_QUOTAIO_V2_H */
