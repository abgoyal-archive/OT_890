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
 * linux/fs/9p/vfs_dir.c
 *
 * This file contains vfs directory ops for the 9P2000 protocol.
 *
 *  Copyright (C) 2004 by Eric Van Hensbergen <ericvh@gmail.com>
 *  Copyright (C) 2002 by Ron Minnich <rminnich@lanl.gov>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2
 *  as published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to:
 *  Free Software Foundation
 *  51 Franklin Street, Fifth Floor
 *  Boston, MA  02111-1301  USA
 *
 */

#include <linux/module.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/file.h>
#include <linux/stat.h>
#include <linux/string.h>
#include <linux/sched.h>
#include <linux/inet.h>
#include <linux/idr.h>
#include <net/9p/9p.h>
#include <net/9p/client.h>

#include "v9fs.h"
#include "v9fs_vfs.h"
#include "fid.h"

/**
 * dt_type - return file type
 * @mistat: mistat structure
 *
 */

static inline int dt_type(struct p9_wstat *mistat)
{
	unsigned long perm = mistat->mode;
	int rettype = DT_REG;

	if (perm & P9_DMDIR)
		rettype = DT_DIR;
	if (perm & P9_DMSYMLINK)
		rettype = DT_LNK;

	return rettype;
}

/**
 * v9fs_dir_readdir - read a directory
 * @filp: opened file structure
 * @dirent: directory structure ???
 * @filldir: function to populate directory structure ???
 *
 */

static int v9fs_dir_readdir(struct file *filp, void *dirent, filldir_t filldir)
{
	int over;
	struct p9_wstat st;
	int err;
	struct p9_fid *fid;
	int buflen;
	char *statbuf;
	int n, i = 0;

	P9_DPRINTK(P9_DEBUG_VFS, "name %s\n", filp->f_path.dentry->d_name.name);
	fid = filp->private_data;

	buflen = fid->clnt->msize - P9_IOHDRSZ;
	statbuf = kmalloc(buflen, GFP_KERNEL);
	if (!statbuf)
		return -ENOMEM;

	while (1) {
		err = v9fs_file_readn(filp, statbuf, NULL, buflen,
								fid->rdir_fpos);
		if (err <= 0)
			break;

		n = err;
		while (i < n) {
			err = p9stat_read(statbuf + i, buflen-i, &st,
							fid->clnt->dotu);
			if (err) {
				P9_DPRINTK(P9_DEBUG_VFS, "returned %d\n", err);
				err = -EIO;
				p9stat_free(&st);
				goto free_and_exit;
			}

			i += st.size+2;
			fid->rdir_fpos += st.size+2;

			over = filldir(dirent, st.name, strlen(st.name),
			    filp->f_pos, v9fs_qid2ino(&st.qid), dt_type(&st));

			filp->f_pos += st.size+2;

			p9stat_free(&st);

			if (over) {
				err = 0;
				goto free_and_exit;
			}
		}
	}

free_and_exit:
	kfree(statbuf);
	return err;
}


/**
 * v9fs_dir_release - close a directory
 * @inode: inode of the directory
 * @filp: file pointer to a directory
 *
 */

int v9fs_dir_release(struct inode *inode, struct file *filp)
{
	struct p9_fid *fid;

	fid = filp->private_data;
	P9_DPRINTK(P9_DEBUG_VFS,
			"inode: %p filp: %p fid: %d\n", inode, filp, fid->fid);
	filemap_write_and_wait(inode->i_mapping);
	p9_client_clunk(fid);
	return 0;
}

const struct file_operations v9fs_dir_operations = {
	.read = generic_read_dir,
	.llseek = generic_file_llseek,
	.readdir = v9fs_dir_readdir,
	.open = v9fs_file_open,
	.release = v9fs_dir_release,
};
