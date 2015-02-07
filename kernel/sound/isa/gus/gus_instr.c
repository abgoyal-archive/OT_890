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
 *  Routines for Gravis UltraSound soundcards - Synthesizer
 *  Copyright (c) by Jaroslav Kysela <perex@perex.cz>
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

#include <linux/time.h>
#include <sound/core.h>
#include <sound/gus.h>

/*
 *
 */

int snd_gus_iwffff_put_sample(void *private_data, struct iwffff_wave *wave,
			      char __user *data, long len, int atomic)
{
	struct snd_gus_card *gus = private_data;
	struct snd_gf1_mem_block *block;
	int err;

	if (wave->format & IWFFFF_WAVE_ROM)
		return 0;	/* it's probably ok - verify the address? */
	if (wave->format & IWFFFF_WAVE_STEREO)
		return -EINVAL;	/* not supported */
	block = snd_gf1_mem_alloc(&gus->gf1.mem_alloc,
				  SNDRV_GF1_MEM_OWNER_WAVE_IWFFFF,
				  NULL, wave->size,
				  wave->format & IWFFFF_WAVE_16BIT, 1,
				  wave->share_id);
	if (block == NULL)
		return -ENOMEM;
	err = snd_gus_dram_write(gus, data,
				 block->ptr, wave->size);
	if (err < 0) {
		snd_gf1_mem_lock(&gus->gf1.mem_alloc, 0);
		snd_gf1_mem_xfree(&gus->gf1.mem_alloc, block);
		snd_gf1_mem_lock(&gus->gf1.mem_alloc, 1);
		return err;
	}
	wave->address.memory = block->ptr;
	return 0;
}

int snd_gus_iwffff_get_sample(void *private_data, struct iwffff_wave *wave,
			      char __user *data, long len, int atomic)
{
	struct snd_gus_card *gus = private_data;

	return snd_gus_dram_read(gus, data, wave->address.memory, wave->size,
				 wave->format & IWFFFF_WAVE_ROM ? 1 : 0);
}

int snd_gus_iwffff_remove_sample(void *private_data, struct iwffff_wave *wave,
				 int atomic)
{
	struct snd_gus_card *gus = private_data;

	if (wave->format & IWFFFF_WAVE_ROM)
		return 0;	/* it's probably ok - verify the address? */	
	return snd_gf1_mem_free(&gus->gf1.mem_alloc, wave->address.memory);
}

/*
 *
 */

int snd_gus_gf1_put_sample(void *private_data, struct gf1_wave *wave,
			   char __user *data, long len, int atomic)
{
	struct snd_gus_card *gus = private_data;
	struct snd_gf1_mem_block *block;
	int err;

	if (wave->format & GF1_WAVE_STEREO)
		return -EINVAL;	/* not supported */
	block = snd_gf1_mem_alloc(&gus->gf1.mem_alloc,
				  SNDRV_GF1_MEM_OWNER_WAVE_GF1,
				  NULL, wave->size,
				  wave->format & GF1_WAVE_16BIT, 1,
				  wave->share_id);
	if (block == NULL)
		return -ENOMEM;
	err = snd_gus_dram_write(gus, data,
				 block->ptr, wave->size);
	if (err < 0) {
		snd_gf1_mem_lock(&gus->gf1.mem_alloc, 0);
		snd_gf1_mem_xfree(&gus->gf1.mem_alloc, block);
		snd_gf1_mem_lock(&gus->gf1.mem_alloc, 1);
		return err;
	}
	wave->address.memory = block->ptr;
	return 0;
}

int snd_gus_gf1_get_sample(void *private_data, struct gf1_wave *wave,
			   char __user *data, long len, int atomic)
{
	struct snd_gus_card *gus = private_data;

	return snd_gus_dram_read(gus, data, wave->address.memory, wave->size, 0);
}

int snd_gus_gf1_remove_sample(void *private_data, struct gf1_wave *wave,
			      int atomic)
{
	struct snd_gus_card *gus = private_data;

	return snd_gf1_mem_free(&gus->gf1.mem_alloc, wave->address.memory);
}

/*
 *
 */

int snd_gus_simple_put_sample(void *private_data, struct simple_instrument *instr,
			      char __user *data, long len, int atomic)
{
	struct snd_gus_card *gus = private_data;
	struct snd_gf1_mem_block *block;
	int err;

	if (instr->format & SIMPLE_WAVE_STEREO)
		return -EINVAL;	/* not supported */
	block = snd_gf1_mem_alloc(&gus->gf1.mem_alloc,
				  SNDRV_GF1_MEM_OWNER_WAVE_SIMPLE,
				  NULL, instr->size,
				  instr->format & SIMPLE_WAVE_16BIT, 1,
				  instr->share_id);
	if (block == NULL)
		return -ENOMEM;
	err = snd_gus_dram_write(gus, data, block->ptr, instr->size);
	if (err < 0) {
		snd_gf1_mem_lock(&gus->gf1.mem_alloc, 0);
		snd_gf1_mem_xfree(&gus->gf1.mem_alloc, block);
		snd_gf1_mem_lock(&gus->gf1.mem_alloc, 1);
		return err;
	}
	instr->address.memory = block->ptr;
	return 0;
}

int snd_gus_simple_get_sample(void *private_data, struct simple_instrument *instr,
			      char __user *data, long len, int atomic)
{
	struct snd_gus_card *gus = private_data;

	return snd_gus_dram_read(gus, data, instr->address.memory, instr->size, 0);
}

int snd_gus_simple_remove_sample(void *private_data, struct simple_instrument *instr,
			         int atomic)
{
	struct snd_gus_card *gus = private_data;

	return snd_gf1_mem_free(&gus->gf1.mem_alloc, instr->address.memory);
}
