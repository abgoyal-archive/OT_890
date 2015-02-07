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

#ifndef __SOUND_ASOUND_FM_H
#define __SOUND_ASOUND_FM_H

/*
 *  Advanced Linux Sound Architecture - ALSA
 *
 *  Interface file between ALSA driver & user space
 *  Copyright (c) 1994-98 by Jaroslav Kysela <perex@perex.cz>,
 *                           4Front Technologies
 *
 *  Direct FM control
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

#define SNDRV_DM_FM_MODE_OPL2	0x00
#define SNDRV_DM_FM_MODE_OPL3	0x01

struct snd_dm_fm_info {
	unsigned char fm_mode;		/* OPL mode, see SNDRV_DM_FM_MODE_XXX */
	unsigned char rhythm;		/* percussion mode flag */
};

/*
 *  Data structure composing an FM "note" or sound event.
 */

struct snd_dm_fm_voice {
	unsigned char op;		/* operator cell (0 or 1) */
	unsigned char voice;		/* FM voice (0 to 17) */

	unsigned char am;		/* amplitude modulation */
	unsigned char vibrato;		/* vibrato effect */
	unsigned char do_sustain;	/* sustain phase */
	unsigned char kbd_scale;	/* keyboard scaling */
	unsigned char harmonic;		/* 4 bits: harmonic and multiplier */
	unsigned char scale_level;	/* 2 bits: decrease output freq rises */
	unsigned char volume;		/* 6 bits: volume */

	unsigned char attack;		/* 4 bits: attack rate */
	unsigned char decay;		/* 4 bits: decay rate */
	unsigned char sustain;		/* 4 bits: sustain level */
	unsigned char release;		/* 4 bits: release rate */

	unsigned char feedback;		/* 3 bits: feedback for op0 */
	unsigned char connection;	/* 0 for serial, 1 for parallel */
	unsigned char left;		/* stereo left */
	unsigned char right;		/* stereo right */
	unsigned char waveform;		/* 3 bits: waveform shape */
};

/*
 *  This describes an FM note by its voice, octave, frequency number (10bit)
 *  and key on/off.
 */

struct snd_dm_fm_note {
	unsigned char voice;	/* 0-17 voice channel */
	unsigned char octave;	/* 3 bits: what octave to play */
	unsigned int fnum;	/* 10 bits: frequency number */
	unsigned char key_on;	/* set for active, clear for silent */
};

/*
 *  FM parameters that apply globally to all voices, and thus are not "notes"
 */

struct snd_dm_fm_params {
	unsigned char am_depth;		/* amplitude modulation depth (1=hi) */
	unsigned char vib_depth;	/* vibrato depth (1=hi) */
	unsigned char kbd_split;	/* keyboard split */
	unsigned char rhythm;		/* percussion mode select */

	/* This block is the percussion instrument data */
	unsigned char bass;
	unsigned char snare;
	unsigned char tomtom;
	unsigned char cymbal;
	unsigned char hihat;
};

/*
 *  FM mode ioctl settings
 */

#define SNDRV_DM_FM_IOCTL_INFO		_IOR('H', 0x20, struct snd_dm_fm_info)
#define SNDRV_DM_FM_IOCTL_RESET		_IO ('H', 0x21)
#define SNDRV_DM_FM_IOCTL_PLAY_NOTE	_IOW('H', 0x22, struct snd_dm_fm_note)
#define SNDRV_DM_FM_IOCTL_SET_VOICE	_IOW('H', 0x23, struct snd_dm_fm_voice)
#define SNDRV_DM_FM_IOCTL_SET_PARAMS	_IOW('H', 0x24, struct snd_dm_fm_params)
#define SNDRV_DM_FM_IOCTL_SET_MODE	_IOW('H', 0x25, int)
/* for OPL3 only */
#define SNDRV_DM_FM_IOCTL_SET_CONNECTION	_IOW('H', 0x26, int)
/* SBI patch management */
#define SNDRV_DM_FM_IOCTL_CLEAR_PATCHES	_IO ('H', 0x40)

#define SNDRV_DM_FM_OSS_IOCTL_RESET		0x20
#define SNDRV_DM_FM_OSS_IOCTL_PLAY_NOTE		0x21
#define SNDRV_DM_FM_OSS_IOCTL_SET_VOICE		0x22
#define SNDRV_DM_FM_OSS_IOCTL_SET_PARAMS	0x23
#define SNDRV_DM_FM_OSS_IOCTL_SET_MODE		0x24
#define SNDRV_DM_FM_OSS_IOCTL_SET_OPL		0x25

/*
 * Patch Record - fixed size for write
 */

#define FM_KEY_SBI	"SBI\032"
#define FM_KEY_2OP	"2OP\032"
#define FM_KEY_4OP	"4OP\032"

struct sbi_patch {
	unsigned char prog;
	unsigned char bank;
	char key[4];
	char name[25];
	char extension[7];
	unsigned char data[32];
};

#endif /* __SOUND_ASOUND_FM_H */
