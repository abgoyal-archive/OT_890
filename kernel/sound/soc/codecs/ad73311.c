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
 * ad73311.c  --  ALSA Soc AD73311 codec support
 *
 * Copyright:	Analog Device Inc.
 * Author:	Cliff Cai <cliff.cai@analog.com>
 *
 *  This program is free software; you can redistribute  it and/or modify it
 *  under  the terms of  the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the  License, or (at your
 *  option) any later version.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/ac97_codec.h>
#include <sound/initval.h>
#include <sound/soc.h>

#include "ad73311.h"

struct snd_soc_dai ad73311_dai = {
	.name = "AD73311",
	.playback = {
		.stream_name = "Playback",
		.channels_min = 1,
		.channels_max = 1,
		.rates = SNDRV_PCM_RATE_8000,
		.formats = SNDRV_PCM_FMTBIT_S16_LE, },
	.capture = {
		.stream_name = "Capture",
		.channels_min = 1,
		.channels_max = 1,
		.rates = SNDRV_PCM_RATE_8000,
		.formats = SNDRV_PCM_FMTBIT_S16_LE, },
};
EXPORT_SYMBOL_GPL(ad73311_dai);

static int ad73311_soc_probe(struct platform_device *pdev)
{
	struct snd_soc_device *socdev = platform_get_drvdata(pdev);
	struct snd_soc_codec *codec;
	int ret = 0;

	codec = kzalloc(sizeof(struct snd_soc_codec), GFP_KERNEL);
	if (codec == NULL)
		return -ENOMEM;
	mutex_init(&codec->mutex);
	codec->name = "AD73311";
	codec->owner = THIS_MODULE;
	codec->dai = &ad73311_dai;
	codec->num_dai = 1;
	socdev->codec = codec;
	INIT_LIST_HEAD(&codec->dapm_widgets);
	INIT_LIST_HEAD(&codec->dapm_paths);

	/* register pcms */
	ret = snd_soc_new_pcms(socdev, SNDRV_DEFAULT_IDX1, SNDRV_DEFAULT_STR1);
	if (ret < 0) {
		printk(KERN_ERR "ad73311: failed to create pcms\n");
		goto pcm_err;
	}

	ret = snd_soc_init_card(socdev);
	if (ret < 0) {
		printk(KERN_ERR "ad73311: failed to register card\n");
		goto register_err;
	}

	return ret;

register_err:
	snd_soc_free_pcms(socdev);
pcm_err:
	kfree(socdev->codec);
	socdev->codec = NULL;
	return ret;
}

static int ad73311_soc_remove(struct platform_device *pdev)
{
	struct snd_soc_device *socdev = platform_get_drvdata(pdev);
	struct snd_soc_codec *codec = socdev->codec;

	if (codec == NULL)
		return 0;
	snd_soc_free_pcms(socdev);
	kfree(codec);
	return 0;
}

struct snd_soc_codec_device soc_codec_dev_ad73311 = {
	.probe = 	ad73311_soc_probe,
	.remove = 	ad73311_soc_remove,
};
EXPORT_SYMBOL_GPL(soc_codec_dev_ad73311);

static int __init ad73311_init(void)
{
	return snd_soc_register_dai(&ad73311_dai);
}
module_init(ad73311_init);

static void __exit ad73311_exit(void)
{
	snd_soc_unregister_dai(&ad73311_dai);
}
module_exit(ad73311_exit);

MODULE_DESCRIPTION("ASoC ad73311 driver");
MODULE_AUTHOR("Cliff Cai ");
MODULE_LICENSE("GPL");
