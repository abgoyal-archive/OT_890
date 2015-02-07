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
 * Copyright (c) 2000-2003,2005 Silicon Graphics, Inc.
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
#ifndef __XFS_INUM_H__
#define	__XFS_INUM_H__

/*
 * Inode number format:
 * low inopblog bits - offset in block
 * next agblklog bits - block number in ag
 * next agno_log bits - ag number
 * high agno_log-agblklog-inopblog bits - 0
 */

typedef	__uint32_t	xfs_agino_t;	/* within allocation grp inode number */

/*
 * Useful inode bits for this kernel.
 * Used in some places where having 64-bits in the 32-bit kernels
 * costs too much.
 */
#if XFS_BIG_INUMS
typedef	xfs_ino_t	xfs_intino_t;
#else
typedef	__uint32_t	xfs_intino_t;
#endif

#define	NULLFSINO	((xfs_ino_t)-1)
#define	NULLAGINO	((xfs_agino_t)-1)

struct xfs_mount;

#define	XFS_INO_MASK(k)			(__uint32_t)((1ULL << (k)) - 1)
#define	XFS_INO_OFFSET_BITS(mp)		(mp)->m_sb.sb_inopblog
#define	XFS_INO_AGBNO_BITS(mp)		(mp)->m_sb.sb_agblklog
#define	XFS_INO_AGINO_BITS(mp)		(mp)->m_agino_log
#define	XFS_INO_AGNO_BITS(mp)		(mp)->m_agno_log
#define	XFS_INO_BITS(mp)		\
	XFS_INO_AGNO_BITS(mp) + XFS_INO_AGINO_BITS(mp)
#define	XFS_INO_TO_AGNO(mp,i)		\
	((xfs_agnumber_t)((i) >> XFS_INO_AGINO_BITS(mp)))
#define	XFS_INO_TO_AGINO(mp,i)		\
	((xfs_agino_t)(i) & XFS_INO_MASK(XFS_INO_AGINO_BITS(mp)))
#define	XFS_INO_TO_AGBNO(mp,i)		\
	(((xfs_agblock_t)(i) >> XFS_INO_OFFSET_BITS(mp)) & \
		XFS_INO_MASK(XFS_INO_AGBNO_BITS(mp)))
#define	XFS_INO_TO_OFFSET(mp,i)		\
	((int)(i) & XFS_INO_MASK(XFS_INO_OFFSET_BITS(mp)))
#define	XFS_INO_TO_FSB(mp,i)		\
	XFS_AGB_TO_FSB(mp, XFS_INO_TO_AGNO(mp,i), XFS_INO_TO_AGBNO(mp,i))
#define	XFS_AGINO_TO_INO(mp,a,i)	\
	(((xfs_ino_t)(a) << XFS_INO_AGINO_BITS(mp)) | (i))
#define	XFS_AGINO_TO_AGBNO(mp,i)	((i) >> XFS_INO_OFFSET_BITS(mp))
#define	XFS_AGINO_TO_OFFSET(mp,i)	\
	((i) & XFS_INO_MASK(XFS_INO_OFFSET_BITS(mp)))
#define	XFS_OFFBNO_TO_AGINO(mp,b,o)	\
	((xfs_agino_t)(((b) << XFS_INO_OFFSET_BITS(mp)) | (o)))

#if XFS_BIG_INUMS
#define	XFS_MAXINUMBER		((xfs_ino_t)((1ULL << 56) - 1ULL))
#define	XFS_INO64_OFFSET	((xfs_ino_t)(1ULL << 32))
#else
#define	XFS_MAXINUMBER		((xfs_ino_t)((1ULL << 32) - 1ULL))
#endif
#define	XFS_MAXINUMBER_32	((xfs_ino_t)((1ULL << 32) - 1ULL))

#endif	/* __XFS_INUM_H__ */
