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
 * Copyright (c) 2000-2001 Christoph Hellwig.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions, and the following disclaimer,
 *    without modification.
 * 2. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * Alternatively, this software may be distributed under the terms of the
 * GNU General Public License ("GPL").
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */
#ifndef _VXFS_OLT_H_
#define _VXFS_OLT_H_

/*
 * Veritas filesystem driver - Object Location Table data structures.
 *
 * This file contains definitions for the Object Location Table used
 * by the Veritas Filesystem version 2 and newer.
 */


/*
 * OLT magic number (vxfs_olt->olt_magic).
 */
#define VXFS_OLT_MAGIC		0xa504FCF5

/*
 * VxFS OLT entry types.
 */
enum {
	VXFS_OLT_FREE	= 1,
	VXFS_OLT_FSHEAD	= 2,
	VXFS_OLT_CUT	= 3,
	VXFS_OLT_ILIST	= 4,
	VXFS_OLT_DEV	= 5,
	VXFS_OLT_SB	= 6
};

/*
 * VxFS OLT header.
 *
 * The Object Location Table header is placed at the beginning of each
 * OLT extent.  It is used to fing certain filesystem-wide metadata, e.g.
 * the inital inode list, the fileset header or the device configuration.
 */
struct vxfs_olt {
	u_int32_t	olt_magic;	/* magic number			*/
	u_int32_t	olt_size;	/* size of this entry		*/
	u_int32_t	olt_checksum;	/* checksum of extent		*/
	u_int32_t	__unused1;	/* ???				*/
	u_int32_t	olt_mtime;	/* time of last mod. (sec)	*/
	u_int32_t	olt_mutime;	/* time of last mod. (usec)	*/
	u_int32_t	olt_totfree;	/* free space in OLT extent	*/
	vx_daddr_t	olt_extents[2];	/* addr of this extent, replica	*/
	u_int32_t	olt_esize;	/* size of this extent		*/
	vx_daddr_t	olt_next[2];    /* addr of next extent, replica	*/
	u_int32_t	olt_nsize;	/* size of next extent		*/
	u_int32_t	__unused2;	/* align to 8 byte boundary	*/
};

/*
 * VxFS common OLT entry (on disk).
 */
struct vxfs_oltcommon {
	u_int32_t	olt_type;	/* type of this record		*/
	u_int32_t	olt_size;	/* size of this record		*/
};

/*
 * VxFS free OLT entry (on disk).
 */
struct vxfs_oltfree {
	u_int32_t	olt_type;	/* type of this record		*/
	u_int32_t	olt_fsize;	/* size of this free record	*/
};

/*
 * VxFS initial-inode list (on disk).
 */
struct vxfs_oltilist {
	u_int32_t	olt_type;	/* type of this record		*/
	u_int32_t	olt_size;	/* size of this record		*/
	vx_ino_t	olt_iext[2];	/* initial inode list, replica	*/
};

/*
 * Current Usage Table 
 */
struct vxfs_oltcut {
	u_int32_t	olt_type;	/* type of this record		*/
	u_int32_t	olt_size;	/* size of this record		*/
	vx_ino_t	olt_cutino;	/* inode of current usage table	*/
	u_int32_t	__pad;		/* unused, 8 byte align		*/
};

/*
 * Inodes containing Superblock, Intent log and OLTs 
 */
struct vxfs_oltsb {
	u_int32_t	olt_type;	/* type of this record		*/
	u_int32_t	olt_size;	/* size of this record		*/
	vx_ino_t	olt_sbino;	/* inode of superblock file	*/
	u_int32_t	__unused1;	/* ???				*/
	vx_ino_t	olt_logino[2];	/* inode of log file,replica	*/
	vx_ino_t	olt_oltino[2];	/* inode of OLT, replica	*/
};

/*
 * Inode containing device configuration + it's replica 
 */
struct vxfs_oltdev {
	u_int32_t	olt_type;	/* type of this record		*/
	u_int32_t	olt_size;	/* size of this record		*/
	vx_ino_t	olt_devino[2];	/* inode of device config files	*/
};

/*
 * Fileset header 
 */
struct vxfs_oltfshead {
	u_int32_t	olt_type;	/* type number			*/
	u_int32_t	olt_size;	/* size of this record		*/
	vx_ino_t	olt_fsino[2];   /* inodes of fileset header	*/
};

#endif /* _VXFS_OLT_H_ */
