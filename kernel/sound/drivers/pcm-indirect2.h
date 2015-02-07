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
 * Helper functions for indirect PCM data transfer to a simple FIFO in
 * hardware (small, no possibility to read "hardware io position",
 * updating position done by interrupt, ...)
 *
 *  Copyright (c) by 2007  Joachim Foerster <JOFT@gmx.de>
 *
 *  Based on "pcm-indirect.h" (alsa-driver-1.0.13) by
 *
 *  Copyright (c) by Takashi Iwai <tiwai@suse.de>
 *                   Jaroslav Kysela <perex@suse.cz>
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
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef __SOUND_PCM_INDIRECT2_H
#define __SOUND_PCM_INDIRECT2_H

/* struct snd_pcm_substream, struct snd_pcm_runtime, snd_pcm_uframes_t */
#include <sound/pcm.h>

/* Debug options for code which may be removed completely in a final version */
#ifdef CONFIG_SND_DEBUG
#define SND_PCM_INDIRECT2_STAT    /* turn on some "statistics" about the
				   * process of copying bytes from the
				   * intermediate buffer to the hardware
				   * fifo and the other way round
				   */
#endif

struct snd_pcm_indirect2 {
	unsigned int hw_buffer_size;  /* Byte size of hardware buffer */
	int hw_ready;		      /* playback: 1 = hw fifo has room left,
				       * 0 = hw fifo is full
				       */
	unsigned int min_multiple;
	int min_periods;	      /* counts number of min. periods until
				       * min_multiple is reached
				       */
	int min_period_count;	      /* counts bytes to count number of
				       * min. periods
				       */

	unsigned int sw_buffer_size;  /* Byte size of software buffer */

	/* sw_data: position in intermediate buffer, where we will read (or
	 *          write) from/to next time (to transfer data to/from HW)
	 */
	unsigned int sw_data;         /* Offset to next dst (or src) in sw
				       * ring buffer
				       */
	/* easiest case (playback):
	 * sw_data is nearly the same as ~ runtime->control->appl_ptr, with the
	 * exception that sw_data is "behind" by the number if bytes ALSA wrote
	 * to the intermediate buffer last time.
	 * A call to ack() callback synchronizes both indirectly.
	 */

	/* We have no real sw_io pointer here. Usually sw_io is pointing to the
	 * current playback/capture position _inside_ the hardware. Devices
	 * with plain FIFOs often have no possibility to publish this position.
	 * So we say: if sw_data is updated, that means bytes were copied to
	 * the hardware, we increase sw_io by that amount, because there have
	 * to be as much bytes which were played. So sw_io will stay behind
	 * sw_data all the time and has to converge to sw_data at the end of
	 * playback.
	 */
	unsigned int sw_io;           /* Current software pointer in bytes */

	/* sw_ready: number of bytes ALSA copied to the intermediate buffer, so
	 * it represents the number of bytes which wait for transfer to the HW
	 */
	int sw_ready;		  /* Bytes ready to be transferred to/from hw */

	/* appl_ptr: last known position of ALSA (where ALSA is going to write
	 * next time into the intermediate buffer
	 */
	snd_pcm_uframes_t appl_ptr;   /* Last seen appl_ptr */

	unsigned int bytes2hw;
	int check_alignment;

#ifdef SND_PCM_INDIRECT2_STAT
	unsigned int zeros2hw;
	unsigned int mul_elapsed;
	unsigned int mul_elapsed_real;
	unsigned long firstbytetime;
	unsigned long lastbytetime;
	unsigned long firstzerotime;
	unsigned int byte_sizes[64];
	unsigned int zero_sizes[64];
	unsigned int min_adds[8];
	unsigned int mul_adds[8];
	unsigned int zero_times[3750];	/* = 15s */
	unsigned int zero_times_saved;
	unsigned int zero_times_notsaved;
	unsigned int irq_occured;
	unsigned int pointer_calls;
	unsigned int lastdifftime;
#endif
};

typedef size_t (*snd_pcm_indirect2_copy_t) (struct snd_pcm_substream *substream,
					   struct snd_pcm_indirect2 *rec,
					   size_t bytes);
typedef size_t (*snd_pcm_indirect2_zero_t) (struct snd_pcm_substream *substream,
					   struct snd_pcm_indirect2 *rec);

#ifdef SND_PCM_INDIRECT2_STAT
void snd_pcm_indirect2_stat(struct snd_pcm_substream *substream,
				   struct snd_pcm_indirect2 *rec);
#endif

snd_pcm_uframes_t
snd_pcm_indirect2_pointer(struct snd_pcm_substream *substream,
			  struct snd_pcm_indirect2 *rec);
void
snd_pcm_indirect2_playback_interrupt(struct snd_pcm_substream *substream,
				     struct snd_pcm_indirect2 *rec,
				     snd_pcm_indirect2_copy_t copy,
				     snd_pcm_indirect2_zero_t zero);
void
snd_pcm_indirect2_capture_interrupt(struct snd_pcm_substream *substream,
				    struct snd_pcm_indirect2 *rec,
				    snd_pcm_indirect2_copy_t copy,
				    snd_pcm_indirect2_zero_t null);

#endif /* __SOUND_PCM_INDIRECT2_H */
