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
 *  Copyright (C) 2000 Takashi Iwai <tiwai@suse.de>
 *
 *  Generic memory management routines for soundcard memory allocation
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 */

#include <linux/mutex.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <sound/core.h>
#include <sound/util_mem.h>

MODULE_AUTHOR("Takashi Iwai");
MODULE_DESCRIPTION("Generic memory management routines for soundcard memory allocation");
MODULE_LICENSE("GPL");

#define get_memblk(p)	list_entry(p, struct snd_util_memblk, list)

/*
 * create a new memory manager
 */
struct snd_util_memhdr *
snd_util_memhdr_new(int memsize)
{
	struct snd_util_memhdr *hdr;

	hdr = kzalloc(sizeof(*hdr), GFP_KERNEL);
	if (hdr == NULL)
		return NULL;
	hdr->size = memsize;
	mutex_init(&hdr->block_mutex);
	INIT_LIST_HEAD(&hdr->block);

	return hdr;
}

/*
 * free a memory manager
 */
void snd_util_memhdr_free(struct snd_util_memhdr *hdr)
{
	struct list_head *p;

	if (!hdr)
		return;
	/* release all blocks */
	while ((p = hdr->block.next) != &hdr->block) {
		list_del(p);
		kfree(get_memblk(p));
	}
	kfree(hdr);
}

/*
 * allocate a memory block (without mutex)
 */
struct snd_util_memblk *
__snd_util_mem_alloc(struct snd_util_memhdr *hdr, int size)
{
	struct snd_util_memblk *blk;
	unsigned int units, prev_offset;
	struct list_head *p;

	if (snd_BUG_ON(!hdr || size <= 0))
		return NULL;

	/* word alignment */
	units = size;
	if (units & 1)
		units++;
	if (units > hdr->size)
		return NULL;

	/* look for empty block */
	prev_offset = 0;
	list_for_each(p, &hdr->block) {
		blk = get_memblk(p);
		if (blk->offset - prev_offset >= units)
			goto __found;
		prev_offset = blk->offset + blk->size;
	}
	if (hdr->size - prev_offset < units)
		return NULL;

__found:
	return __snd_util_memblk_new(hdr, units, p->prev);
}


/*
 * create a new memory block with the given size
 * the block is linked next to prev
 */
struct snd_util_memblk *
__snd_util_memblk_new(struct snd_util_memhdr *hdr, unsigned int units,
		      struct list_head *prev)
{
	struct snd_util_memblk *blk;

	blk = kmalloc(sizeof(struct snd_util_memblk) + hdr->block_extra_size,
		      GFP_KERNEL);
	if (blk == NULL)
		return NULL;

	if (prev == &hdr->block)
		blk->offset = 0;
	else {
		struct snd_util_memblk *p = get_memblk(prev);
		blk->offset = p->offset + p->size;
	}
	blk->size = units;
	list_add(&blk->list, prev);
	hdr->nblocks++;
	hdr->used += units;
	return blk;
}


/*
 * allocate a memory block (with mutex)
 */
struct snd_util_memblk *
snd_util_mem_alloc(struct snd_util_memhdr *hdr, int size)
{
	struct snd_util_memblk *blk;
	mutex_lock(&hdr->block_mutex);
	blk = __snd_util_mem_alloc(hdr, size);
	mutex_unlock(&hdr->block_mutex);
	return blk;
}


/*
 * remove the block from linked-list and free resource
 * (without mutex)
 */
void
__snd_util_mem_free(struct snd_util_memhdr *hdr, struct snd_util_memblk *blk)
{
	list_del(&blk->list);
	hdr->nblocks--;
	hdr->used -= blk->size;
	kfree(blk);
}

/*
 * free a memory block (with mutex)
 */
int snd_util_mem_free(struct snd_util_memhdr *hdr, struct snd_util_memblk *blk)
{
	if (snd_BUG_ON(!hdr || !blk))
		return -EINVAL;

	mutex_lock(&hdr->block_mutex);
	__snd_util_mem_free(hdr, blk);
	mutex_unlock(&hdr->block_mutex);
	return 0;
}

/*
 * return available memory size
 */
int snd_util_mem_avail(struct snd_util_memhdr *hdr)
{
	unsigned int size;
	mutex_lock(&hdr->block_mutex);
	size = hdr->size - hdr->used;
	mutex_unlock(&hdr->block_mutex);
	return size;
}


EXPORT_SYMBOL(snd_util_memhdr_new);
EXPORT_SYMBOL(snd_util_memhdr_free);
EXPORT_SYMBOL(snd_util_mem_alloc);
EXPORT_SYMBOL(snd_util_mem_free);
EXPORT_SYMBOL(snd_util_mem_avail);
EXPORT_SYMBOL(__snd_util_mem_alloc);
EXPORT_SYMBOL(__snd_util_mem_free);
EXPORT_SYMBOL(__snd_util_memblk_new);

/*
 *  INIT part
 */

static int __init alsa_util_mem_init(void)
{
	return 0;
}

static void __exit alsa_util_mem_exit(void)
{
}

module_init(alsa_util_mem_init)
module_exit(alsa_util_mem_exit)
