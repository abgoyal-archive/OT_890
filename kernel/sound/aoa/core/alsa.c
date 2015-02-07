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
 * Apple Onboard Audio Alsa helpers
 *
 * Copyright 2006 Johannes Berg <johannes@sipsolutions.net>
 *
 * GPL v2, can be found in COPYING.
 */
#include <linux/module.h>
#include "alsa.h"

static int index = -1;
module_param(index, int, 0444);
MODULE_PARM_DESC(index, "index for AOA sound card.");

static struct aoa_card *aoa_card;

int aoa_alsa_init(char *name, struct module *mod, struct device *dev)
{
	struct snd_card *alsa_card;
	int err;

	if (aoa_card)
		/* cannot be EEXIST due to usage in aoa_fabric_register */
		return -EBUSY;

	alsa_card = snd_card_new(index, name, mod, sizeof(struct aoa_card));
	if (!alsa_card)
		return -ENOMEM;
	aoa_card = alsa_card->private_data;
	aoa_card->alsa_card = alsa_card;
	alsa_card->dev = dev;
	strlcpy(alsa_card->driver, "AppleOnbdAudio", sizeof(alsa_card->driver));
	strlcpy(alsa_card->shortname, name, sizeof(alsa_card->shortname));
	strlcpy(alsa_card->longname, name, sizeof(alsa_card->longname));
	strlcpy(alsa_card->mixername, name, sizeof(alsa_card->mixername));
	err = snd_card_register(aoa_card->alsa_card);
	if (err < 0) {
		printk(KERN_ERR "snd-aoa: couldn't register alsa card\n");
		snd_card_free(aoa_card->alsa_card);
		aoa_card = NULL;
		return err;
	}
	return 0;
}

struct snd_card *aoa_get_card(void)
{
	if (aoa_card)
		return aoa_card->alsa_card;
	return NULL;
}
EXPORT_SYMBOL_GPL(aoa_get_card);

void aoa_alsa_cleanup(void)
{
	if (aoa_card) {
		snd_card_free(aoa_card->alsa_card);
		aoa_card = NULL;
	}
}

int aoa_snd_device_new(snd_device_type_t type,
		       void * device_data, struct snd_device_ops * ops)
{
	struct snd_card *card = aoa_get_card();
	int err;

	if (!card) return -ENOMEM;

	err = snd_device_new(card, type, device_data, ops);
	if (err) {
		printk(KERN_ERR "snd-aoa: failed to create snd device (%d)\n", err);
		return err;
	}
	err = snd_device_register(card, device_data);
	if (err) {
		printk(KERN_ERR "snd-aoa: failed to register "
				"snd device (%d)\n", err);
		printk(KERN_ERR "snd-aoa: have you forgotten the "
				"dev_register callback?\n");
		snd_device_free(card, device_data);
	}
	return err;
}
EXPORT_SYMBOL_GPL(aoa_snd_device_new);

int aoa_snd_ctl_add(struct snd_kcontrol* control)
{
	int err;

	if (!aoa_card) return -ENODEV;

	err = snd_ctl_add(aoa_card->alsa_card, control);
	if (err)
		printk(KERN_ERR "snd-aoa: failed to add alsa control (%d)\n",
		       err);
	return err;
}
EXPORT_SYMBOL_GPL(aoa_snd_ctl_add);
