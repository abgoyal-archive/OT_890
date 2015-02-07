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

/* getroot.c: get the root dentry for an NFS mount
 *
 * Copyright (C) 2006 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#include <linux/module.h>
#include <linux/init.h>

#include <linux/time.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/string.h>
#include <linux/stat.h>
#include <linux/errno.h>
#include <linux/unistd.h>
#include <linux/sunrpc/clnt.h>
#include <linux/sunrpc/stats.h>
#include <linux/nfs_fs.h>
#include <linux/nfs_mount.h>
#include <linux/nfs4_mount.h>
#include <linux/lockd/bind.h>
#include <linux/seq_file.h>
#include <linux/mount.h>
#include <linux/nfs_idmap.h>
#include <linux/vfs.h>
#include <linux/namei.h>
#include <linux/mnt_namespace.h>
#include <linux/security.h>

#include <asm/system.h>
#include <asm/uaccess.h>

#include "nfs4_fs.h"
#include "delegation.h"
#include "internal.h"

#define NFSDBG_FACILITY		NFSDBG_CLIENT

/*
 * Set the superblock root dentry.
 * Note that this function frees the inode in case of error.
 */
static int nfs_superblock_set_dummy_root(struct super_block *sb, struct inode *inode)
{
	/* The mntroot acts as the dummy root dentry for this superblock */
	if (sb->s_root == NULL) {
		sb->s_root = d_alloc_root(inode);
		if (sb->s_root == NULL) {
			iput(inode);
			return -ENOMEM;
		}
		/* Circumvent igrab(): we know the inode is not being freed */
		atomic_inc(&inode->i_count);
		/*
		 * Ensure that this dentry is invisible to d_find_alias().
		 * Otherwise, it may be spliced into the tree by
		 * d_materialise_unique if a parent directory from the same
		 * filesystem gets mounted at a later time.
		 * This again causes shrink_dcache_for_umount_subtree() to
		 * Oops, since the test for IS_ROOT() will fail.
		 */
		spin_lock(&dcache_lock);
		list_del_init(&sb->s_root->d_alias);
		spin_unlock(&dcache_lock);
	}
	return 0;
}

/*
 * get an NFS2/NFS3 root dentry from the root filehandle
 */
struct dentry *nfs_get_root(struct super_block *sb, struct nfs_fh *mntfh)
{
	struct nfs_server *server = NFS_SB(sb);
	struct nfs_fsinfo fsinfo;
	struct nfs_fattr fattr;
	struct dentry *mntroot;
	struct inode *inode;
	int error;

	/* get the actual root for this mount */
	fsinfo.fattr = &fattr;

	error = server->nfs_client->rpc_ops->getroot(server, mntfh, &fsinfo);
	if (error < 0) {
		dprintk("nfs_get_root: getattr error = %d\n", -error);
		return ERR_PTR(error);
	}

	inode = nfs_fhget(sb, mntfh, fsinfo.fattr);
	if (IS_ERR(inode)) {
		dprintk("nfs_get_root: get root inode failed\n");
		return ERR_CAST(inode);
	}

	error = nfs_superblock_set_dummy_root(sb, inode);
	if (error != 0)
		return ERR_PTR(error);

	/* root dentries normally start off anonymous and get spliced in later
	 * if the dentry tree reaches them; however if the dentry already
	 * exists, we'll pick it up at this point and use it as the root
	 */
	mntroot = d_obtain_alias(inode);
	if (IS_ERR(mntroot)) {
		dprintk("nfs_get_root: get root dentry failed\n");
		return mntroot;
	}

	security_d_instantiate(mntroot, inode);

	if (!mntroot->d_op)
		mntroot->d_op = server->nfs_client->rpc_ops->dentry_ops;

	return mntroot;
}

#ifdef CONFIG_NFS_V4

/*
 * Do a simple pathwalk from the root FH of the server to the nominated target
 * of the mountpoint
 * - give error on symlinks
 * - give error on ".." occurring in the path
 * - follow traversals
 */
int nfs4_path_walk(struct nfs_server *server,
		   struct nfs_fh *mntfh,
		   const char *path)
{
	struct nfs_fsinfo fsinfo;
	struct nfs_fattr fattr;
	struct nfs_fh lastfh;
	struct qstr name;
	int ret;

	dprintk("--> nfs4_path_walk(,,%s)\n", path);

	fsinfo.fattr = &fattr;
	nfs_fattr_init(&fattr);

	/* Eat leading slashes */
	while (*path == '/')
		path++;

	/* Start by getting the root filehandle from the server */
	ret = server->nfs_client->rpc_ops->getroot(server, mntfh, &fsinfo);
	if (ret < 0) {
		dprintk("nfs4_get_root: getroot error = %d\n", -ret);
		return ret;
	}

	if (fattr.type != NFDIR) {
		printk(KERN_ERR "nfs4_get_root:"
		       " getroot encountered non-directory\n");
		return -ENOTDIR;
	}

	/* FIXME: It is quite valid for the server to return a referral here */
	if (fattr.valid & NFS_ATTR_FATTR_V4_REFERRAL) {
		printk(KERN_ERR "nfs4_get_root:"
		       " getroot obtained referral\n");
		return -EREMOTE;
	}

next_component:
	dprintk("Next: %s\n", path);

	/* extract the next bit of the path */
	if (!*path)
		goto path_walk_complete;

	name.name = path;
	while (*path && *path != '/')
		path++;
	name.len = path - (const char *) name.name;

	if (name.len > NFS4_MAXNAMLEN)
		return -ENAMETOOLONG;

eat_dot_dir:
	while (*path == '/')
		path++;

	if (path[0] == '.' && (path[1] == '/' || !path[1])) {
		path += 2;
		goto eat_dot_dir;
	}

	/* FIXME: Why shouldn't the user be able to use ".." in the path? */
	if (path[0] == '.' && path[1] == '.' && (path[2] == '/' || !path[2])
	    ) {
		printk(KERN_ERR "nfs4_get_root:"
		       " Mount path contains reference to \"..\"\n");
		return -EINVAL;
	}

	/* lookup the next FH in the sequence */
	memcpy(&lastfh, mntfh, sizeof(lastfh));

	dprintk("LookupFH: %*.*s [%s]\n", name.len, name.len, name.name, path);

	ret = server->nfs_client->rpc_ops->lookupfh(server, &lastfh, &name,
						    mntfh, &fattr);
	if (ret < 0) {
		dprintk("nfs4_get_root: getroot error = %d\n", -ret);
		return ret;
	}

	if (fattr.type != NFDIR) {
		printk(KERN_ERR "nfs4_get_root:"
		       " lookupfh encountered non-directory\n");
		return -ENOTDIR;
	}

	/* FIXME: Referrals are quite valid here too */
	if (fattr.valid & NFS_ATTR_FATTR_V4_REFERRAL) {
		printk(KERN_ERR "nfs4_get_root:"
		       " lookupfh obtained referral\n");
		return -EREMOTE;
	}

	goto next_component;

path_walk_complete:
	memcpy(&server->fsid, &fattr.fsid, sizeof(server->fsid));
	dprintk("<-- nfs4_path_walk() = 0\n");
	return 0;
}

/*
 * get an NFS4 root dentry from the root filehandle
 */
struct dentry *nfs4_get_root(struct super_block *sb, struct nfs_fh *mntfh)
{
	struct nfs_server *server = NFS_SB(sb);
	struct nfs_fattr fattr;
	struct dentry *mntroot;
	struct inode *inode;
	int error;

	dprintk("--> nfs4_get_root()\n");

	/* get the info about the server and filesystem */
	error = nfs4_server_capabilities(server, mntfh);
	if (error < 0) {
		dprintk("nfs_get_root: getcaps error = %d\n",
			-error);
		return ERR_PTR(error);
	}

	/* get the actual root for this mount */
	error = server->nfs_client->rpc_ops->getattr(server, mntfh, &fattr);
	if (error < 0) {
		dprintk("nfs_get_root: getattr error = %d\n", -error);
		return ERR_PTR(error);
	}

	inode = nfs_fhget(sb, mntfh, &fattr);
	if (IS_ERR(inode)) {
		dprintk("nfs_get_root: get root inode failed\n");
		return ERR_CAST(inode);
	}

	error = nfs_superblock_set_dummy_root(sb, inode);
	if (error != 0)
		return ERR_PTR(error);

	/* root dentries normally start off anonymous and get spliced in later
	 * if the dentry tree reaches them; however if the dentry already
	 * exists, we'll pick it up at this point and use it as the root
	 */
	mntroot = d_obtain_alias(inode);
	if (IS_ERR(mntroot)) {
		dprintk("nfs_get_root: get root dentry failed\n");
		return mntroot;
	}

	security_d_instantiate(mntroot, inode);

	if (!mntroot->d_op)
		mntroot->d_op = server->nfs_client->rpc_ops->dentry_ops;

	dprintk("<-- nfs4_get_root()\n");
	return mntroot;
}

#endif /* CONFIG_NFS_V4 */
