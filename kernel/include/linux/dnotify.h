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

#ifndef _LINUX_DNOTIFY_H
#define _LINUX_DNOTIFY_H
/*
 * Directory notification for Linux
 *
 * Copyright (C) 2000,2002 Stephen Rothwell
 */

#include <linux/fs.h>

struct dnotify_struct {
	struct dnotify_struct *	dn_next;
	unsigned long		dn_mask;
	int			dn_fd;
	struct file *		dn_filp;
	fl_owner_t		dn_owner;
};

#ifdef __KERNEL__


#ifdef CONFIG_DNOTIFY

extern void __inode_dir_notify(struct inode *, unsigned long);
extern void dnotify_flush(struct file *, fl_owner_t);
extern int fcntl_dirnotify(int, struct file *, unsigned long);
extern void dnotify_parent(struct dentry *, unsigned long);

static inline void inode_dir_notify(struct inode *inode, unsigned long event)
{
	if (inode->i_dnotify_mask & (event))
		__inode_dir_notify(inode, event);
}

#else

static inline void __inode_dir_notify(struct inode *inode, unsigned long event)
{
}

static inline void dnotify_flush(struct file *filp, fl_owner_t id)
{
}

static inline int fcntl_dirnotify(int fd, struct file *filp, unsigned long arg)
{
	return -EINVAL;
}

static inline void dnotify_parent(struct dentry *dentry, unsigned long event)
{
}

static inline void inode_dir_notify(struct inode *inode, unsigned long event)
{
}

#endif /* CONFIG_DNOTIFY */

#endif /* __KERNEL __ */

#endif /* _LINUX_DNOTIFY_H */
