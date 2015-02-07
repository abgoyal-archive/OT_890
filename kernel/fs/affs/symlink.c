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
 *  linux/fs/affs/symlink.c
 *
 *  1995  Hans-Joachim Widmaier - Modified for affs.
 *
 *  Copyright (C) 1991, 1992  Linus Torvalds
 *
 *  affs symlink handling code
 */

#include "affs.h"

static int affs_symlink_readpage(struct file *file, struct page *page)
{
	struct buffer_head *bh;
	struct inode *inode = page->mapping->host;
	char *link = kmap(page);
	struct slink_front *lf;
	int err;
	int			 i, j;
	char			 c;
	char			 lc;
	char			*pf;

	pr_debug("AFFS: follow_link(ino=%lu)\n",inode->i_ino);

	err = -EIO;
	bh = affs_bread(inode->i_sb, inode->i_ino);
	if (!bh)
		goto fail;
	i  = 0;
	j  = 0;
	lf = (struct slink_front *)bh->b_data;
	lc = 0;
	pf = AFFS_SB(inode->i_sb)->s_prefix ? AFFS_SB(inode->i_sb)->s_prefix : "/";

	if (strchr(lf->symname,':')) {	/* Handle assign or volume name */
		while (i < 1023 && (c = pf[i]))
			link[i++] = c;
		while (i < 1023 && lf->symname[j] != ':')
			link[i++] = lf->symname[j++];
		if (i < 1023)
			link[i++] = '/';
		j++;
		lc = '/';
	}
	while (i < 1023 && (c = lf->symname[j])) {
		if (c == '/' && lc == '/' && i < 1020) {	/* parent dir */
			link[i++] = '.';
			link[i++] = '.';
		}
		link[i++] = c;
		lc = c;
		j++;
	}
	link[i] = '\0';
	affs_brelse(bh);
	SetPageUptodate(page);
	kunmap(page);
	unlock_page(page);
	return 0;
fail:
	SetPageError(page);
	kunmap(page);
	unlock_page(page);
	return err;
}

const struct address_space_operations affs_symlink_aops = {
	.readpage	= affs_symlink_readpage,
};

const struct inode_operations affs_symlink_inode_operations = {
	.readlink	= generic_readlink,
	.follow_link	= page_follow_link_light,
	.put_link	= page_put_link,
	.setattr	= affs_notify_change,
};
