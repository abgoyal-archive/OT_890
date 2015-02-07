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
 * PC-Speaker driver for Linux
 *
 * Mixer implementation.
 * Copyright (C) 2001-2008  Stas Sergeev
 */

#include <sound/core.h>
#include <sound/control.h>
#include "pcsp.h"


static int pcsp_enable_info(struct snd_kcontrol *kcontrol,
			    struct snd_ctl_elem_info *uinfo)
{
	uinfo->type = SNDRV_CTL_ELEM_TYPE_BOOLEAN;
	uinfo->count = 1;
	uinfo->value.integer.min = 0;
	uinfo->value.integer.max = 1;
	return 0;
}

static int pcsp_enable_get(struct snd_kcontrol *kcontrol,
			   struct snd_ctl_elem_value *ucontrol)
{
	struct snd_pcsp *chip = snd_kcontrol_chip(kcontrol);
	ucontrol->value.integer.value[0] = chip->enable;
	return 0;
}

static int pcsp_enable_put(struct snd_kcontrol *kcontrol,
			   struct snd_ctl_elem_value *ucontrol)
{
	struct snd_pcsp *chip = snd_kcontrol_chip(kcontrol);
	int changed = 0;
	int enab = ucontrol->value.integer.value[0];
	if (enab != chip->enable) {
		chip->enable = enab;
		changed = 1;
	}
	return changed;
}

static int pcsp_treble_info(struct snd_kcontrol *kcontrol,
			    struct snd_ctl_elem_info *uinfo)
{
	struct snd_pcsp *chip = snd_kcontrol_chip(kcontrol);
	uinfo->type = SNDRV_CTL_ELEM_TYPE_ENUMERATED;
	uinfo->count = 1;
	uinfo->value.enumerated.items = chip->max_treble + 1;
	if (uinfo->value.enumerated.item > chip->max_treble)
		uinfo->value.enumerated.item = chip->max_treble;
	sprintf(uinfo->value.enumerated.name, "%d",
			PCSP_CALC_RATE(uinfo->value.enumerated.item));
	return 0;
}

static int pcsp_treble_get(struct snd_kcontrol *kcontrol,
			   struct snd_ctl_elem_value *ucontrol)
{
	struct snd_pcsp *chip = snd_kcontrol_chip(kcontrol);
	ucontrol->value.enumerated.item[0] = chip->treble;
	return 0;
}

static int pcsp_treble_put(struct snd_kcontrol *kcontrol,
			   struct snd_ctl_elem_value *ucontrol)
{
	struct snd_pcsp *chip = snd_kcontrol_chip(kcontrol);
	int changed = 0;
	int treble = ucontrol->value.enumerated.item[0];
	if (treble != chip->treble) {
		chip->treble = treble;
#if PCSP_DEBUG
		printk(KERN_INFO "PCSP: rate set to %i\n", PCSP_RATE());
#endif
		changed = 1;
	}
	return changed;
}

static int pcsp_pcspkr_info(struct snd_kcontrol *kcontrol,
			    struct snd_ctl_elem_info *uinfo)
{
	uinfo->type = SNDRV_CTL_ELEM_TYPE_BOOLEAN;
	uinfo->count = 1;
	uinfo->value.integer.min = 0;
	uinfo->value.integer.max = 1;
	return 0;
}

static int pcsp_pcspkr_get(struct snd_kcontrol *kcontrol,
			   struct snd_ctl_elem_value *ucontrol)
{
	struct snd_pcsp *chip = snd_kcontrol_chip(kcontrol);
	ucontrol->value.integer.value[0] = chip->pcspkr;
	return 0;
}

static int pcsp_pcspkr_put(struct snd_kcontrol *kcontrol,
			   struct snd_ctl_elem_value *ucontrol)
{
	struct snd_pcsp *chip = snd_kcontrol_chip(kcontrol);
	int changed = 0;
	int spkr = ucontrol->value.integer.value[0];
	if (spkr != chip->pcspkr) {
		chip->pcspkr = spkr;
		changed = 1;
	}
	return changed;
}

#define PCSP_MIXER_CONTROL(ctl_type, ctl_name) \
{ \
	.iface =	SNDRV_CTL_ELEM_IFACE_MIXER, \
	.name =		ctl_name, \
	.info =		pcsp_##ctl_type##_info, \
	.get =		pcsp_##ctl_type##_get, \
	.put =		pcsp_##ctl_type##_put, \
}

static struct snd_kcontrol_new __devinitdata snd_pcsp_controls[] = {
	PCSP_MIXER_CONTROL(enable, "Master Playback Switch"),
	PCSP_MIXER_CONTROL(treble, "BaseFRQ Playback Volume"),
	PCSP_MIXER_CONTROL(pcspkr, "PC Speaker Playback Switch"),
};

int __devinit snd_pcsp_new_mixer(struct snd_pcsp *chip)
{
	struct snd_card *card = chip->card;
	int i, err;

	for (i = 0; i < ARRAY_SIZE(snd_pcsp_controls); i++) {
		err = snd_ctl_add(card,
				 snd_ctl_new1(snd_pcsp_controls + i,
					      chip));
		if (err < 0)
			return err;
	}

	strcpy(card->mixername, "PC-Speaker");

	return 0;
}
