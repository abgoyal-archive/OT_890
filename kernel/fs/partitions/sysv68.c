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
 *  fs/partitions/sysv68.c
 *
 *  Copyright (C) 2007 Philippe De Muyter <phdm@macqel.be>
 */

#include "check.h"
#include "sysv68.h"

/*
 *	Volume ID structure: on first 256-bytes sector of disk
 */

struct volumeid {
	u8	vid_unused[248];
	u8	vid_mac[8];	/* ASCII string "MOTOROLA" */
};

/*
 *	config block: second 256-bytes sector on disk
 */

struct dkconfig {
	u8	ios_unused0[128];
	__be32	ios_slcblk;	/* Slice table block number */
	__be16	ios_slccnt;	/* Number of entries in slice table */
	u8	ios_unused1[122];
};

/*
 *	combined volumeid and dkconfig block
 */

struct dkblk0 {
	struct volumeid dk_vid;
	struct dkconfig dk_ios;
};

/*
 *	Slice Table Structure
 */

struct slice {
	__be32	nblocks;		/* slice size (in blocks) */
	__be32	blkoff;			/* block offset of slice */
};


int sysv68_partition(struct parsed_partitions *state, struct block_device *bdev)
{
	int i, slices;
	int slot = 1;
	Sector sect;
	unsigned char *data;
	struct dkblk0 *b;
	struct slice *slice;

	data = read_dev_sector(bdev, 0, &sect);
	if (!data)
		return -1;

	b = (struct dkblk0 *)data;
	if (memcmp(b->dk_vid.vid_mac, "MOTOROLA", sizeof(b->dk_vid.vid_mac))) {
		put_dev_sector(sect);
		return 0;
	}
	slices = be16_to_cpu(b->dk_ios.ios_slccnt);
	i = be32_to_cpu(b->dk_ios.ios_slcblk);
	put_dev_sector(sect);

	data = read_dev_sector(bdev, i, &sect);
	if (!data)
		return -1;

	slices -= 1; /* last slice is the whole disk */
	printk("sysV68: %s(s%u)", state->name, slices);
	slice = (struct slice *)data;
	for (i = 0; i < slices; i++, slice++) {
		if (slot == state->limit)
			break;
		if (be32_to_cpu(slice->nblocks)) {
			put_partition(state, slot,
				be32_to_cpu(slice->blkoff),
				be32_to_cpu(slice->nblocks));
			printk("(s%u)", i);
		}
		slot++;
	}
	printk("\n");
	put_dev_sector(sect);
	return 1;
}
