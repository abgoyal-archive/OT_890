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
 * ac97.c  --  ALSA Soc AC97 codec support
 *
 * Copyright 2005 Wolfson Microelectronics PLC.
 * Author: Liam Girdwood <lrg@slimlogic.co.uk>
 *
 *  This program is free software; you can redistribute  it and/or modify it
 *  under  the terms of  the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the  License, or (at your
 *  option) any later version.
 *
 * Generic AC97 support.
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/ac97_codec.h>
#include <sound/initval.h>
#include <sound/soc.h>
#include "ac97.h"

#define AC97_VERSION "0.6"

static int ac97_prepare(struct snd_pcm_substream *substream,
			struct snd_soc_dai *dai)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_device *socdev = rtd->socdev;
	struct snd_soc_codec *codec = socdev->codec;

	int reg = (substream->stream == SNDRV_PCM_STREAM_PLAYBACK) ?
		  AC97_PCM_FRONT_DAC_RATE : AC97_PCM_LR_ADC_RATE;
	return snd_ac97_set_rate(codec->ac97, reg, runtime->rate);
}

#define STD_AC97_RATES (SNDRV_PCM_RATE_8000 | SNDRV_PCM_RATE_11025 |\
		SNDRV_PCM_RATE_22050 | SNDRV_PCM_RATE_44100 |\
		SNDRV_PCM_RATE_48000)

struct snd_soc_dai ac97_dai = {
	.name = "AC97 HiFi",
	.ac97_control = 1,
	.playback = {
		.stream_name = "AC97 Playback",
		.channels_min = 1,
		.channels_max = 2,
		.rates = STD_AC97_RATES,
		.formats = SNDRV_PCM_FMTBIT_S16_LE,},
	.capture = {
		.stream_name = "AC97 Capture",
		.channels_min = 1,
		.channels_max = 2,
		.rates = STD_AC97_RATES,
		.formats = SNDRV_PCM_FMTBIT_S16_LE,},
	.ops = {
		.prepare = ac97_prepare,},
};
EXPORT_SYMBOL_GPL(ac97_dai);

static unsigned int ac97_read(struct snd_soc_codec *codec,
	unsigned int reg)
{
	return soc_ac97_ops.read(codec->ac97, reg);
}

static int ac97_write(struct snd_soc_codec *codec, unsigned int reg,
	unsigned int val)
{
	soc_ac97_ops.write(codec->ac97, reg, val);
	return 0;
}

static int ac97_soc_probe(struct platform_device *pdev)
{
	struct snd_soc_device *socdev = platform_get_drvdata(pdev);
	struct snd_soc_codec *codec;
	struct snd_ac97_bus *ac97_bus;
	struct snd_ac97_template ac97_template;
	int ret = 0;

	printk(KERN_INFO "AC97 SoC Audio Codec %s\n", AC97_VERSION);

	socdev->codec = kzalloc(sizeof(struct snd_soc_codec), GFP_KERNEL);
	if (!socdev->codec)
		return -ENOMEM;
	codec = socdev->codec;
	mutex_init(&codec->mutex);

	codec->name = "AC97";
	codec->owner = THIS_MODULE;
	codec->dai = &ac97_dai;
	codec->num_dai = 1;
	codec->write = ac97_write;
	codec->read = ac97_read;
	INIT_LIST_HEAD(&codec->dapm_widgets);
	INIT_LIST_HEAD(&codec->dapm_paths);

	/* register pcms */
	ret = snd_soc_new_pcms(socdev, SNDRV_DEFAULT_IDX1, SNDRV_DEFAULT_STR1);
	if (ret < 0)
		goto err;

	/* add codec as bus device for standard ac97 */
	ret = snd_ac97_bus(codec->card, 0, &soc_ac97_ops, NULL, &ac97_bus);
	if (ret < 0)
		goto bus_err;

	memset(&ac97_template, 0, sizeof(struct snd_ac97_template));
	ret = snd_ac97_mixer(ac97_bus, &ac97_template, &codec->ac97);
	if (ret < 0)
		goto bus_err;

	ret = snd_soc_init_card(socdev);
	if (ret < 0)
		goto bus_err;
	return 0;

bus_err:
	snd_soc_free_pcms(socdev);

err:
	kfree(socdev->codec->reg_cache);
	kfree(socdev->codec);
	socdev->codec = NULL;
	return ret;
}

static int ac97_soc_remove(struct platform_device *pdev)
{
	struct snd_soc_device *socdev = platform_get_drvdata(pdev);
	struct snd_soc_codec *codec = socdev->codec;

	if (!codec)
		return 0;

	snd_soc_free_pcms(socdev);
	kfree(socdev->codec->reg_cache);
	kfree(socdev->codec);

	return 0;
}

#ifdef CONFIG_PM
static int ac97_soc_suspend(struct platform_device *pdev, pm_message_t msg)
{
	struct snd_soc_device *socdev = platform_get_drvdata(pdev);

	snd_ac97_suspend(socdev->codec->ac97);

	return 0;
}

static int ac97_soc_resume(struct platform_device *pdev)
{
	struct snd_soc_device *socdev = platform_get_drvdata(pdev);

	snd_ac97_resume(socdev->codec->ac97);

	return 0;
}
#else
#define ac97_soc_suspend NULL
#define ac97_soc_resume NULL
#endif

struct snd_soc_codec_device soc_codec_dev_ac97 = {
	.probe = 	ac97_soc_probe,
	.remove = 	ac97_soc_remove,
	.suspend =	ac97_soc_suspend,
	.resume =	ac97_soc_resume,
};
EXPORT_SYMBOL_GPL(soc_codec_dev_ac97);

MODULE_DESCRIPTION("Soc Generic AC97 driver");
MODULE_AUTHOR("Liam Girdwood");
MODULE_LICENSE("GPL");
