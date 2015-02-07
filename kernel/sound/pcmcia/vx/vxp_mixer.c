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
 * Driver for Digigram VXpocket soundcards
 *
 * VX-pocket mixer
 *
 * Copyright (c) 2002 by Takashi Iwai <tiwai@suse.de>
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

#include <sound/core.h>
#include <sound/control.h>
#include <sound/tlv.h>
#include "vxpocket.h"

#define MIC_LEVEL_MIN	0
#define MIC_LEVEL_MAX	8

/*
 * mic level control (for VXPocket)
 */
static int vx_mic_level_info(struct snd_kcontrol *kcontrol, struct snd_ctl_elem_info *uinfo)
{
	uinfo->type = SNDRV_CTL_ELEM_TYPE_INTEGER;
	uinfo->count = 1;
	uinfo->value.integer.min = 0;
	uinfo->value.integer.max = MIC_LEVEL_MAX;
	return 0;
}

static int vx_mic_level_get(struct snd_kcontrol *kcontrol, struct snd_ctl_elem_value *ucontrol)
{
	struct vx_core *_chip = snd_kcontrol_chip(kcontrol);
	struct snd_vxpocket *chip = (struct snd_vxpocket *)_chip;
	ucontrol->value.integer.value[0] = chip->mic_level;
	return 0;
}

static int vx_mic_level_put(struct snd_kcontrol *kcontrol, struct snd_ctl_elem_value *ucontrol)
{
	struct vx_core *_chip = snd_kcontrol_chip(kcontrol);
	struct snd_vxpocket *chip = (struct snd_vxpocket *)_chip;
	unsigned int val = ucontrol->value.integer.value[0];

	if (val > MIC_LEVEL_MAX)
		return -EINVAL;
	mutex_lock(&_chip->mixer_mutex);
	if (chip->mic_level != ucontrol->value.integer.value[0]) {
		vx_set_mic_level(_chip, ucontrol->value.integer.value[0]);
		chip->mic_level = ucontrol->value.integer.value[0];
		mutex_unlock(&_chip->mixer_mutex);
		return 1;
	}
	mutex_unlock(&_chip->mixer_mutex);
	return 0;
}

static const DECLARE_TLV_DB_SCALE(db_scale_mic, -21, 3, 0);

static struct snd_kcontrol_new vx_control_mic_level = {
	.iface =	SNDRV_CTL_ELEM_IFACE_MIXER,
	.access =	(SNDRV_CTL_ELEM_ACCESS_READWRITE |
			 SNDRV_CTL_ELEM_ACCESS_TLV_READ),
	.name =		"Mic Capture Volume",
	.info =		vx_mic_level_info,
	.get =		vx_mic_level_get,
	.put =		vx_mic_level_put,
	.tlv = { .p = db_scale_mic },
};

/*
 * mic boost level control (for VXP440)
 */
#define vx_mic_boost_info		snd_ctl_boolean_mono_info

static int vx_mic_boost_get(struct snd_kcontrol *kcontrol, struct snd_ctl_elem_value *ucontrol)
{
	struct vx_core *_chip = snd_kcontrol_chip(kcontrol);
	struct snd_vxpocket *chip = (struct snd_vxpocket *)_chip;
	ucontrol->value.integer.value[0] = chip->mic_level;
	return 0;
}

static int vx_mic_boost_put(struct snd_kcontrol *kcontrol, struct snd_ctl_elem_value *ucontrol)
{
	struct vx_core *_chip = snd_kcontrol_chip(kcontrol);
	struct snd_vxpocket *chip = (struct snd_vxpocket *)_chip;
	int val = !!ucontrol->value.integer.value[0];
	mutex_lock(&_chip->mixer_mutex);
	if (chip->mic_level != val) {
		vx_set_mic_boost(_chip, val);
		chip->mic_level = val;
		mutex_unlock(&_chip->mixer_mutex);
		return 1;
	}
	mutex_unlock(&_chip->mixer_mutex);
	return 0;
}

static struct snd_kcontrol_new vx_control_mic_boost = {
	.iface =	SNDRV_CTL_ELEM_IFACE_MIXER,
	.name =		"Mic Boost",
	.info =		vx_mic_boost_info,
	.get =		vx_mic_boost_get,
	.put =		vx_mic_boost_put,
};


int vxp_add_mic_controls(struct vx_core *_chip)
{
	struct snd_vxpocket *chip = (struct snd_vxpocket *)_chip;
	int err;

	/* mute input levels */
	chip->mic_level = 0;
	switch (_chip->type) {
	case VX_TYPE_VXPOCKET:
		vx_set_mic_level(_chip, 0);
		break;
	case VX_TYPE_VXP440:
		vx_set_mic_boost(_chip, 0);
		break;
	}

	/* mic level */
	switch (_chip->type) {
	case VX_TYPE_VXPOCKET:
		if ((err = snd_ctl_add(_chip->card, snd_ctl_new1(&vx_control_mic_level, chip))) < 0)
			return err;
		break;
	case VX_TYPE_VXP440:
		if ((err = snd_ctl_add(_chip->card, snd_ctl_new1(&vx_control_mic_boost, chip))) < 0)
			return err;
		break;
	}

	return 0;
}

