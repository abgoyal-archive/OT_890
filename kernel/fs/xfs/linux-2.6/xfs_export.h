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
 * Copyright (c) 2005 Silicon Graphics, Inc.
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
#ifndef __XFS_EXPORT_H__
#define __XFS_EXPORT_H__

/*
 * Common defines for code related to exporting XFS filesystems over NFS.
 *
 * The NFS fileid goes out on the wire as an array of
 * 32bit unsigned ints in host order.  There are 5 possible
 * formats.
 *
 * (1)	fileid_type=0x00
 *	(no fileid data; handled by the generic code)
 *
 * (2)	fileid_type=0x01
 *	inode-num
 *	generation
 *
 * (3)	fileid_type=0x02
 *	inode-num
 *	generation
 *	parent-inode-num
 *	parent-generation
 *
 * (4)	fileid_type=0x81
 *	inode-num-lo32
 *	inode-num-hi32
 *	generation
 *
 * (5)	fileid_type=0x82
 *	inode-num-lo32
 *	inode-num-hi32
 *	generation
 *	parent-inode-num-lo32
 *	parent-inode-num-hi32
 *	parent-generation
 *
 * Note, the NFS filehandle also includes an fsid portion which
 * may have an inode number in it.  That number is hardcoded to
 * 32bits and there is no way for XFS to intercept it.  In
 * practice this means when exporting an XFS filesystem with 64bit
 * inodes you should either export the mountpoint (rather than
 * a subdirectory) or use the "fsid" export option.
 */

struct xfs_fid64 {
	u64 ino;
	u32 gen;
	u64 parent_ino;
	u32 parent_gen;
} __attribute__((packed));

/* This flag goes on the wire.  Don't play with it. */
#define XFS_FILEID_TYPE_64FLAG	0x80	/* NFS fileid has 64bit inodes */

#endif	/* __XFS_EXPORT_H__ */
