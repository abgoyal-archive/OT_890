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
 * linux/fs/befs/io.c
 *
 * Copyright (C) 2001 Will Dyson <will_dyson@pobox.com
 *
 * Based on portions of file.c and inode.c 
 * by Makoto Kato (m_kato@ga2.so-net.ne.jp)
 *
 * Many thanks to Dominic Giampaolo, author of Practical File System
 * Design with the Be File System, for such a helpful book.
 *
 */

#include <linux/buffer_head.h>

#include "befs.h"
#include "io.h"

/*
 * Converts befs notion of disk addr to a disk offset and uses
 * linux kernel function sb_bread() to get the buffer containing
 * the offset. -Will Dyson
 *
 */

struct buffer_head *
befs_bread_iaddr(struct super_block *sb, befs_inode_addr iaddr)
{
	struct buffer_head *bh = NULL;
	befs_blocknr_t block = 0;
	befs_sb_info *befs_sb = BEFS_SB(sb);

	befs_debug(sb, "---> Enter befs_read_iaddr() "
		   "[%u, %hu, %hu]",
		   iaddr.allocation_group, iaddr.start, iaddr.len);

	if (iaddr.allocation_group > befs_sb->num_ags) {
		befs_error(sb, "BEFS: Invalid allocation group %u, max is %u",
			   iaddr.allocation_group, befs_sb->num_ags);
		goto error;
	}

	block = iaddr2blockno(sb, &iaddr);

	befs_debug(sb, "befs_read_iaddr: offset = %lu", block);

	bh = sb_bread(sb, block);

	if (bh == NULL) {
		befs_error(sb, "Failed to read block %lu", block);
		goto error;
	}

	befs_debug(sb, "<--- befs_read_iaddr()");
	return bh;

      error:
	befs_debug(sb, "<--- befs_read_iaddr() ERROR");
	return NULL;
}

struct buffer_head *
befs_bread(struct super_block *sb, befs_blocknr_t block)
{
	struct buffer_head *bh = NULL;

	befs_debug(sb, "---> Enter befs_read() %Lu", block);

	bh = sb_bread(sb, block);

	if (bh == NULL) {
		befs_error(sb, "Failed to read block %lu", block);
		goto error;
	}

	befs_debug(sb, "<--- befs_read()");

	return bh;

      error:
	befs_debug(sb, "<--- befs_read() ERROR");
	return NULL;
}
