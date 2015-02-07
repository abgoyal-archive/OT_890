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

#include <linux/file.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/mount.h>
#include <linux/namei.h>

#include <asm/uaccess.h>

#include "spufs.h"

/**
 * sys_spu_run - run code loaded into an SPU
 *
 * @unpc:    next program counter for the SPU
 * @ustatus: status of the SPU
 *
 * This system call transfers the control of execution of a
 * user space thread to an SPU. It will return when the
 * SPU has finished executing or when it hits an error
 * condition and it will be interrupted if a signal needs
 * to be delivered to a handler in user space.
 *
 * The next program counter is set to the passed value
 * before the SPU starts fetching code and the user space
 * pointer gets updated with the new value when returning
 * from kernel space.
 *
 * The status value returned from spu_run reflects the
 * value of the spu_status register after the SPU has stopped.
 *
 */
static long do_spu_run(struct file *filp,
			__u32 __user *unpc,
			__u32 __user *ustatus)
{
	long ret;
	struct spufs_inode_info *i;
	u32 npc, status;

	ret = -EFAULT;
	if (get_user(npc, unpc))
		goto out;

	/* check if this file was created by spu_create */
	ret = -EINVAL;
	if (filp->f_op != &spufs_context_fops)
		goto out;

	i = SPUFS_I(filp->f_path.dentry->d_inode);
	ret = spufs_run_spu(i->i_ctx, &npc, &status);

	if (put_user(npc, unpc))
		ret = -EFAULT;

	if (ustatus && put_user(status, ustatus))
		ret = -EFAULT;
out:
	return ret;
}

static long do_spu_create(const char __user *pathname, unsigned int flags,
		mode_t mode, struct file *neighbor)
{
	char *tmp;
	int ret;

	tmp = getname(pathname);
	ret = PTR_ERR(tmp);
	if (!IS_ERR(tmp)) {
		struct nameidata nd;

		ret = path_lookup(tmp, LOOKUP_PARENT, &nd);
		if (!ret) {
			nd.flags |= LOOKUP_OPEN | LOOKUP_CREATE;
			ret = spufs_create(&nd, flags, mode, neighbor);
			path_put(&nd.path);
		}
		putname(tmp);
	}

	return ret;
}

struct spufs_calls spufs_calls = {
	.create_thread = do_spu_create,
	.spu_run = do_spu_run,
	.coredump_extra_notes_size = spufs_coredump_extra_notes_size,
	.coredump_extra_notes_write = spufs_coredump_extra_notes_write,
	.notify_spus_active = do_notify_spus_active,
	.owner = THIS_MODULE,
};
