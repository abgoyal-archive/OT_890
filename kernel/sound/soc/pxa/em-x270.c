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
 * em-x270.c  --  SoC audio for EM-X270
 *
 * Copyright 2007 CompuLab, Ltd.
 *
 * Author: Mike Rapoport <mike@compulab.co.il>
 *
 * Copied from tosa.c:
 * Copyright 2005 Wolfson Microelectronics PLC.
 * Copyright 2005 Openedhand Ltd.
 *
 * Authors: Liam Girdwood <lrg@slimlogic.co.uk>
 *          Richard Purdie <richard@openedhand.com>
 *
 *  This program is free software; you can redistribute  it and/or modify it
 *  under  the terms of  the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the  License, or (at your
 *  option) any later version.
 *
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/device.h>

#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/soc.h>
#include <sound/soc-dapm.h>

#include <asm/mach-types.h>
#include <mach/pxa-regs.h>
#include <mach/hardware.h>
#include <mach/audio.h>

#include "../codecs/wm9712.h"
#include "pxa2xx-pcm.h"
#include "pxa2xx-ac97.h"

static struct snd_soc_dai_link em_x270_dai[] = {
	{
		.name = "AC97",
		.stream_name = "AC97 HiFi",
		.cpu_dai = &pxa_ac97_dai[PXA2XX_DAI_AC97_HIFI],
		.codec_dai = &wm9712_dai[WM9712_DAI_AC97_HIFI],
	},
	{
		.name = "AC97 Aux",
		.stream_name = "AC97 Aux",
		.cpu_dai = &pxa_ac97_dai[PXA2XX_DAI_AC97_AUX],
		.codec_dai = &wm9712_dai[WM9712_DAI_AC97_AUX],
	},
};

static struct snd_soc_card em_x270 = {
	.name = "EM-X270",
	.platform = &pxa2xx_soc_platform,
	.dai_link = em_x270_dai,
	.num_links = ARRAY_SIZE(em_x270_dai),
};

static struct snd_soc_device em_x270_snd_devdata = {
	.card = &em_x270,
	.codec_dev = &soc_codec_dev_wm9712,
};

static struct platform_device *em_x270_snd_device;

static int __init em_x270_init(void)
{
	int ret;

	if (!machine_is_em_x270())
		return -ENODEV;

	em_x270_snd_device = platform_device_alloc("soc-audio", -1);
	if (!em_x270_snd_device)
		return -ENOMEM;

	platform_set_drvdata(em_x270_snd_device, &em_x270_snd_devdata);
	em_x270_snd_devdata.dev = &em_x270_snd_device->dev;
	ret = platform_device_add(em_x270_snd_device);

	if (ret)
		platform_device_put(em_x270_snd_device);

	return ret;
}

static void __exit em_x270_exit(void)
{
	platform_device_unregister(em_x270_snd_device);
}

module_init(em_x270_init);
module_exit(em_x270_exit);

/* Module information */
MODULE_AUTHOR("Mike Rapoport");
MODULE_DESCRIPTION("ALSA SoC EM-X270");
MODULE_LICENSE("GPL");
