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
 *  fs/partitions/osf.c
 *
 *  Code extracted from drivers/block/genhd.c
 *
 *  Copyright (C) 1991-1998  Linus Torvalds
 *  Re-organised Feb 1998 Russell King
 */

#include "check.h"
#include "osf.h"

int osf_partition(struct parsed_partitions *state, struct block_device *bdev)
{
	int i;
	int slot = 1;
	Sector sect;
	unsigned char *data;
	struct disklabel {
		__le32 d_magic;
		__le16 d_type,d_subtype;
		u8 d_typename[16];
		u8 d_packname[16];
		__le32 d_secsize;
		__le32 d_nsectors;
		__le32 d_ntracks;
		__le32 d_ncylinders;
		__le32 d_secpercyl;
		__le32 d_secprtunit;
		__le16 d_sparespertrack;
		__le16 d_sparespercyl;
		__le32 d_acylinders;
		__le16 d_rpm, d_interleave, d_trackskew, d_cylskew;
		__le32 d_headswitch, d_trkseek, d_flags;
		__le32 d_drivedata[5];
		__le32 d_spare[5];
		__le32 d_magic2;
		__le16 d_checksum;
		__le16 d_npartitions;
		__le32 d_bbsize, d_sbsize;
		struct d_partition {
			__le32 p_size;
			__le32 p_offset;
			__le32 p_fsize;
			u8  p_fstype;
			u8  p_frag;
			__le16 p_cpg;
		} d_partitions[8];
	} * label;
	struct d_partition * partition;

	data = read_dev_sector(bdev, 0, &sect);
	if (!data)
		return -1;

	label = (struct disklabel *) (data+64);
	partition = label->d_partitions;
	if (le32_to_cpu(label->d_magic) != DISKLABELMAGIC) {
		put_dev_sector(sect);
		return 0;
	}
	if (le32_to_cpu(label->d_magic2) != DISKLABELMAGIC) {
		put_dev_sector(sect);
		return 0;
	}
	for (i = 0 ; i < le16_to_cpu(label->d_npartitions); i++, partition++) {
		if (slot == state->limit)
		        break;
		if (le32_to_cpu(partition->p_size))
			put_partition(state, slot,
				le32_to_cpu(partition->p_offset),
				le32_to_cpu(partition->p_size));
		slot++;
	}
	printk("\n");
	put_dev_sector(sect);
	return 1;
}
