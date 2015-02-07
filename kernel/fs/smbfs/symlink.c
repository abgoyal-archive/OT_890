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
 *  symlink.c
 *
 *  Copyright (C) 2002 by John Newbigin
 *
 *  Please add a note about your changes to smbfs in the ChangeLog file.
 */

#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/fcntl.h>
#include <linux/stat.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/pagemap.h>
#include <linux/net.h>
#include <linux/namei.h>

#include <asm/uaccess.h>
#include <asm/system.h>

#include <linux/smbno.h>
#include <linux/smb_fs.h>

#include "smb_debug.h"
#include "proto.h"

int smb_symlink(struct inode *inode, struct dentry *dentry, const char *oldname)
{
	DEBUG1("create symlink %s -> %s/%s\n", oldname, DENTRY_PATH(dentry));

	return smb_proc_symlink(server_from_dentry(dentry), dentry, oldname);
}

static void *smb_follow_link(struct dentry *dentry, struct nameidata *nd)
{
	char *link = __getname();
	DEBUG1("followlink of %s/%s\n", DENTRY_PATH(dentry));

	if (!link) {
		link = ERR_PTR(-ENOMEM);
	} else {
		int len = smb_proc_read_link(server_from_dentry(dentry),
						dentry, link, PATH_MAX - 1);
		if (len < 0) {
			__putname(link);
			link = ERR_PTR(len);
		} else {
			link[len] = 0;
		}
	}
	nd_set_link(nd, link);
	return NULL;
}

static void smb_put_link(struct dentry *dentry, struct nameidata *nd, void *p)
{
	char *s = nd_get_link(nd);
	if (!IS_ERR(s))
		__putname(s);
}

const struct inode_operations smb_link_inode_operations =
{
	.readlink	= generic_readlink,
	.follow_link	= smb_follow_link,
	.put_link	= smb_put_link,
};
