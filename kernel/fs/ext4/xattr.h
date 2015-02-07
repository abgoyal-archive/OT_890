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
  File: fs/ext4/xattr.h

  On-disk format of extended attributes for the ext4 filesystem.

  (C) 2001 Andreas Gruenbacher, <a.gruenbacher@computer.org>
*/

#include <linux/xattr.h>

/* Magic value in attribute blocks */
#define EXT4_XATTR_MAGIC		0xEA020000

/* Maximum number of references to one attribute block */
#define EXT4_XATTR_REFCOUNT_MAX		1024

/* Name indexes */
#define EXT4_XATTR_INDEX_USER			1
#define EXT4_XATTR_INDEX_POSIX_ACL_ACCESS	2
#define EXT4_XATTR_INDEX_POSIX_ACL_DEFAULT	3
#define EXT4_XATTR_INDEX_TRUSTED		4
#define	EXT4_XATTR_INDEX_LUSTRE			5
#define EXT4_XATTR_INDEX_SECURITY	        6

struct ext4_xattr_header {
	__le32	h_magic;	/* magic number for identification */
	__le32	h_refcount;	/* reference count */
	__le32	h_blocks;	/* number of disk blocks used */
	__le32	h_hash;		/* hash value of all attributes */
	__u32	h_reserved[4];	/* zero right now */
};

struct ext4_xattr_ibody_header {
	__le32	h_magic;	/* magic number for identification */
};

struct ext4_xattr_entry {
	__u8	e_name_len;	/* length of name */
	__u8	e_name_index;	/* attribute name index */
	__le16	e_value_offs;	/* offset in disk block of value */
	__le32	e_value_block;	/* disk block attribute is stored on (n/i) */
	__le32	e_value_size;	/* size of attribute value */
	__le32	e_hash;		/* hash value of name and value */
	char	e_name[0];	/* attribute name */
};

#define EXT4_XATTR_PAD_BITS		2
#define EXT4_XATTR_PAD		(1<<EXT4_XATTR_PAD_BITS)
#define EXT4_XATTR_ROUND		(EXT4_XATTR_PAD-1)
#define EXT4_XATTR_LEN(name_len) \
	(((name_len) + EXT4_XATTR_ROUND + \
	sizeof(struct ext4_xattr_entry)) & ~EXT4_XATTR_ROUND)
#define EXT4_XATTR_NEXT(entry) \
	((struct ext4_xattr_entry *)( \
	 (char *)(entry) + EXT4_XATTR_LEN((entry)->e_name_len)))
#define EXT4_XATTR_SIZE(size) \
	(((size) + EXT4_XATTR_ROUND) & ~EXT4_XATTR_ROUND)

#define IHDR(inode, raw_inode) \
	((struct ext4_xattr_ibody_header *) \
		((void *)raw_inode + \
		EXT4_GOOD_OLD_INODE_SIZE + \
		EXT4_I(inode)->i_extra_isize))
#define IFIRST(hdr) ((struct ext4_xattr_entry *)((hdr)+1))

# ifdef CONFIG_EXT4_FS_XATTR

extern struct xattr_handler ext4_xattr_user_handler;
extern struct xattr_handler ext4_xattr_trusted_handler;
extern struct xattr_handler ext4_xattr_acl_access_handler;
extern struct xattr_handler ext4_xattr_acl_default_handler;
extern struct xattr_handler ext4_xattr_security_handler;

extern ssize_t ext4_listxattr(struct dentry *, char *, size_t);

extern int ext4_xattr_get(struct inode *, int, const char *, void *, size_t);
extern int ext4_xattr_set(struct inode *, int, const char *, const void *, size_t, int);
extern int ext4_xattr_set_handle(handle_t *, struct inode *, int, const char *, const void *, size_t, int);

extern void ext4_xattr_delete_inode(handle_t *, struct inode *);
extern void ext4_xattr_put_super(struct super_block *);

extern int ext4_expand_extra_isize_ea(struct inode *inode, int new_extra_isize,
			    struct ext4_inode *raw_inode, handle_t *handle);

extern int init_ext4_xattr(void);
extern void exit_ext4_xattr(void);

extern struct xattr_handler *ext4_xattr_handlers[];

# else  /* CONFIG_EXT4_FS_XATTR */

static inline int
ext4_xattr_get(struct inode *inode, int name_index, const char *name,
	       void *buffer, size_t size, int flags)
{
	return -EOPNOTSUPP;
}

static inline int
ext4_xattr_set(struct inode *inode, int name_index, const char *name,
	       const void *value, size_t size, int flags)
{
	return -EOPNOTSUPP;
}

static inline int
ext4_xattr_set_handle(handle_t *handle, struct inode *inode, int name_index,
	       const char *name, const void *value, size_t size, int flags)
{
	return -EOPNOTSUPP;
}

static inline void
ext4_xattr_delete_inode(handle_t *handle, struct inode *inode)
{
}

static inline void
ext4_xattr_put_super(struct super_block *sb)
{
}

static inline int
init_ext4_xattr(void)
{
	return 0;
}

static inline void
exit_ext4_xattr(void)
{
}

static inline int
ext4_expand_extra_isize_ea(struct inode *inode, int new_extra_isize,
			    struct ext4_inode *raw_inode, handle_t *handle)
{
	return -EOPNOTSUPP;
}

#define ext4_xattr_handlers	NULL

# endif  /* CONFIG_EXT4_FS_XATTR */

#ifdef CONFIG_EXT4_FS_SECURITY
extern int ext4_init_security(handle_t *handle, struct inode *inode,
				struct inode *dir);
#else
static inline int ext4_init_security(handle_t *handle, struct inode *inode,
				struct inode *dir)
{
	return 0;
}
#endif
