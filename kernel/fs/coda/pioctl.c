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
 * Pioctl operations for Coda.
 * Original version: (C) 1996 Peter Braam 
 * Rewritten for Linux 2.1: (C) 1997 Carnegie Mellon University
 *
 * Carnegie Mellon encourages users of this code to contribute improvements
 * to the Coda project. Contact Peter Braam <coda@cs.cmu.edu>.
 */

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/time.h>
#include <linux/fs.h>
#include <linux/stat.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/namei.h>
#include <linux/module.h>
#include <asm/uaccess.h>

#include <linux/coda.h>
#include <linux/coda_linux.h>
#include <linux/coda_fs_i.h>
#include <linux/coda_psdev.h>

/* pioctl ops */
static int coda_ioctl_permission(struct inode *inode, int mask);
static int coda_pioctl(struct inode * inode, struct file * filp, 
                       unsigned int cmd, unsigned long user_data);

/* exported from this file */
const struct inode_operations coda_ioctl_inode_operations =
{
	.permission	= coda_ioctl_permission,
	.setattr	= coda_setattr,
};

const struct file_operations coda_ioctl_operations = {
	.owner		= THIS_MODULE,
	.ioctl		= coda_pioctl,
};

/* the coda pioctl inode ops */
static int coda_ioctl_permission(struct inode *inode, int mask)
{
	return (mask & MAY_EXEC) ? -EACCES : 0;
}

static int coda_pioctl(struct inode * inode, struct file * filp, 
                       unsigned int cmd, unsigned long user_data)
{
	struct path path;
        int error;
	struct PioctlData data;
        struct inode *target_inode = NULL;
        struct coda_inode_info *cnp;

        /* get the Pioctl data arguments from user space */
        if (copy_from_user(&data, (void __user *)user_data, sizeof(data))) {
	    return -EINVAL;
	}
       
        /* 
         * Look up the pathname. Note that the pathname is in 
         * user memory, and namei takes care of this
         */
        if (data.follow) {
                error = user_path(data.path, &path);
	} else {
	        error = user_lpath(data.path, &path);
	}
		
	if ( error ) {
		return error;
        } else {
		target_inode = path.dentry->d_inode;
	}
	
	/* return if it is not a Coda inode */
	if ( target_inode->i_sb != inode->i_sb ) {
		path_put(&path);
	        return  -EINVAL;
	}

	/* now proceed to make the upcall */
        cnp = ITOC(target_inode);

	error = venus_pioctl(inode->i_sb, &(cnp->c_fid), cmd, &data);

	path_put(&path);
        return error;
}

