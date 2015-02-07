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
 * Copyright (c) 2000,2005 Silicon Graphics, Inc.
 * All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it would be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write the Free Software Foundation,
 * Inc.,  51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
#ifndef __XFS_ALLOC_BTREE_H__
#define	__XFS_ALLOC_BTREE_H__

/*
 * Freespace on-disk structures
 */

struct xfs_buf;
struct xfs_btree_cur;
struct xfs_mount;

/*
 * There are two on-disk btrees, one sorted by blockno and one sorted
 * by blockcount and blockno.  All blocks look the same to make the code
 * simpler; if we have time later, we'll make the optimizations.
 */
#define	XFS_ABTB_MAGIC	0x41425442	/* 'ABTB' for bno tree */
#define	XFS_ABTC_MAGIC	0x41425443	/* 'ABTC' for cnt tree */

/*
 * Data record/key structure
 */
typedef struct xfs_alloc_rec {
	__be32		ar_startblock;	/* starting block number */
	__be32		ar_blockcount;	/* count of free blocks */
} xfs_alloc_rec_t, xfs_alloc_key_t;

typedef struct xfs_alloc_rec_incore {
	xfs_agblock_t	ar_startblock;	/* starting block number */
	xfs_extlen_t	ar_blockcount;	/* count of free blocks */
} xfs_alloc_rec_incore_t;

/* btree pointer type */
typedef __be32 xfs_alloc_ptr_t;

/*
 * Minimum and maximum blocksize and sectorsize.
 * The blocksize upper limit is pretty much arbitrary.
 * The sectorsize upper limit is due to sizeof(sb_sectsize).
 */
#define XFS_MIN_BLOCKSIZE_LOG	9	/* i.e. 512 bytes */
#define XFS_MAX_BLOCKSIZE_LOG	16	/* i.e. 65536 bytes */
#define XFS_MIN_BLOCKSIZE	(1 << XFS_MIN_BLOCKSIZE_LOG)
#define XFS_MAX_BLOCKSIZE	(1 << XFS_MAX_BLOCKSIZE_LOG)
#define XFS_MIN_SECTORSIZE_LOG	9	/* i.e. 512 bytes */
#define XFS_MAX_SECTORSIZE_LOG	15	/* i.e. 32768 bytes */
#define XFS_MIN_SECTORSIZE	(1 << XFS_MIN_SECTORSIZE_LOG)
#define XFS_MAX_SECTORSIZE	(1 << XFS_MAX_SECTORSIZE_LOG)

/*
 * Block numbers in the AG:
 * SB is sector 0, AGF is sector 1, AGI is sector 2, AGFL is sector 3.
 */
#define	XFS_BNO_BLOCK(mp)	((xfs_agblock_t)(XFS_AGFL_BLOCK(mp) + 1))
#define	XFS_CNT_BLOCK(mp)	((xfs_agblock_t)(XFS_BNO_BLOCK(mp) + 1))

/*
 * Btree block header size depends on a superblock flag.
 *
 * (not quite yet, but soon)
 */
#define XFS_ALLOC_BLOCK_LEN(mp)	XFS_BTREE_SBLOCK_LEN

/*
 * Record, key, and pointer address macros for btree blocks.
 *
 * (note that some of these may appear unused, but they are used in userspace)
 */
#define XFS_ALLOC_REC_ADDR(mp, block, index) \
	((xfs_alloc_rec_t *) \
		((char *)(block) + \
		 XFS_ALLOC_BLOCK_LEN(mp) + \
		 (((index) - 1) * sizeof(xfs_alloc_rec_t))))

#define XFS_ALLOC_KEY_ADDR(mp, block, index) \
	((xfs_alloc_key_t *) \
		((char *)(block) + \
		 XFS_ALLOC_BLOCK_LEN(mp) + \
		 ((index) - 1) * sizeof(xfs_alloc_key_t)))

#define XFS_ALLOC_PTR_ADDR(mp, block, index, maxrecs) \
	((xfs_alloc_ptr_t *) \
		((char *)(block) + \
		 XFS_ALLOC_BLOCK_LEN(mp) + \
		 (maxrecs) * sizeof(xfs_alloc_key_t) + \
		 ((index) - 1) * sizeof(xfs_alloc_ptr_t)))

extern struct xfs_btree_cur *xfs_allocbt_init_cursor(struct xfs_mount *,
		struct xfs_trans *, struct xfs_buf *,
		xfs_agnumber_t, xfs_btnum_t);
extern int xfs_allocbt_maxrecs(struct xfs_mount *, int, int);

#endif	/* __XFS_ALLOC_BTREE_H__ */
