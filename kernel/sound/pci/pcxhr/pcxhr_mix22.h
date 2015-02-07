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
 * Driver for Digigram pcxhr compatible soundcards
 *
 * low level interface with interrupt ans message handling
 *
 * Copyright (c) 2004 by Digigram <alsa@digigram.com>
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

#ifndef __SOUND_PCXHR_MIX22_H
#define __SOUND_PCXHR_MIX22_H

struct pcxhr_mgr;

int hr222_sub_init(struct pcxhr_mgr *mgr);
int hr222_sub_set_clock(struct pcxhr_mgr *mgr, unsigned int rate,
			int *changed);
int hr222_get_external_clock(struct pcxhr_mgr *mgr,
			     enum pcxhr_clock_type clock_type,
			     int *sample_rate);

#define HR222_LINE_PLAYBACK_LEVEL_MIN		0	/* -25.5 dB */
#define HR222_LINE_PLAYBACK_ZERO_LEVEL		51	/* 0.0 dB */
#define HR222_LINE_PLAYBACK_LEVEL_MAX		99	/* +24.0 dB */

#define HR222_LINE_CAPTURE_LEVEL_MIN		0	/* -111.5 dB */
#define HR222_LINE_CAPTURE_ZERO_LEVEL		223	/* 0.0 dB */
#define HR222_LINE_CAPTURE_LEVEL_MAX		255	/* +16 dB */
#define HR222_MICRO_CAPTURE_LEVEL_MIN		0	/* -98.5 dB */
#define HR222_MICRO_CAPTURE_LEVEL_MAX		210	/* +6.5 dB */

int hr222_update_analog_audio_level(struct snd_pcxhr *chip,
				    int is_capture,
				    int channel);
int hr222_set_audio_source(struct snd_pcxhr *chip);
int hr222_iec958_capture_byte(struct snd_pcxhr *chip, int aes_idx,
			      unsigned char *aes_bits);
int hr222_iec958_update_byte(struct snd_pcxhr *chip, int aes_idx,
			     unsigned char aes_bits);

int hr222_add_mic_controls(struct snd_pcxhr *chip);

#endif /* __SOUND_PCXHR_MIX22_H */
