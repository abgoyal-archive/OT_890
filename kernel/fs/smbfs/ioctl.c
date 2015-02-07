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
 *  ioctl.c
 *
 *  Copyright (C) 1995, 1996 by Volker Lendecke
 *  Copyright (C) 1997 by Volker Lendecke
 *
 *  Please add a note about your changes to smbfs in the ChangeLog file.
 */

#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/ioctl.h>
#include <linux/time.h>
#include <linux/mm.h>
#include <linux/highuid.h>
#include <linux/net.h>

#include <linux/smb_fs.h>
#include <linux/smb_mount.h>

#include <asm/uaccess.h>

#include "proto.h"

int
smb_ioctl(struct inode *inode, struct file *filp,
	  unsigned int cmd, unsigned long arg)
{
	struct smb_sb_info *server = server_from_inode(inode);
	struct smb_conn_opt opt;
	int result = -EINVAL;

	switch (cmd) {
		uid16_t uid16;
		uid_t uid32;
	case SMB_IOC_GETMOUNTUID:
		SET_UID(uid16, server->mnt->mounted_uid);
		result = put_user(uid16, (uid16_t __user *) arg);
		break;
	case SMB_IOC_GETMOUNTUID32:
		SET_UID(uid32, server->mnt->mounted_uid);
		result = put_user(uid32, (uid_t __user *) arg);
		break;

	case SMB_IOC_NEWCONN:
		/* arg is smb_conn_opt, or NULL if no connection was made */
		if (!arg) {
			result = 0;
			smb_lock_server(server);
			server->state = CONN_RETRIED;
			printk(KERN_ERR "Connection attempt failed!  [%d]\n",
			       server->conn_error);
			smbiod_flush(server);
			smb_unlock_server(server);
			break;
		}

		result = -EFAULT;
		if (!copy_from_user(&opt, (void __user *)arg, sizeof(opt)))
			result = smb_newconn(server, &opt);
		break;
	default:
		break;
	}

	return result;
}
