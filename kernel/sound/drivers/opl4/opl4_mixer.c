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
 * OPL4 mixer functions
 * Copyright (c) 2003 by Clemens Ladisch <clemens@ladisch.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 */

#include "opl4_local.h"
#include <sound/control.h>

static int snd_opl4_ctl_info(struct snd_kcontrol *kcontrol, struct snd_ctl_elem_info *uinfo)
{
	uinfo->type = SNDRV_CTL_ELEM_TYPE_INTEGER;
	uinfo->count = 2;
	uinfo->value.integer.min = 0;
	uinfo->value.integer.max = 7;
	return 0;
}

static int snd_opl4_ctl_get(struct snd_kcontrol *kcontrol, struct snd_ctl_elem_value *ucontrol)
{
	struct snd_opl4 *opl4 = snd_kcontrol_chip(kcontrol);
	unsigned long flags;
	u8 reg = kcontrol->private_value;
	u8 value;

	spin_lock_irqsave(&opl4->reg_lock, flags);
	value = snd_opl4_read(opl4, reg);
	spin_unlock_irqrestore(&opl4->reg_lock, flags);
	ucontrol->value.integer.value[0] = 7 - (value & 7);
	ucontrol->value.integer.value[1] = 7 - ((value >> 3) & 7);
	return 0;
}

static int snd_opl4_ctl_put(struct snd_kcontrol *kcontrol, struct snd_ctl_elem_value *ucontrol)
{
	struct snd_opl4 *opl4 = snd_kcontrol_chip(kcontrol);
	unsigned long flags;
	u8 reg = kcontrol->private_value;
	u8 value, old_value;

	value = (7 - (ucontrol->value.integer.value[0] & 7)) |
		((7 - (ucontrol->value.integer.value[1] & 7)) << 3);
	spin_lock_irqsave(&opl4->reg_lock, flags);
	old_value = snd_opl4_read(opl4, reg);
	snd_opl4_write(opl4, reg, value);
	spin_unlock_irqrestore(&opl4->reg_lock, flags);
	return value != old_value;
}

static struct snd_kcontrol_new snd_opl4_controls[] = {
	{
		.iface = SNDRV_CTL_ELEM_IFACE_MIXER,
		.name = "FM Playback Volume",
		.info = snd_opl4_ctl_info,
		.get = snd_opl4_ctl_get,
		.put = snd_opl4_ctl_put,
		.private_value = OPL4_REG_MIX_CONTROL_FM
	},
	{
		.iface = SNDRV_CTL_ELEM_IFACE_MIXER,
		.name = "Wavetable Playback Volume",
		.info = snd_opl4_ctl_info,
		.get = snd_opl4_ctl_get,
		.put = snd_opl4_ctl_put,
		.private_value = OPL4_REG_MIX_CONTROL_PCM
	}
};

int snd_opl4_create_mixer(struct snd_opl4 *opl4)
{
	struct snd_card *card = opl4->card;
	int i, err;

	strcat(card->mixername, ",OPL4");

	for (i = 0; i < 2; ++i) {
		err = snd_ctl_add(card, snd_ctl_new1(&snd_opl4_controls[i], opl4));
		if (err < 0)
			return err;
	}
	return 0;
}
