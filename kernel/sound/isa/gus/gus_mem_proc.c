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
 *  Copyright (c) by Jaroslav Kysela <perex@perex.cz>
 *  GUS's memory access via proc filesystem
 *
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
 *
 */

#include <linux/slab.h>
#include <sound/core.h>
#include <sound/gus.h>
#include <sound/info.h>

struct gus_proc_private {
	int rom;		/* data are in ROM */
	unsigned int address;
	unsigned int size;
	struct snd_gus_card * gus;
};

static long snd_gf1_mem_proc_dump(struct snd_info_entry *entry, void *file_private_data,
			          struct file *file, char __user *buf,
			          unsigned long count, unsigned long pos)
{
	long size;
	struct gus_proc_private *priv = entry->private_data;
	struct snd_gus_card *gus = priv->gus;
	int err;

	size = count;
	if (pos + size > priv->size)
		size = (long)priv->size - pos;
	if (size > 0) {
		if ((err = snd_gus_dram_read(gus, buf, pos, size, priv->rom)) < 0)
			return err;
		return size;
	}
	return 0;
}			

static long long snd_gf1_mem_proc_llseek(struct snd_info_entry *entry,
					void *private_file_data,
					struct file *file,
					long long offset,
					int orig)
{
	struct gus_proc_private *priv = entry->private_data;

	switch (orig) {
	case SEEK_SET:
		file->f_pos = offset;
		break;
	case SEEK_CUR:
		file->f_pos += offset;
		break;
	case SEEK_END: /* offset is negative */
		file->f_pos = priv->size + offset;
		break;
	default:
		return -EINVAL;
	}
	if (file->f_pos > priv->size)
		file->f_pos = priv->size;
	return file->f_pos;
}

static void snd_gf1_mem_proc_free(struct snd_info_entry *entry)
{
	struct gus_proc_private *priv = entry->private_data;
	kfree(priv);
}

static struct snd_info_entry_ops snd_gf1_mem_proc_ops = {
	.read = snd_gf1_mem_proc_dump,
	.llseek = snd_gf1_mem_proc_llseek,
};

int snd_gf1_mem_proc_init(struct snd_gus_card * gus)
{
	int idx;
	char name[16];
	struct gus_proc_private *priv;
	struct snd_info_entry *entry;

	for (idx = 0; idx < 4; idx++) {
		if (gus->gf1.mem_alloc.banks_8[idx].size > 0) {
			priv = kzalloc(sizeof(*priv), GFP_KERNEL);
			if (priv == NULL)
				return -ENOMEM;
			priv->gus = gus;
			sprintf(name, "gus-ram-%i", idx);
			if (! snd_card_proc_new(gus->card, name, &entry)) {
				entry->content = SNDRV_INFO_CONTENT_DATA;
				entry->private_data = priv;
				entry->private_free = snd_gf1_mem_proc_free;
				entry->c.ops = &snd_gf1_mem_proc_ops;
				priv->address = gus->gf1.mem_alloc.banks_8[idx].address;
				priv->size = entry->size = gus->gf1.mem_alloc.banks_8[idx].size;
			}
		}
	}
	for (idx = 0; idx < 4; idx++) {
		if (gus->gf1.rom_present & (1 << idx)) {
			priv = kzalloc(sizeof(*priv), GFP_KERNEL);
			if (priv == NULL)
				return -ENOMEM;
			priv->rom = 1;
			priv->gus = gus;
			sprintf(name, "gus-rom-%i", idx);
			if (! snd_card_proc_new(gus->card, name, &entry)) {
				entry->content = SNDRV_INFO_CONTENT_DATA;
				entry->private_data = priv;
				entry->private_free = snd_gf1_mem_proc_free;
				entry->c.ops = &snd_gf1_mem_proc_ops;
				priv->address = idx * 4096 * 1024;
				priv->size = entry->size = gus->gf1.rom_memory;
			}
		}
	}
	return 0;
}
