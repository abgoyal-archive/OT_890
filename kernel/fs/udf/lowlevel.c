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
 * lowlevel.c
 *
 * PURPOSE
 *  Low Level Device Routines for the UDF filesystem
 *
 * COPYRIGHT
 *	This file is distributed under the terms of the GNU General Public
 *	License (GPL). Copies of the GPL can be obtained from:
 *		ftp://prep.ai.mit.edu/pub/gnu/GPL
 *	Each contributing author retains all rights to their own work.
 *
 *  (C) 1999-2001 Ben Fennema
 *
 * HISTORY
 *
 *  03/26/99 blf  Created.
 */

#include "udfdecl.h"

#include <linux/blkdev.h>
#include <linux/cdrom.h>
#include <asm/uaccess.h>

#include "udf_sb.h"

unsigned int udf_get_last_session(struct super_block *sb)
{
	struct cdrom_multisession ms_info;
	unsigned int vol_desc_start;
	struct block_device *bdev = sb->s_bdev;
	int i;

	vol_desc_start = 0;
	ms_info.addr_format = CDROM_LBA;
	i = ioctl_by_bdev(bdev, CDROMMULTISESSION, (unsigned long)&ms_info);

#define WE_OBEY_THE_WRITTEN_STANDARDS 1

	if (i == 0) {
		udf_debug("XA disk: %s, vol_desc_start=%d\n",
			  (ms_info.xa_flag ? "yes" : "no"), ms_info.addr.lba);
#if WE_OBEY_THE_WRITTEN_STANDARDS
		if (ms_info.xa_flag) /* necessary for a valid ms_info.addr */
#endif
			vol_desc_start = ms_info.addr.lba;
	} else {
		udf_debug("CDROMMULTISESSION not supported: rc=%d\n", i);
	}
	return vol_desc_start;
}

unsigned long udf_get_last_block(struct super_block *sb)
{
	struct block_device *bdev = sb->s_bdev;
	unsigned long lblock = 0;

	if (ioctl_by_bdev(bdev, CDROM_LAST_WRITTEN, (unsigned long) &lblock))
		lblock = bdev->bd_inode->i_size >> sb->s_blocksize_bits;

	if (lblock)
		return lblock - 1;
	else
		return 0;
}
