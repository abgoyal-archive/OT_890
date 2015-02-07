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
 *  fs/partitions/amiga.c
 *
 *  Code extracted from drivers/block/genhd.c
 *
 *  Copyright (C) 1991-1998  Linus Torvalds
 *  Re-organised Feb 1998 Russell King
 */

#include <linux/types.h>
#include <linux/affs_hardblocks.h>

#include "check.h"
#include "amiga.h"

static __inline__ u32
checksum_block(__be32 *m, int size)
{
	u32 sum = 0;

	while (size--)
		sum += be32_to_cpu(*m++);
	return sum;
}

int
amiga_partition(struct parsed_partitions *state, struct block_device *bdev)
{
	Sector sect;
	unsigned char *data;
	struct RigidDiskBlock *rdb;
	struct PartitionBlock *pb;
	int start_sect, nr_sects, blk, part, res = 0;
	int blksize = 1;	/* Multiplier for disk block size */
	int slot = 1;
	char b[BDEVNAME_SIZE];

	for (blk = 0; ; blk++, put_dev_sector(sect)) {
		if (blk == RDB_ALLOCATION_LIMIT)
			goto rdb_done;
		data = read_dev_sector(bdev, blk, &sect);
		if (!data) {
			if (warn_no_part)
				printk("Dev %s: unable to read RDB block %d\n",
				       bdevname(bdev, b), blk);
			res = -1;
			goto rdb_done;
		}
		if (*(__be32 *)data != cpu_to_be32(IDNAME_RIGIDDISK))
			continue;

		rdb = (struct RigidDiskBlock *)data;
		if (checksum_block((__be32 *)data, be32_to_cpu(rdb->rdb_SummedLongs) & 0x7F) == 0)
			break;
		/* Try again with 0xdc..0xdf zeroed, Windows might have
		 * trashed it.
		 */
		*(__be32 *)(data+0xdc) = 0;
		if (checksum_block((__be32 *)data,
				be32_to_cpu(rdb->rdb_SummedLongs) & 0x7F)==0) {
			printk("Warning: Trashed word at 0xd0 in block %d "
				"ignored in checksum calculation\n",blk);
			break;
		}

		printk("Dev %s: RDB in block %d has bad checksum\n",
			       bdevname(bdev, b), blk);
	}

	/* blksize is blocks per 512 byte standard block */
	blksize = be32_to_cpu( rdb->rdb_BlockBytes ) / 512;

	printk(" RDSK (%d)", blksize * 512);	/* Be more informative */
	blk = be32_to_cpu(rdb->rdb_PartitionList);
	put_dev_sector(sect);
	for (part = 1; blk>0 && part<=16; part++, put_dev_sector(sect)) {
		blk *= blksize;	/* Read in terms partition table understands */
		data = read_dev_sector(bdev, blk, &sect);
		if (!data) {
			if (warn_no_part)
				printk("Dev %s: unable to read partition block %d\n",
				       bdevname(bdev, b), blk);
			res = -1;
			goto rdb_done;
		}
		pb  = (struct PartitionBlock *)data;
		blk = be32_to_cpu(pb->pb_Next);
		if (pb->pb_ID != cpu_to_be32(IDNAME_PARTITION))
			continue;
		if (checksum_block((__be32 *)pb, be32_to_cpu(pb->pb_SummedLongs) & 0x7F) != 0 )
			continue;

		/* Tell Kernel about it */

		nr_sects = (be32_to_cpu(pb->pb_Environment[10]) + 1 -
			    be32_to_cpu(pb->pb_Environment[9])) *
			   be32_to_cpu(pb->pb_Environment[3]) *
			   be32_to_cpu(pb->pb_Environment[5]) *
			   blksize;
		if (!nr_sects)
			continue;
		start_sect = be32_to_cpu(pb->pb_Environment[9]) *
			     be32_to_cpu(pb->pb_Environment[3]) *
			     be32_to_cpu(pb->pb_Environment[5]) *
			     blksize;
		put_partition(state,slot++,start_sect,nr_sects);
		{
			/* Be even more informative to aid mounting */
			char dostype[4];
			__be32 *dt = (__be32 *)dostype;
			*dt = pb->pb_Environment[16];
			if (dostype[3] < ' ')
				printk(" (%c%c%c^%c)",
					dostype[0], dostype[1],
					dostype[2], dostype[3] + '@' );
			else
				printk(" (%c%c%c%c)",
					dostype[0], dostype[1],
					dostype[2], dostype[3]);
			printk("(res %d spb %d)",
				be32_to_cpu(pb->pb_Environment[6]),
				be32_to_cpu(pb->pb_Environment[4]));
		}
		res = 1;
	}
	printk("\n");

rdb_done:
	return res;
}
