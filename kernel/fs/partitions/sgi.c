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
 *  fs/partitions/sgi.c
 *
 *  Code extracted from drivers/block/genhd.c
 */

#include "check.h"
#include "sgi.h"

struct sgi_disklabel {
	__be32 magic_mushroom;		/* Big fat spliff... */
	__be16 root_part_num;		/* Root partition number */
	__be16 swap_part_num;		/* Swap partition number */
	s8 boot_file[16];		/* Name of boot file for ARCS */
	u8 _unused0[48];		/* Device parameter useless crapola.. */
	struct sgi_volume {
		s8 name[8];		/* Name of volume */
		__be32 block_num;		/* Logical block number */
		__be32 num_bytes;		/* How big, in bytes */
	} volume[15];
	struct sgi_partition {
		__be32 num_blocks;		/* Size in logical blocks */
		__be32 first_block;	/* First logical block */
		__be32 type;		/* Type of this partition */
	} partitions[16];
	__be32 csum;			/* Disk label checksum */
	__be32 _unused1;			/* Padding */
};

int sgi_partition(struct parsed_partitions *state, struct block_device *bdev)
{
	int i, csum;
	__be32 magic;
	int slot = 1;
	unsigned int start, blocks;
	__be32 *ui, cs;
	Sector sect;
	struct sgi_disklabel *label;
	struct sgi_partition *p;
	char b[BDEVNAME_SIZE];

	label = (struct sgi_disklabel *) read_dev_sector(bdev, 0, &sect);
	if (!label)
		return -1;
	p = &label->partitions[0];
	magic = label->magic_mushroom;
	if(be32_to_cpu(magic) != SGI_LABEL_MAGIC) {
		/*printk("Dev %s SGI disklabel: bad magic %08x\n",
		       bdevname(bdev, b), be32_to_cpu(magic));*/
		put_dev_sector(sect);
		return 0;
	}
	ui = ((__be32 *) (label + 1)) - 1;
	for(csum = 0; ui >= ((__be32 *) label);) {
		cs = *ui--;
		csum += be32_to_cpu(cs);
	}
	if(csum) {
		printk(KERN_WARNING "Dev %s SGI disklabel: csum bad, label corrupted\n",
		       bdevname(bdev, b));
		put_dev_sector(sect);
		return 0;
	}
	/* All SGI disk labels have 16 partitions, disks under Linux only
	 * have 15 minor's.  Luckily there are always a few zero length
	 * partitions which we don't care about so we never overflow the
	 * current_minor.
	 */
	for(i = 0; i < 16; i++, p++) {
		blocks = be32_to_cpu(p->num_blocks);
		start  = be32_to_cpu(p->first_block);
		if (blocks) {
			put_partition(state, slot, start, blocks);
			if (be32_to_cpu(p->type) == LINUX_RAID_PARTITION)
				state->parts[slot].flags = ADDPART_FLAG_RAID;
		}
		slot++;
	}
	printk("\n");
	put_dev_sector(sect);
	return 1;
}
