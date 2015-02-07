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

#ifndef __SOUND_MIXER_OSS_H
#define __SOUND_MIXER_OSS_H

/*
 *  OSS MIXER API
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

#if defined(CONFIG_SND_MIXER_OSS) || defined(CONFIG_SND_MIXER_OSS_MODULE)

#define SNDRV_OSS_MAX_MIXERS	32

struct snd_mixer_oss_file;

struct snd_mixer_oss_slot {
	int number;
	unsigned int stereo: 1;
	int (*get_volume)(struct snd_mixer_oss_file *fmixer,
			  struct snd_mixer_oss_slot *chn,
			  int *left, int *right);
	int (*put_volume)(struct snd_mixer_oss_file *fmixer,
			  struct snd_mixer_oss_slot *chn,
			  int left, int right);
	int (*get_recsrc)(struct snd_mixer_oss_file *fmixer,
			  struct snd_mixer_oss_slot *chn,
			  int *active);
	int (*put_recsrc)(struct snd_mixer_oss_file *fmixer,
			  struct snd_mixer_oss_slot *chn,
			  int active);
	unsigned long private_value;
	void *private_data;
	void (*private_free)(struct snd_mixer_oss_slot *slot);
	int volume[2];
};

struct snd_mixer_oss {
	struct snd_card *card;
	char id[16];
	char name[32];
	struct snd_mixer_oss_slot slots[SNDRV_OSS_MAX_MIXERS]; /* OSS mixer slots */
	unsigned int mask_recsrc;		/* exclusive recsrc mask */
	int (*get_recsrc)(struct snd_mixer_oss_file *fmixer,
			  unsigned int *active_index);
	int (*put_recsrc)(struct snd_mixer_oss_file *fmixer,
			  unsigned int active_index);
	void *private_data_recsrc;
	void (*private_free_recsrc)(struct snd_mixer_oss *mixer);
	struct mutex reg_mutex;
	struct snd_info_entry *proc_entry;
	int oss_dev_alloc;
	/* --- */
	int oss_recsrc;
};

struct snd_mixer_oss_file {
	struct snd_card *card;
	struct snd_mixer_oss *mixer;
};

#endif /* CONFIG_SND_MIXER_OSS */

#endif /* __SOUND_MIXER_OSS_H */
