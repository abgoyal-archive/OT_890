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
 * QNX4 file system, Linux implementation.
 *
 * Version : 0.2.1
 *
 * Using parts of the xiafs filesystem.
 *
 * History :
 *
 * 28-05-1998 by Richard Frowijn : first release.
 * 20-06-1998 by Frank Denis : Linux 2.1.99+ & dcache support.
 */

#include <linux/string.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/qnx4_fs.h>
#include <linux/stat.h>
#include <linux/smp_lock.h>
#include <linux/buffer_head.h>


static int qnx4_readdir(struct file *filp, void *dirent, filldir_t filldir)
{
	struct inode *inode = filp->f_path.dentry->d_inode;
	unsigned int offset;
	struct buffer_head *bh;
	struct qnx4_inode_entry *de;
	struct qnx4_link_info *le;
	unsigned long blknum;
	int ix, ino;
	int size;

	QNX4DEBUG(("qnx4_readdir:i_size = %ld\n", (long) inode->i_size));
	QNX4DEBUG(("filp->f_pos         = %ld\n", (long) filp->f_pos));

	lock_kernel();

	while (filp->f_pos < inode->i_size) {
		blknum = qnx4_block_map( inode, filp->f_pos >> QNX4_BLOCK_SIZE_BITS );
		bh = sb_bread(inode->i_sb, blknum);
		if(bh==NULL) {
			printk(KERN_ERR "qnx4_readdir: bread failed (%ld)\n", blknum);
			break;
		}
		ix = (int)(filp->f_pos >> QNX4_DIR_ENTRY_SIZE_BITS) % QNX4_INODES_PER_BLOCK;
		while (ix < QNX4_INODES_PER_BLOCK) {
			offset = ix * QNX4_DIR_ENTRY_SIZE;
			de = (struct qnx4_inode_entry *) (bh->b_data + offset);
			size = strlen(de->di_fname);
			if (size) {
				if ( !( de->di_status & QNX4_FILE_LINK ) && size > QNX4_SHORT_NAME_MAX )
					size = QNX4_SHORT_NAME_MAX;
				else if ( size > QNX4_NAME_MAX )
					size = QNX4_NAME_MAX;

				if ( ( de->di_status & (QNX4_FILE_USED|QNX4_FILE_LINK) ) != 0 ) {
					QNX4DEBUG(("qnx4_readdir:%.*s\n", size, de->di_fname));
					if ( ( de->di_status & QNX4_FILE_LINK ) == 0 )
						ino = blknum * QNX4_INODES_PER_BLOCK + ix - 1;
					else {
						le  = (struct qnx4_link_info*)de;
						ino = ( le32_to_cpu(le->dl_inode_blk) - 1 ) *
							QNX4_INODES_PER_BLOCK +
							le->dl_inode_ndx;
					}
					if (filldir(dirent, de->di_fname, size, filp->f_pos, ino, DT_UNKNOWN) < 0) {
						brelse(bh);
						goto out;
					}
				}
			}
			ix++;
			filp->f_pos += QNX4_DIR_ENTRY_SIZE;
		}
		brelse(bh);
	}
out:
	unlock_kernel();
	return 0;
}

const struct file_operations qnx4_dir_operations =
{
	.read		= generic_read_dir,
	.readdir	= qnx4_readdir,
	.fsync		= file_fsync,
};

const struct inode_operations qnx4_dir_inode_operations =
{
	.lookup		= qnx4_lookup,
#ifdef CONFIG_QNX4FS_RW
	.create		= qnx4_create,
	.unlink		= qnx4_unlink,
	.rmdir		= qnx4_rmdir,
#endif
};
