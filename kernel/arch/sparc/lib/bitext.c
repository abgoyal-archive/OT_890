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
 * bitext.c: kernel little helper (of bit shuffling variety).
 *
 * Copyright (C) 2002 Pete Zaitcev <zaitcev@yahoo.com>
 *
 * The algorithm to search a zero bit string is geared towards its application.
 * We expect a couple of fixed sizes of requests, so a rotating counter, reset
 * by align size, should provide fast enough search while maintaining low
 * fragmentation.
 */

#include <linux/string.h>
#include <linux/bitops.h>

#include <asm/bitext.h>

/**
 * bit_map_string_get - find and set a bit string in bit map.
 * @t: the bit map.
 * @len: requested string length
 * @align: requested alignment
 *
 * Returns offset in the map or -1 if out of space.
 *
 * Not safe to call from an interrupt (uses spin_lock).
 */
int bit_map_string_get(struct bit_map *t, int len, int align)
{
	int offset, count;	/* siamese twins */
	int off_new;
	int align1;
	int i, color;

	if (t->num_colors) {
		/* align is overloaded to be the page color */
		color = align;
		align = t->num_colors;
	} else {
		color = 0;
		if (align == 0)
			align = 1;
	}
	align1 = align - 1;
	if ((align & align1) != 0)
		BUG();
	if (align < 0 || align >= t->size)
		BUG();
	if (len <= 0 || len > t->size)
		BUG();
	color &= align1;

	spin_lock(&t->lock);
	if (len < t->last_size)
		offset = t->first_free;
	else
		offset = t->last_off & ~align1;
	count = 0;
	for (;;) {
		off_new = find_next_zero_bit(t->map, t->size, offset);
		off_new = ((off_new + align1) & ~align1) + color;
		count += off_new - offset;
		offset = off_new;
		if (offset >= t->size)
			offset = 0;
		if (count + len > t->size) {
			spin_unlock(&t->lock);
/* P3 */ printk(KERN_ERR
  "bitmap out: size %d used %d off %d len %d align %d count %d\n",
  t->size, t->used, offset, len, align, count);
			return -1;
		}

		if (offset + len > t->size) {
			count += t->size - offset;
			offset = 0;
			continue;
		}

		i = 0;
		while (test_bit(offset + i, t->map) == 0) {
			i++;
			if (i == len) {
				for (i = 0; i < len; i++)
					__set_bit(offset + i, t->map);
				if (offset == t->first_free)
					t->first_free = find_next_zero_bit
							(t->map, t->size,
							 t->first_free + len);
				if ((t->last_off = offset + len) >= t->size)
					t->last_off = 0;
				t->used += len;
				t->last_size = len;
				spin_unlock(&t->lock);
				return offset;
			}
		}
		count += i + 1;
		if ((offset += i + 1) >= t->size)
			offset = 0;
	}
}

void bit_map_clear(struct bit_map *t, int offset, int len)
{
	int i;

	if (t->used < len)
		BUG();		/* Much too late to do any good, but alas... */
	spin_lock(&t->lock);
	for (i = 0; i < len; i++) {
		if (test_bit(offset + i, t->map) == 0)
			BUG();
		__clear_bit(offset + i, t->map);
	}
	if (offset < t->first_free)
		t->first_free = offset;
	t->used -= len;
	spin_unlock(&t->lock);
}

void bit_map_init(struct bit_map *t, unsigned long *map, int size)
{

	if ((size & 07) != 0)
		BUG();
	memset(map, 0, size>>3);

	memset(t, 0, sizeof *t);
	spin_lock_init(&t->lock);
	t->map = map;
	t->size = size;
}
