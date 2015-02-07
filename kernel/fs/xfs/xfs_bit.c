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
 * Copyright (c) 2000-2005 Silicon Graphics, Inc.
 * All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it would be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write the Free Software Foundation,
 * Inc.,  51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
#include "xfs.h"
#include "xfs_bit.h"
#include "xfs_log.h"
#include "xfs_trans.h"
#include "xfs_buf_item.h"

/*
 * XFS bit manipulation routines, used in non-realtime code.
 */

/*
 * Return whether bitmap is empty.
 * Size is number of words in the bitmap, which is padded to word boundary
 * Returns 1 for empty, 0 for non-empty.
 */
int
xfs_bitmap_empty(uint *map, uint size)
{
	uint i;
	uint ret = 0;

	for (i = 0; i < size; i++) {
		ret |= map[i];
	}

	return (ret == 0);
}

/*
 * Count the number of contiguous bits set in the bitmap starting with bit
 * start_bit.  Size is the size of the bitmap in words.
 */
int
xfs_contig_bits(uint *map, uint	size, uint start_bit)
{
	uint * p = ((unsigned int *) map) + (start_bit >> BIT_TO_WORD_SHIFT);
	uint result = 0;
	uint tmp;

	size <<= BIT_TO_WORD_SHIFT;

	ASSERT(start_bit < size);
	size -= start_bit & ~(NBWORD - 1);
	start_bit &= (NBWORD - 1);
	if (start_bit) {
		tmp = *p++;
		/* set to one first offset bits prior to start */
		tmp |= (~0U >> (NBWORD-start_bit));
		if (tmp != ~0U)
			goto found;
		result += NBWORD;
		size -= NBWORD;
	}
	while (size) {
		if ((tmp = *p++) != ~0U)
			goto found;
		result += NBWORD;
		size -= NBWORD;
	}
	return result - start_bit;
found:
	return result + ffz(tmp) - start_bit;
}

/*
 * This takes the bit number to start looking from and
 * returns the next set bit from there.  It returns -1
 * if there are no more bits set or the start bit is
 * beyond the end of the bitmap.
 *
 * Size is the number of words, not bytes, in the bitmap.
 */
int xfs_next_bit(uint *map, uint size, uint start_bit)
{
	uint * p = ((unsigned int *) map) + (start_bit >> BIT_TO_WORD_SHIFT);
	uint result = start_bit & ~(NBWORD - 1);
	uint tmp;

	size <<= BIT_TO_WORD_SHIFT;

	if (start_bit >= size)
		return -1;
	size -= result;
	start_bit &= (NBWORD - 1);
	if (start_bit) {
		tmp = *p++;
		/* set to zero first offset bits prior to start */
		tmp &= (~0U << start_bit);
		if (tmp != 0U)
			goto found;
		result += NBWORD;
		size -= NBWORD;
	}
	while (size) {
		if ((tmp = *p++) != 0U)
			goto found;
		result += NBWORD;
		size -= NBWORD;
	}
	return -1;
found:
	return result + ffs(tmp) - 1;
}
