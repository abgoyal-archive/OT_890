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
#ifndef __XFS_TRANS_SPACE_H__
#define __XFS_TRANS_SPACE_H__

/*
 * Components of space reservations.
 */
#define XFS_MAX_CONTIG_EXTENTS_PER_BLOCK(mp)    \
		(((mp)->m_alloc_mxr[0]) - ((mp)->m_alloc_mnr[0]))
#define	XFS_EXTENTADD_SPACE_RES(mp,w)	(XFS_BM_MAXLEVELS(mp,w) - 1)
#define XFS_NEXTENTADD_SPACE_RES(mp,b,w)\
	(((b + XFS_MAX_CONTIG_EXTENTS_PER_BLOCK(mp) - 1) / \
	  XFS_MAX_CONTIG_EXTENTS_PER_BLOCK(mp)) * \
	  XFS_EXTENTADD_SPACE_RES(mp,w))
#define	XFS_DAENTER_1B(mp,w)	((w) == XFS_DATA_FORK ? (mp)->m_dirblkfsbs : 1)
#define	XFS_DAENTER_DBS(mp,w)	\
	(XFS_DA_NODE_MAXDEPTH + (((w) == XFS_DATA_FORK) ? 2 : 0))
#define	XFS_DAENTER_BLOCKS(mp,w)	\
	(XFS_DAENTER_1B(mp,w) * XFS_DAENTER_DBS(mp,w))
#define	XFS_DAENTER_BMAP1B(mp,w)	\
	XFS_NEXTENTADD_SPACE_RES(mp, XFS_DAENTER_1B(mp, w), w)
#define	XFS_DAENTER_BMAPS(mp,w)		\
	(XFS_DAENTER_DBS(mp,w) * XFS_DAENTER_BMAP1B(mp,w))
#define	XFS_DAENTER_SPACE_RES(mp,w)	\
	(XFS_DAENTER_BLOCKS(mp,w) + XFS_DAENTER_BMAPS(mp,w))
#define	XFS_DAREMOVE_SPACE_RES(mp,w)	XFS_DAENTER_BMAPS(mp,w)
#define	XFS_DIRENTER_MAX_SPLIT(mp,nl)	1
#define	XFS_DIRENTER_SPACE_RES(mp,nl)	\
	(XFS_DAENTER_SPACE_RES(mp, XFS_DATA_FORK) * \
	 XFS_DIRENTER_MAX_SPLIT(mp,nl))
#define	XFS_DIRREMOVE_SPACE_RES(mp)	\
	XFS_DAREMOVE_SPACE_RES(mp, XFS_DATA_FORK)
#define	XFS_IALLOC_SPACE_RES(mp)	\
	(XFS_IALLOC_BLOCKS(mp) + XFS_IN_MAXLEVELS(mp)-1)

/*
 * Space reservation values for various transactions.
 */
#define	XFS_ADDAFORK_SPACE_RES(mp)	\
	((mp)->m_dirblkfsbs + XFS_DAENTER_BMAP1B(mp, XFS_DATA_FORK))
#define	XFS_ATTRRM_SPACE_RES(mp)	\
	XFS_DAREMOVE_SPACE_RES(mp, XFS_ATTR_FORK)
/* This macro is not used - see inline code in xfs_attr_set */
#define	XFS_ATTRSET_SPACE_RES(mp, v)	\
	(XFS_DAENTER_SPACE_RES(mp, XFS_ATTR_FORK) + XFS_B_TO_FSB(mp, v))
#define	XFS_CREATE_SPACE_RES(mp,nl)	\
	(XFS_IALLOC_SPACE_RES(mp) + XFS_DIRENTER_SPACE_RES(mp,nl))
#define	XFS_DIOSTRAT_SPACE_RES(mp, v)	\
	(XFS_EXTENTADD_SPACE_RES(mp, XFS_DATA_FORK) + (v))
#define	XFS_GROWFS_SPACE_RES(mp)	\
	(2 * XFS_AG_MAXLEVELS(mp))
#define	XFS_GROWFSRT_SPACE_RES(mp,b)	\
	((b) + XFS_EXTENTADD_SPACE_RES(mp, XFS_DATA_FORK))
#define	XFS_LINK_SPACE_RES(mp,nl)	\
	XFS_DIRENTER_SPACE_RES(mp,nl)
#define	XFS_MKDIR_SPACE_RES(mp,nl)	\
	(XFS_IALLOC_SPACE_RES(mp) + XFS_DIRENTER_SPACE_RES(mp,nl))
#define	XFS_QM_DQALLOC_SPACE_RES(mp)	\
	(XFS_EXTENTADD_SPACE_RES(mp, XFS_DATA_FORK) + \
	 XFS_DQUOT_CLUSTER_SIZE_FSB)
#define	XFS_QM_QINOCREATE_SPACE_RES(mp)	\
	XFS_IALLOC_SPACE_RES(mp)
#define	XFS_REMOVE_SPACE_RES(mp)	\
	XFS_DIRREMOVE_SPACE_RES(mp)
#define	XFS_RENAME_SPACE_RES(mp,nl)	\
	(XFS_DIRREMOVE_SPACE_RES(mp) + XFS_DIRENTER_SPACE_RES(mp,nl))
#define	XFS_SYMLINK_SPACE_RES(mp,nl,b)	\
	(XFS_IALLOC_SPACE_RES(mp) + XFS_DIRENTER_SPACE_RES(mp,nl) + (b))

#endif	/* __XFS_TRANS_SPACE_H__ */
