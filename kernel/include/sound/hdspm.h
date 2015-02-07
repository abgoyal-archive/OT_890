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

#ifndef __SOUND_HDSPM_H
#define __SOUND_HDSPM_H
/*
 *   Copyright (C) 2003 Winfried Ritsch (IEM)
 *   based on hdsp.h from Thomas Charbonnel (thomas@undata.org)
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
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/* Maximum channels is 64 even on 56Mode you have 64playbacks to matrix */
#define HDSPM_MAX_CHANNELS      64

/* -------------------- IOCTL Peak/RMS Meters -------------------- */

/* peam rms level structure like we get from hardware 
  
   maybe in future we can memory map it so I just copy it
   to user on ioctl call now an dont change anything
   rms are made out of low and high values
   where (long) ????_rms = (????_rms_l >> 8) + ((????_rms_h & 0xFFFFFF00)<<24)
   (i asume so from the code)
*/

struct hdspm_peak_rms {

	unsigned int level_offset[1024];

	unsigned int input_peak[64];
	unsigned int playback_peak[64];
	unsigned int output_peak[64];
	unsigned int xxx_peak[64];	/* not used */

	unsigned int reserved[256];	/* not used */

	unsigned int input_rms_l[64];
	unsigned int playback_rms_l[64];
	unsigned int output_rms_l[64];
	unsigned int xxx_rms_l[64];	/* not used */

	unsigned int input_rms_h[64];
	unsigned int playback_rms_h[64];
	unsigned int output_rms_h[64];
	unsigned int xxx_rms_h[64];	/* not used */
};

struct hdspm_peak_rms_ioctl {
	struct hdspm_peak_rms *peak;
};

/* use indirect access due to the limit of ioctl bit size */
#define SNDRV_HDSPM_IOCTL_GET_PEAK_RMS \
	_IOR('H', 0x40, struct hdspm_peak_rms_ioctl)

/* ------------ CONFIG block IOCTL ---------------------- */

struct hdspm_config_info {
	unsigned char pref_sync_ref;
	unsigned char wordclock_sync_check;
	unsigned char madi_sync_check;
	unsigned int system_sample_rate;
	unsigned int autosync_sample_rate;
	unsigned char system_clock_mode;
	unsigned char clock_source;
	unsigned char autosync_ref;
	unsigned char line_out;
	unsigned int passthru;
	unsigned int analog_out;
};

#define SNDRV_HDSPM_IOCTL_GET_CONFIG_INFO \
	_IOR('H', 0x41, struct hdspm_config_info)


/* get Soundcard Version */

struct hdspm_version {
	unsigned short firmware_rev;
};

#define SNDRV_HDSPM_IOCTL_GET_VERSION _IOR('H', 0x43, struct hdspm_version)


/* ------------- get Matrix Mixer IOCTL --------------- */

/* MADI mixer: 64inputs+64playback in 64outputs = 8192 => *4Byte =
 * 32768 Bytes
 */

/* organisation is 64 channelfader in a continous memory block */
/* equivalent to hardware definition, maybe for future feature of mmap of
 * them
 */
/* each of 64 outputs has 64 infader and 64 outfader: 
   Ins to Outs mixer[out].in[in], Outstreams to Outs mixer[out].pb[pb] */

#define HDSPM_MIXER_CHANNELS HDSPM_MAX_CHANNELS

struct hdspm_channelfader {
	unsigned int in[HDSPM_MIXER_CHANNELS];
	unsigned int pb[HDSPM_MIXER_CHANNELS];
};

struct hdspm_mixer {
	struct hdspm_channelfader ch[HDSPM_MIXER_CHANNELS];
};

struct hdspm_mixer_ioctl {
	struct hdspm_mixer *mixer;
};

/* use indirect access due to the limit of ioctl bit size */
#define SNDRV_HDSPM_IOCTL_GET_MIXER _IOR('H', 0x44, struct hdspm_mixer_ioctl)

/* typedefs for compatibility to user-space */
typedef struct hdspm_peak_rms hdspm_peak_rms_t;
typedef struct hdspm_config_info hdspm_config_info_t;
typedef struct hdspm_version hdspm_version_t;
typedef struct hdspm_channelfader snd_hdspm_channelfader_t;
typedef struct hdspm_mixer hdspm_mixer_t;

#endif				/* __SOUND_HDSPM_H */
