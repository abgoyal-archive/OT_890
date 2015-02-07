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
 * fs/sysfs/sysfs.h - sysfs internal header file
 *
 * Copyright (c) 2001-3 Patrick Mochel
 * Copyright (c) 2007 SUSE Linux Products GmbH
 * Copyright (c) 2007 Tejun Heo <teheo@suse.de>
 *
 * This file is released under the GPLv2.
 */

struct sysfs_open_dirent;

/* type-specific structures for sysfs_dirent->s_* union members */
struct sysfs_elem_dir {
	struct kobject		*kobj;
	/* children list starts here and goes through sd->s_sibling */
	struct sysfs_dirent	*children;
};

struct sysfs_elem_symlink {
	struct sysfs_dirent	*target_sd;
};

struct sysfs_elem_attr {
	struct attribute	*attr;
	struct sysfs_open_dirent *open;
};

struct sysfs_elem_bin_attr {
	struct bin_attribute	*bin_attr;
};

/*
 * sysfs_dirent - the building block of sysfs hierarchy.  Each and
 * every sysfs node is represented by single sysfs_dirent.
 *
 * As long as s_count reference is held, the sysfs_dirent itself is
 * accessible.  Dereferencing s_elem or any other outer entity
 * requires s_active reference.
 */
struct sysfs_dirent {
	atomic_t		s_count;
	atomic_t		s_active;
	struct sysfs_dirent	*s_parent;
	struct sysfs_dirent	*s_sibling;
	const char		*s_name;

	union {
		struct sysfs_elem_dir		s_dir;
		struct sysfs_elem_symlink	s_symlink;
		struct sysfs_elem_attr		s_attr;
		struct sysfs_elem_bin_attr	s_bin_attr;
	};

	unsigned int		s_flags;
	ino_t			s_ino;
	umode_t			s_mode;
	struct iattr		*s_iattr;
};

#define SD_DEACTIVATED_BIAS		INT_MIN

#define SYSFS_TYPE_MASK			0x00ff
#define SYSFS_DIR			0x0001
#define SYSFS_KOBJ_ATTR			0x0002
#define SYSFS_KOBJ_BIN_ATTR		0x0004
#define SYSFS_KOBJ_LINK			0x0008
#define SYSFS_COPY_NAME			(SYSFS_DIR | SYSFS_KOBJ_LINK)

#define SYSFS_FLAG_MASK			~SYSFS_TYPE_MASK
#define SYSFS_FLAG_REMOVED		0x0200

static inline unsigned int sysfs_type(struct sysfs_dirent *sd)
{
	return sd->s_flags & SYSFS_TYPE_MASK;
}

/*
 * Context structure to be used while adding/removing nodes.
 */
struct sysfs_addrm_cxt {
	struct sysfs_dirent	*parent_sd;
	struct inode		*parent_inode;
	struct sysfs_dirent	*removed;
	int			cnt;
};

/*
 * mount.c
 */
extern struct sysfs_dirent sysfs_root;
extern struct super_block *sysfs_sb;
extern struct kmem_cache *sysfs_dir_cachep;

/*
 * dir.c
 */
extern struct mutex sysfs_mutex;
extern struct mutex sysfs_rename_mutex;
extern spinlock_t sysfs_assoc_lock;

extern const struct file_operations sysfs_dir_operations;
extern const struct inode_operations sysfs_dir_inode_operations;

struct dentry *sysfs_get_dentry(struct sysfs_dirent *sd);
struct sysfs_dirent *sysfs_get_active_two(struct sysfs_dirent *sd);
void sysfs_put_active_two(struct sysfs_dirent *sd);
void sysfs_addrm_start(struct sysfs_addrm_cxt *acxt,
		       struct sysfs_dirent *parent_sd);
int __sysfs_add_one(struct sysfs_addrm_cxt *acxt, struct sysfs_dirent *sd);
int sysfs_add_one(struct sysfs_addrm_cxt *acxt, struct sysfs_dirent *sd);
void sysfs_remove_one(struct sysfs_addrm_cxt *acxt, struct sysfs_dirent *sd);
void sysfs_addrm_finish(struct sysfs_addrm_cxt *acxt);

struct sysfs_dirent *sysfs_find_dirent(struct sysfs_dirent *parent_sd,
				       const unsigned char *name);
struct sysfs_dirent *sysfs_get_dirent(struct sysfs_dirent *parent_sd,
				      const unsigned char *name);
struct sysfs_dirent *sysfs_new_dirent(const char *name, umode_t mode, int type);

void release_sysfs_dirent(struct sysfs_dirent *sd);

int sysfs_create_subdir(struct kobject *kobj, const char *name,
			struct sysfs_dirent **p_sd);
void sysfs_remove_subdir(struct sysfs_dirent *sd);

static inline struct sysfs_dirent *__sysfs_get(struct sysfs_dirent *sd)
{
	if (sd) {
		WARN_ON(!atomic_read(&sd->s_count));
		atomic_inc(&sd->s_count);
	}
	return sd;
}
#define sysfs_get(sd) __sysfs_get(sd)

static inline void __sysfs_put(struct sysfs_dirent *sd)
{
	if (sd && atomic_dec_and_test(&sd->s_count))
		release_sysfs_dirent(sd);
}
#define sysfs_put(sd) __sysfs_put(sd)

/*
 * inode.c
 */
struct inode *sysfs_get_inode(struct sysfs_dirent *sd);
int sysfs_setattr(struct dentry *dentry, struct iattr *iattr);
int sysfs_hash_and_remove(struct sysfs_dirent *dir_sd, const char *name);
int sysfs_inode_init(void);

/*
 * file.c
 */
extern const struct file_operations sysfs_file_operations;

int sysfs_add_file(struct sysfs_dirent *dir_sd,
		   const struct attribute *attr, int type);

int sysfs_add_file_mode(struct sysfs_dirent *dir_sd,
			const struct attribute *attr, int type, mode_t amode);
/*
 * bin.c
 */
extern const struct file_operations bin_fops;

/*
 * symlink.c
 */
extern const struct inode_operations sysfs_symlink_inode_operations;
