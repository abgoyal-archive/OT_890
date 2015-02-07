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

#include <linux/init.h>
#include <linux/posix_acl.h>

#define REISERFS_ACL_VERSION	0x0001

typedef struct {
	__le16 e_tag;
	__le16 e_perm;
	__le32 e_id;
} reiserfs_acl_entry;

typedef struct {
	__le16 e_tag;
	__le16 e_perm;
} reiserfs_acl_entry_short;

typedef struct {
	__le32 a_version;
} reiserfs_acl_header;

static inline size_t reiserfs_acl_size(int count)
{
	if (count <= 4) {
		return sizeof(reiserfs_acl_header) +
		    count * sizeof(reiserfs_acl_entry_short);
	} else {
		return sizeof(reiserfs_acl_header) +
		    4 * sizeof(reiserfs_acl_entry_short) +
		    (count - 4) * sizeof(reiserfs_acl_entry);
	}
}

static inline int reiserfs_acl_count(size_t size)
{
	ssize_t s;
	size -= sizeof(reiserfs_acl_header);
	s = size - 4 * sizeof(reiserfs_acl_entry_short);
	if (s < 0) {
		if (size % sizeof(reiserfs_acl_entry_short))
			return -1;
		return size / sizeof(reiserfs_acl_entry_short);
	} else {
		if (s % sizeof(reiserfs_acl_entry))
			return -1;
		return s / sizeof(reiserfs_acl_entry) + 4;
	}
}

#ifdef CONFIG_REISERFS_FS_POSIX_ACL
struct posix_acl *reiserfs_get_acl(struct inode *inode, int type);
int reiserfs_acl_chmod(struct inode *inode);
int reiserfs_inherit_default_acl(struct inode *dir, struct dentry *dentry,
				 struct inode *inode);
int reiserfs_cache_default_acl(struct inode *dir);
extern int reiserfs_xattr_posix_acl_init(void) __init;
extern int reiserfs_xattr_posix_acl_exit(void);
extern struct reiserfs_xattr_handler posix_acl_default_handler;
extern struct reiserfs_xattr_handler posix_acl_access_handler;

static inline void reiserfs_init_acl_access(struct inode *inode)
{
	REISERFS_I(inode)->i_acl_access = NULL;
}

static inline void reiserfs_init_acl_default(struct inode *inode)
{
	REISERFS_I(inode)->i_acl_default = NULL;
}
#else

#define reiserfs_cache_default_acl(inode) 0

static inline struct posix_acl *reiserfs_get_acl(struct inode *inode, int type)
{
	return NULL;
}

static inline int reiserfs_xattr_posix_acl_init(void)
{
	return 0;
}

static inline int reiserfs_xattr_posix_acl_exit(void)
{
	return 0;
}

static inline int reiserfs_acl_chmod(struct inode *inode)
{
	return 0;
}

static inline int
reiserfs_inherit_default_acl(const struct inode *dir, struct dentry *dentry,
			     struct inode *inode)
{
	return 0;
}

static inline void reiserfs_init_acl_access(struct inode *inode)
{
}

static inline void reiserfs_init_acl_default(struct inode *inode)
{
}
#endif
