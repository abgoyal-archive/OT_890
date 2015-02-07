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

#ifndef __SHMEM_FS_H
#define __SHMEM_FS_H

#include <linux/swap.h>
#include <linux/mempolicy.h>

/* inode in-kernel data */

#define SHMEM_NR_DIRECT 16

struct shmem_inode_info {
	spinlock_t		lock;
	unsigned long		flags;
	unsigned long		alloced;	/* data pages alloced to file */
	unsigned long		swapped;	/* subtotal assigned to swap */
	unsigned long		next_index;	/* highest alloced index + 1 */
	struct shared_policy	policy;		/* NUMA memory alloc policy */
	struct page		*i_indirect;	/* top indirect blocks page */
	swp_entry_t		i_direct[SHMEM_NR_DIRECT]; /* first blocks */
	struct list_head	swaplist;	/* chain of maybes on swap */
	struct inode		vfs_inode;
#ifdef CONFIG_TMPFS_POSIX_ACL
	struct posix_acl	*i_acl;
	struct posix_acl	*i_default_acl;
#endif
};

struct shmem_sb_info {
	unsigned long max_blocks;   /* How many blocks are allowed */
	unsigned long free_blocks;  /* How many are left for allocation */
	unsigned long max_inodes;   /* How many inodes are allowed */
	unsigned long free_inodes;  /* How many are left for allocation */
	spinlock_t stat_lock;	    /* Serialize shmem_sb_info changes */
	uid_t uid;		    /* Mount uid for root directory */
	gid_t gid;		    /* Mount gid for root directory */
	mode_t mode;		    /* Mount mode for root directory */
	struct mempolicy *mpol;     /* default memory policy for mappings */
};

static inline struct shmem_inode_info *SHMEM_I(struct inode *inode)
{
	return container_of(inode, struct shmem_inode_info, vfs_inode);
}

#ifdef CONFIG_TMPFS_POSIX_ACL
int shmem_permission(struct inode *, int);
int shmem_acl_init(struct inode *, struct inode *);
void shmem_acl_destroy_inode(struct inode *);

extern struct xattr_handler shmem_xattr_acl_access_handler;
extern struct xattr_handler shmem_xattr_acl_default_handler;

extern struct generic_acl_operations shmem_acl_ops;

#else
static inline int shmem_acl_init(struct inode *inode, struct inode *dir)
{
	return 0;
}
static inline void shmem_acl_destroy_inode(struct inode *inode)
{
}
#endif  /* CONFIG_TMPFS_POSIX_ACL */

#endif
