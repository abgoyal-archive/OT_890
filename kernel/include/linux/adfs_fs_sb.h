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
 *  linux/include/linux/adfs_fs_sb.h
 *
 * Copyright (C) 1997-1999 Russell King
 */

#ifndef _ADFS_FS_SB
#define _ADFS_FS_SB

/*
 * Forward-declare this
 */
struct adfs_discmap;
struct adfs_dir_ops;

/*
 * ADFS file system superblock data in memory
 */
struct adfs_sb_info {
	struct adfs_discmap *s_map;	/* bh list containing map		 */
	struct adfs_dir_ops *s_dir;	/* directory operations			 */

	uid_t		s_uid;		/* owner uid				 */
	gid_t		s_gid;		/* owner gid				 */
	umode_t		s_owner_mask;	/* ADFS owner perm -> unix perm		 */
	umode_t		s_other_mask;	/* ADFS other perm -> unix perm		 */

	__u32		s_ids_per_zone;	/* max. no ids in one zone		 */
	__u32		s_idlen;	/* length of ID in map			 */
	__u32		s_map_size;	/* sector size of a map			 */
	unsigned long	s_size;		/* total size (in blocks) of this fs	 */
	signed int	s_map2blk;	/* shift left by this for map->sector	 */
	unsigned int	s_log2sharesize;/* log2 share size			 */
	__le32		s_version;	/* disc format version			 */
	unsigned int	s_namelen;	/* maximum number of characters in name	 */
};

#endif
