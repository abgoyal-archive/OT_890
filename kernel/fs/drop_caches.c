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
 * Implement the manual drop-all-pagecache function
 */

#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/writeback.h>
#include <linux/sysctl.h>
#include <linux/gfp.h>

/* A global variable is a bit ugly, but it keeps the code simple */
int sysctl_drop_caches;

static void drop_pagecache_sb(struct super_block *sb)
{
	struct inode *inode, *toput_inode = NULL;

	spin_lock(&inode_lock);
	list_for_each_entry(inode, &sb->s_inodes, i_sb_list) {
		if (inode->i_state & (I_FREEING|I_WILL_FREE))
			continue;
		if (inode->i_mapping->nrpages == 0)
			continue;
		__iget(inode);
		spin_unlock(&inode_lock);
		__invalidate_mapping_pages(inode->i_mapping, 0, -1, true);
		iput(toput_inode);
		toput_inode = inode;
		spin_lock(&inode_lock);
	}
	spin_unlock(&inode_lock);
	iput(toput_inode);
}

static void drop_pagecache(void)
{
	struct super_block *sb;

	spin_lock(&sb_lock);
restart:
	list_for_each_entry(sb, &super_blocks, s_list) {
		sb->s_count++;
		spin_unlock(&sb_lock);
		down_read(&sb->s_umount);
		if (sb->s_root)
			drop_pagecache_sb(sb);
		up_read(&sb->s_umount);
		spin_lock(&sb_lock);
		if (__put_super_and_need_restart(sb))
			goto restart;
	}
	spin_unlock(&sb_lock);
}

static void drop_slab(void)
{
	int nr_objects;

	do {
		nr_objects = shrink_slab(1000, GFP_KERNEL, 1000);
	} while (nr_objects > 10);
}

int drop_caches_sysctl_handler(ctl_table *table, int write,
	struct file *file, void __user *buffer, size_t *length, loff_t *ppos)
{
	proc_dointvec_minmax(table, write, file, buffer, length, ppos);
	if (write) {
		if (sysctl_drop_caches & 1)
			drop_pagecache();
		if (sysctl_drop_caches & 2)
			drop_slab();
	}
	return 0;
}
