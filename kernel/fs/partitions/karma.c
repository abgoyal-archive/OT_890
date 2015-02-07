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
 *  fs/partitions/karma.c
 *  Rio Karma partition info.
 *
 *  Copyright (C) 2006 Bob Copeland (me@bobcopeland.com)
 *  based on osf.c
 */

#include "check.h"
#include "karma.h"

int karma_partition(struct parsed_partitions *state, struct block_device *bdev)
{
	int i;
	int slot = 1;
	Sector sect;
	unsigned char *data;
	struct disklabel {
		u8 d_reserved[270];
		struct d_partition {
			__le32 p_res;
			u8 p_fstype;
			u8 p_res2[3];
			__le32 p_offset;
			__le32 p_size;
		} d_partitions[2];
		u8 d_blank[208];
		__le16 d_magic;
	} __attribute__((packed)) *label;
	struct d_partition *p;

	data = read_dev_sector(bdev, 0, &sect);
	if (!data)
		return -1;

	label = (struct disklabel *)data;
	if (le16_to_cpu(label->d_magic) != KARMA_LABEL_MAGIC) {
		put_dev_sector(sect);
		return 0;
	}

	p = label->d_partitions;
	for (i = 0 ; i < 2; i++, p++) {
		if (slot == state->limit)
			break;

		if (p->p_fstype == 0x4d && le32_to_cpu(p->p_size)) {
			put_partition(state, slot, le32_to_cpu(p->p_offset),
				le32_to_cpu(p->p_size));
		}
		slot++;
	}
	printk("\n");
	put_dev_sector(sect);
	return 1;
}

