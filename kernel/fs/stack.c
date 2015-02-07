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

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/fs_stack.h>

/* does _NOT_ require i_mutex to be held.
 *
 * This function cannot be inlined since i_size_{read,write} is rather
 * heavy-weight on 32-bit systems
 */
void fsstack_copy_inode_size(struct inode *dst, const struct inode *src)
{
	i_size_write(dst, i_size_read((struct inode *)src));
	dst->i_blocks = src->i_blocks;
}
EXPORT_SYMBOL_GPL(fsstack_copy_inode_size);

/* copy all attributes; get_nlinks is optional way to override the i_nlink
 * copying
 */
void fsstack_copy_attr_all(struct inode *dest, const struct inode *src,
				int (*get_nlinks)(struct inode *))
{
	dest->i_mode = src->i_mode;
	dest->i_uid = src->i_uid;
	dest->i_gid = src->i_gid;
	dest->i_rdev = src->i_rdev;
	dest->i_atime = src->i_atime;
	dest->i_mtime = src->i_mtime;
	dest->i_ctime = src->i_ctime;
	dest->i_blkbits = src->i_blkbits;
	dest->i_flags = src->i_flags;

	/*
	 * Update the nlinks AFTER updating the above fields, because the
	 * get_links callback may depend on them.
	 */
	if (!get_nlinks)
		dest->i_nlink = src->i_nlink;
	else
		dest->i_nlink = (*get_nlinks)(dest);
}
EXPORT_SYMBOL_GPL(fsstack_copy_attr_all);
