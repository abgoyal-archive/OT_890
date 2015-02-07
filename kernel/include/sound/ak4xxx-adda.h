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

#ifndef __SOUND_AK4XXX_ADDA_H
#define __SOUND_AK4XXX_ADDA_H

/*
 *   ALSA driver for AK4524 / AK4528 / AK4529 / AK4355 / AK4381
 *   AD and DA converters
 *
 *	Copyright (c) 2000 Jaroslav Kysela <perex@perex.cz>
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

#ifndef AK4XXX_MAX_CHIPS
#define AK4XXX_MAX_CHIPS	4
#endif

struct snd_akm4xxx;

struct snd_ak4xxx_ops {
	void (*lock)(struct snd_akm4xxx *ak, int chip);
	void (*unlock)(struct snd_akm4xxx *ak, int chip);
	void (*write)(struct snd_akm4xxx *ak, int chip, unsigned char reg,
		      unsigned char val);
	void (*set_rate_val)(struct snd_akm4xxx *ak, unsigned int rate);
};

#define AK4XXX_IMAGE_SIZE	(AK4XXX_MAX_CHIPS * 16)	/* 64 bytes */

/* DAC label and channels */
struct snd_akm4xxx_dac_channel {
	char *name;		/* mixer volume name */
	unsigned int num_channels;
	char *switch_name;		/* mixer switch*/
};

/* ADC labels and channels */
struct snd_akm4xxx_adc_channel {
	char *name;		/* capture gain volume label */
	char *switch_name;	/* capture switch */
	unsigned int num_channels;
	char *selector_name;	/* capture source select label */
	const char **input_names; /* capture source names (NULL terminated) */
};

struct snd_akm4xxx {
	struct snd_card *card;
	unsigned int num_adcs;			/* AK4524 or AK4528 ADCs */
	unsigned int num_dacs;			/* AK4524 or AK4528 DACs */
	unsigned char images[AK4XXX_IMAGE_SIZE]; /* saved register image */
	unsigned char volumes[AK4XXX_IMAGE_SIZE]; /* saved volume values */
	unsigned long private_value[AK4XXX_MAX_CHIPS];	/* helper for driver */
	void *private_data[AK4XXX_MAX_CHIPS];		/* helper for driver */
	/* template should fill the following fields */
	unsigned int idx_offset;		/* control index offset */
	enum {
		SND_AK4524, SND_AK4528, SND_AK4529,
		SND_AK4355, SND_AK4358, SND_AK4381,
		SND_AK5365
	} type;

	/* (array) information of combined codecs */
	const struct snd_akm4xxx_dac_channel *dac_info;
	const struct snd_akm4xxx_adc_channel *adc_info;

	struct snd_ak4xxx_ops ops;
};

void snd_akm4xxx_write(struct snd_akm4xxx *ak, int chip, unsigned char reg,
		       unsigned char val);
void snd_akm4xxx_reset(struct snd_akm4xxx *ak, int state);
void snd_akm4xxx_init(struct snd_akm4xxx *ak);
int snd_akm4xxx_build_controls(struct snd_akm4xxx *ak);

#define snd_akm4xxx_get(ak,chip,reg) \
	(ak)->images[(chip) * 16 + (reg)]
#define snd_akm4xxx_set(ak,chip,reg,val) \
	((ak)->images[(chip) * 16 + (reg)] = (val))
#define snd_akm4xxx_get_vol(ak,chip,reg) \
	(ak)->volumes[(chip) * 16 + (reg)]
#define snd_akm4xxx_set_vol(ak,chip,reg,val) \
	((ak)->volumes[(chip) * 16 + (reg)] = (val))

#endif /* __SOUND_AK4XXX_ADDA_H */
