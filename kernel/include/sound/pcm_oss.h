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

#ifndef __SOUND_PCM_OSS_H
#define __SOUND_PCM_OSS_H

/*
 *  Digital Audio (PCM) - OSS compatibility abstract layer
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

struct snd_pcm_oss_setup {
	char *task_name;
	unsigned int disable:1,
		     direct:1,
		     block:1,
		     nonblock:1,
		     partialfrag:1,
		     nosilence:1,
		     buggyptr:1;
	unsigned int periods;
	unsigned int period_size;
	struct snd_pcm_oss_setup *next;
};

struct snd_pcm_oss_runtime {
	unsigned params: 1,			/* format/parameter change */
		 prepare: 1,			/* need to prepare the operation */
		 trigger: 1,			/* trigger flag */
		 sync_trigger: 1;		/* sync trigger flag */
	int rate;				/* requested rate */
	int format;				/* requested OSS format */
	unsigned int channels;			/* requested channels */
	unsigned int fragshift;
	unsigned int maxfrags;
	unsigned int subdivision;		/* requested subdivision */
	size_t period_bytes;			/* requested period size */
	size_t period_frames;			/* period frames for poll */
	size_t period_ptr;			/* actual write pointer to period */
	unsigned int periods;
	size_t buffer_bytes;			/* requested buffer size */
	size_t bytes;				/* total # bytes processed */
	size_t mmap_bytes;
	char *buffer;				/* vmallocated period */
	size_t buffer_used;			/* used length from period buffer */
	struct mutex params_lock;
#ifdef CONFIG_SND_PCM_OSS_PLUGINS
	struct snd_pcm_plugin *plugin_first;
	struct snd_pcm_plugin *plugin_last;
#endif
	unsigned int prev_hw_ptr_interrupt;
};

struct snd_pcm_oss_file {
	struct snd_pcm_substream *streams[2];
};

struct snd_pcm_oss_substream {
	unsigned oss: 1;			/* oss mode */
	struct snd_pcm_oss_setup setup;		/* active setup */
};

struct snd_pcm_oss_stream {
	struct snd_pcm_oss_setup *setup_list;	/* setup list */
	struct mutex setup_mutex;
#ifdef CONFIG_SND_VERBOSE_PROCFS
	struct snd_info_entry *proc_entry;
#endif
};

struct snd_pcm_oss {
	int reg;
	unsigned int reg_mask;
};

#endif /* __SOUND_PCM_OSS_H */
