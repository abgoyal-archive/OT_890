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

#ifndef _REISER_FS_I
#define _REISER_FS_I

#include <linux/list.h>

struct reiserfs_journal_list;

/** bitmasks for i_flags field in reiserfs-specific part of inode */
typedef enum {
    /** this says what format of key do all items (but stat data) of
      an object have.  If this is set, that format is 3.6 otherwise
      - 3.5 */
	i_item_key_version_mask = 0x0001,
    /** If this is unset, object has 3.5 stat data, otherwise, it has
      3.6 stat data with 64bit size, 32bit nlink etc. */
	i_stat_data_version_mask = 0x0002,
    /** file might need tail packing on close */
	i_pack_on_close_mask = 0x0004,
    /** don't pack tail of file */
	i_nopack_mask = 0x0008,
    /** If those is set, "safe link" was created for this file during
      truncate or unlink. Safe link is used to avoid leakage of disk
      space on crash with some files open, but unlinked. */
	i_link_saved_unlink_mask = 0x0010,
	i_link_saved_truncate_mask = 0x0020,
	i_has_xattr_dir = 0x0040,
	i_data_log = 0x0080,
	i_ever_mapped = 0x0100
} reiserfs_inode_flags;

struct reiserfs_inode_info {
	__u32 i_key[4];		/* key is still 4 32 bit integers */
    /** transient inode flags that are never stored on disk. Bitmasks
      for this field are defined above. */
	__u32 i_flags;

	__u32 i_first_direct_byte;	// offset of first byte stored in direct item.

	/* copy of persistent inode flags read from sd_attrs. */
	__u32 i_attrs;

	int i_prealloc_block;	/* first unused block of a sequence of unused blocks */
	int i_prealloc_count;	/* length of that sequence */
	struct list_head i_prealloc_list;	/* per-transaction list of inodes which
						 * have preallocated blocks */

	unsigned new_packing_locality:1;	/* new_packig_locality is created; new blocks
						 * for the contents of this directory should be
						 * displaced */

	/* we use these for fsync or O_SYNC to decide which transaction
	 ** needs to be committed in order for this inode to be properly
	 ** flushed */
	unsigned long i_trans_id;
	struct reiserfs_journal_list *i_jl;
	struct mutex i_mmap;
#ifdef CONFIG_REISERFS_FS_POSIX_ACL
	struct posix_acl *i_acl_access;
	struct posix_acl *i_acl_default;
#endif
#ifdef CONFIG_REISERFS_FS_XATTR
	struct rw_semaphore xattr_sem;
#endif
	struct inode vfs_inode;
};

#endif
