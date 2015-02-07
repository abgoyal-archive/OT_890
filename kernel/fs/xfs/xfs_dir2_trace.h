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
#ifndef __XFS_DIR2_TRACE_H__
#define __XFS_DIR2_TRACE_H__

/*
 * Tracing for xfs v2 directories.
 */

#if defined(XFS_DIR2_TRACE)

struct ktrace;
struct xfs_dabuf;
struct xfs_da_args;

#define	XFS_DIR2_GTRACE_SIZE		4096	/* global buffer */
#define	XFS_DIR2_KTRACE_SIZE		32	/* per-inode buffer */
extern struct ktrace *xfs_dir2_trace_buf;

#define	XFS_DIR2_KTRACE_ARGS		1	/* args only */
#define	XFS_DIR2_KTRACE_ARGS_B		2	/* args + buffer */
#define	XFS_DIR2_KTRACE_ARGS_BB		3	/* args + 2 buffers */
#define	XFS_DIR2_KTRACE_ARGS_DB		4	/* args, db, buffer */
#define	XFS_DIR2_KTRACE_ARGS_I		5	/* args, inum */
#define	XFS_DIR2_KTRACE_ARGS_S		6	/* args, int */
#define	XFS_DIR2_KTRACE_ARGS_SB		7	/* args, int, buffer */
#define	XFS_DIR2_KTRACE_ARGS_BIBII	8	/* args, buf/int/buf/int/int */

void xfs_dir2_trace_args(char *where, struct xfs_da_args *args);
void xfs_dir2_trace_args_b(char *where, struct xfs_da_args *args,
			   struct xfs_dabuf *bp);
void xfs_dir2_trace_args_bb(char *where, struct xfs_da_args *args,
			    struct xfs_dabuf *lbp, struct xfs_dabuf *dbp);
void xfs_dir2_trace_args_bibii(char *where, struct xfs_da_args *args,
			       struct xfs_dabuf *bs, int ss,
			       struct xfs_dabuf *bd, int sd, int c);
void xfs_dir2_trace_args_db(char *where, struct xfs_da_args *args,
			    xfs_dir2_db_t db, struct xfs_dabuf *bp);
void xfs_dir2_trace_args_i(char *where, struct xfs_da_args *args, xfs_ino_t i);
void xfs_dir2_trace_args_s(char *where, struct xfs_da_args *args, int s);
void xfs_dir2_trace_args_sb(char *where, struct xfs_da_args *args, int s,
			    struct xfs_dabuf *bp);

#else	/* XFS_DIR2_TRACE */

#define	xfs_dir2_trace_args(where, args)
#define	xfs_dir2_trace_args_b(where, args, bp)
#define	xfs_dir2_trace_args_bb(where, args, lbp, dbp)
#define	xfs_dir2_trace_args_bibii(where, args, bs, ss, bd, sd, c)
#define	xfs_dir2_trace_args_db(where, args, db, bp)
#define	xfs_dir2_trace_args_i(where, args, i)
#define	xfs_dir2_trace_args_s(where, args, s)
#define	xfs_dir2_trace_args_sb(where, args, s, bp)

#endif	/* XFS_DIR2_TRACE */

#endif	/* __XFS_DIR2_TRACE_H__ */
