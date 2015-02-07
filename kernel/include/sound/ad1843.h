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
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright 2003 Vivien Chappelier <vivien.chappelier@linux-mips.org>
 * Copyright 2008 Thomas Bogendoerfer <tsbogend@franken.de>
 */

#ifndef __SOUND_AD1843_H
#define __SOUND_AD1843_H

struct snd_ad1843 {
	void *chip;
	int (*read)(void *chip, int reg);
	int (*write)(void *chip, int reg, int val);
};

#define AD1843_GAIN_RECLEV 0
#define AD1843_GAIN_LINE   1
#define AD1843_GAIN_LINE_2 2
#define AD1843_GAIN_MIC    3
#define AD1843_GAIN_PCM_0  4
#define AD1843_GAIN_PCM_1  5
#define AD1843_GAIN_SIZE   (AD1843_GAIN_PCM_1+1)

int ad1843_get_gain_max(struct snd_ad1843 *ad1843, int id);
int ad1843_get_gain(struct snd_ad1843 *ad1843, int id);
int ad1843_set_gain(struct snd_ad1843 *ad1843, int id, int newval);
int ad1843_get_recsrc(struct snd_ad1843 *ad1843);
int ad1843_set_recsrc(struct snd_ad1843 *ad1843, int newsrc);
void ad1843_setup_dac(struct snd_ad1843 *ad1843,
		      unsigned int id,
		      unsigned int framerate,
		      snd_pcm_format_t fmt,
		      unsigned int channels);
void ad1843_shutdown_dac(struct snd_ad1843 *ad1843,
			 unsigned int id);
void ad1843_setup_adc(struct snd_ad1843 *ad1843,
		      unsigned int framerate,
		      snd_pcm_format_t fmt,
		      unsigned int channels);
void ad1843_shutdown_adc(struct snd_ad1843 *ad1843);
int ad1843_init(struct snd_ad1843 *ad1843);

#endif /* __SOUND_AD1843_H */
