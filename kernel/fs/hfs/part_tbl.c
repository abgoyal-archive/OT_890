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
 *  linux/fs/hfs/part_tbl.c
 *
 * Copyright (C) 1996-1997  Paul H. Hargrove
 * (C) 2003 Ardis Technologies <roman@ardistech.com>
 * This file may be distributed under the terms of the GNU General Public License.
 *
 * Original code to handle the new style Mac partition table based on
 * a patch contributed by Holger Schemel (aeglos@valinor.owl.de).
 */

#include "hfs_fs.h"

/*
 * The new style Mac partition map
 *
 * For each partition on the media there is a physical block (512-byte
 * block) containing one of these structures.  These blocks are
 * contiguous starting at block 1.
 */
struct new_pmap {
	__be16	pmSig;		/* signature */
	__be16	reSigPad;	/* padding */
	__be32	pmMapBlkCnt;	/* partition blocks count */
	__be32	pmPyPartStart;	/* physical block start of partition */
	__be32	pmPartBlkCnt;	/* physical block count of partition */
	u8	pmPartName[32];	/* (null terminated?) string
				   giving the name of this
				   partition */
	u8	pmPartType[32];	/* (null terminated?) string
				   giving the type of this
				   partition */
	/* a bunch more stuff we don't need */
} __packed;

/*
 * The old style Mac partition map
 *
 * The partition map consists for a 2-byte signature followed by an
 * array of these structures.  The map is terminated with an all-zero
 * one of these.
 */
struct old_pmap {
	__be16		pdSig;	/* Signature bytes */
	struct 	old_pmap_entry {
		__be32	pdStart;
		__be32	pdSize;
		__be32	pdFSID;
	}	pdEntry[42];
} __packed;

/*
 * hfs_part_find()
 *
 * Parse the partition map looking for the
 * start and length of the 'part'th HFS partition.
 */
int hfs_part_find(struct super_block *sb,
		  sector_t *part_start, sector_t *part_size)
{
	struct buffer_head *bh;
	__be16 *data;
	int i, size, res;

	res = -ENOENT;
	bh = sb_bread512(sb, *part_start + HFS_PMAP_BLK, data);
	if (!bh)
		return -EIO;

	switch (be16_to_cpu(*data)) {
	case HFS_OLD_PMAP_MAGIC:
	  {
		struct old_pmap *pm;
		struct old_pmap_entry *p;

		pm = (struct old_pmap *)bh->b_data;
		p = pm->pdEntry;
		size = 42;
		for (i = 0; i < size; p++, i++) {
			if (p->pdStart && p->pdSize &&
			    p->pdFSID == cpu_to_be32(0x54465331)/*"TFS1"*/ &&
			    (HFS_SB(sb)->part < 0 || HFS_SB(sb)->part == i)) {
				*part_start += be32_to_cpu(p->pdStart);
				*part_size = be32_to_cpu(p->pdSize);
				res = 0;
			}
		}
		break;
	  }
	case HFS_NEW_PMAP_MAGIC:
	  {
		struct new_pmap *pm;

		pm = (struct new_pmap *)bh->b_data;
		size = be32_to_cpu(pm->pmMapBlkCnt);
		for (i = 0; i < size;) {
			if (!memcmp(pm->pmPartType,"Apple_HFS", 9) &&
			    (HFS_SB(sb)->part < 0 || HFS_SB(sb)->part == i)) {
				*part_start += be32_to_cpu(pm->pmPyPartStart);
				*part_size = be32_to_cpu(pm->pmPartBlkCnt);
				res = 0;
				break;
			}
			brelse(bh);
			bh = sb_bread512(sb, *part_start + HFS_PMAP_BLK + ++i, pm);
			if (!bh)
				return -EIO;
			if (pm->pmSig != cpu_to_be16(HFS_NEW_PMAP_MAGIC))
				break;
		}
		break;
	  }
	}
	brelse(bh);

	return res;
}
