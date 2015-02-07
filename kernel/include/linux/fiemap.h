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
 * FS_IOC_FIEMAP ioctl infrastructure.
 *
 * Some portions copyright (C) 2007 Cluster File Systems, Inc
 *
 * Authors: Mark Fasheh <mfasheh@suse.com>
 *          Kalpak Shah <kalpak.shah@sun.com>
 *          Andreas Dilger <adilger@sun.com>
 */

#ifndef _LINUX_FIEMAP_H
#define _LINUX_FIEMAP_H

struct fiemap_extent {
	__u64 fe_logical;  /* logical offset in bytes for the start of
			    * the extent from the beginning of the file */
	__u64 fe_physical; /* physical offset in bytes for the start
			    * of the extent from the beginning of the disk */
	__u64 fe_length;   /* length in bytes for this extent */
	__u64 fe_reserved64[2];
	__u32 fe_flags;    /* FIEMAP_EXTENT_* flags for this extent */
	__u32 fe_reserved[3];
};

struct fiemap {
	__u64 fm_start;		/* logical offset (inclusive) at
				 * which to start mapping (in) */
	__u64 fm_length;	/* logical length of mapping which
				 * userspace wants (in) */
	__u32 fm_flags;		/* FIEMAP_FLAG_* flags for request (in/out) */
	__u32 fm_mapped_extents;/* number of extents that were mapped (out) */
	__u32 fm_extent_count;  /* size of fm_extents array (in) */
	__u32 fm_reserved;
	struct fiemap_extent fm_extents[0]; /* array of mapped extents (out) */
};

#define FIEMAP_MAX_OFFSET	(~0ULL)

#define FIEMAP_FLAG_SYNC	0x00000001 /* sync file data before map */
#define FIEMAP_FLAG_XATTR	0x00000002 /* map extended attribute tree */

#define FIEMAP_FLAGS_COMPAT	(FIEMAP_FLAG_SYNC | FIEMAP_FLAG_XATTR)

#define FIEMAP_EXTENT_LAST		0x00000001 /* Last extent in file. */
#define FIEMAP_EXTENT_UNKNOWN		0x00000002 /* Data location unknown. */
#define FIEMAP_EXTENT_DELALLOC		0x00000004 /* Location still pending.
						    * Sets EXTENT_UNKNOWN. */
#define FIEMAP_EXTENT_ENCODED		0x00000008 /* Data can not be read
						    * while fs is unmounted */
#define FIEMAP_EXTENT_DATA_ENCRYPTED	0x00000080 /* Data is encrypted by fs.
						    * Sets EXTENT_NO_BYPASS. */
#define FIEMAP_EXTENT_NOT_ALIGNED	0x00000100 /* Extent offsets may not be
						    * block aligned. */
#define FIEMAP_EXTENT_DATA_INLINE	0x00000200 /* Data mixed with metadata.
						    * Sets EXTENT_NOT_ALIGNED.*/
#define FIEMAP_EXTENT_DATA_TAIL		0x00000400 /* Multiple files in block.
						    * Sets EXTENT_NOT_ALIGNED.*/
#define FIEMAP_EXTENT_UNWRITTEN		0x00000800 /* Space allocated, but
						    * no data (i.e. zero). */
#define FIEMAP_EXTENT_MERGED		0x00001000 /* File does not natively
						    * support extents. Result
						    * merged for efficiency. */

#endif /* _LINUX_FIEMAP_H */
