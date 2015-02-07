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
#ifndef _VXFS_DIR_H_
#define _VXFS_DIR_H_

/*
 * Veritas filesystem driver - directory structure.
 *
 * This file contains the definition of the vxfs directory format.
 */


/*
 * VxFS directory block header.
 *
 * This entry is the head of every filesystem block in a directory.
 * It is used for free space management and additionally includes
 * a hash for speeding up directory search (lookup).
 *
 * The hash may be empty and in fact we do not use it all in the
 * Linux driver for now.
 */
struct vxfs_dirblk {
	u_int16_t	d_free;		/* free space in dirblock */
	u_int16_t	d_nhash;	/* no of hash chains */
	u_int16_t	d_hash[1];	/* hash chain */
};

/*
 * VXFS_NAMELEN is the maximum length of the d_name field
 *	of an VxFS directory entry.
 */
#define VXFS_NAMELEN	256

/*
 * VxFS directory entry.
 */
struct vxfs_direct {
	vx_ino_t	d_ino;			/* inode number */
	u_int16_t	d_reclen;		/* record length */
	u_int16_t	d_namelen;		/* d_name length */
	u_int16_t	d_hashnext;		/* next hash entry */
	char		d_name[VXFS_NAMELEN];	/* name */
};

/*
 * VXFS_DIRPAD defines the directory entry boundaries, is _must_ be
 *	a multiple of four.
 * VXFS_NAMEMIN is the length of a directory entry with a NULL d_name.
 * VXFS_DIRROUND is an internal macros that rounds a length to a value
 *	usable for directory sizes.
 * VXFS_DIRLEN calculates the directory entry size for an entry with
 *	a d_name with size len.
 */
#define VXFS_DIRPAD		4
#define VXFS_NAMEMIN		offsetof(struct vxfs_direct, d_name)
#define VXFS_DIRROUND(len)	((VXFS_DIRPAD + (len) - 1) & ~(VXFS_DIRPAD -1))
#define VXFS_DIRLEN(len)	(VXFS_DIRROUND(VXFS_NAMEMIN + (len)))

/*
 * VXFS_DIRBLKOV is the overhead of a specific dirblock.
 */
#define VXFS_DIRBLKOV(dbp)	((sizeof(short) * dbp->d_nhash) + 4)

#endif /* _VXFS_DIR_H_ */
