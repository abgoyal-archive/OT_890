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
 * Generic AC97 sound support for SH7760
 *
 * (c) 2007 Manuel Lauss
 *
 * Licensed under the GPLv2.
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/platform_device.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/soc.h>
#include <sound/soc-dapm.h>
#include <asm/io.h>

#include "../codecs/ac97.h"

#define IPSEL 0xFE400034

/* platform specific structs can be declared here */
extern struct snd_soc_dai sh4_hac_dai[2];
extern struct snd_soc_platform sh7760_soc_platform;

static int machine_init(struct snd_soc_codec *codec)
{
	snd_soc_dapm_sync(codec);
	return 0;
}

static struct snd_soc_dai_link sh7760_ac97_dai = {
	.name = "AC97",
	.stream_name = "AC97 HiFi",
	.cpu_dai = &sh4_hac_dai[0],	/* HAC0 */
	.codec_dai = &ac97_dai,
	.init = machine_init,
	.ops = NULL,
};

static struct snd_soc_card sh7760_ac97_soc_machine  = {
	.name = "SH7760 AC97",
	.platform = &sh7760_soc_platform,
	.dai_link = &sh7760_ac97_dai,
	.num_links = 1,
};

static struct snd_soc_device sh7760_ac97_snd_devdata = {
	.card = &sh7760_ac97_soc_machine,
	.codec_dev = &soc_codec_dev_ac97,
};

static struct platform_device *sh7760_ac97_snd_device;

static int __init sh7760_ac97_init(void)
{
	int ret;
	unsigned short ipsel;

	/* enable both AC97 controllers in pinmux reg */
	ipsel = ctrl_inw(IPSEL);
	ctrl_outw(ipsel | (3 << 10), IPSEL);

	ret = -ENOMEM;
	sh7760_ac97_snd_device = platform_device_alloc("soc-audio", -1);
	if (!sh7760_ac97_snd_device)
		goto out;

	platform_set_drvdata(sh7760_ac97_snd_device,
			     &sh7760_ac97_snd_devdata);
	sh7760_ac97_snd_devdata.dev = &sh7760_ac97_snd_device->dev;
	ret = platform_device_add(sh7760_ac97_snd_device);

	if (ret)
		platform_device_put(sh7760_ac97_snd_device);

out:
	return ret;
}

static void __exit sh7760_ac97_exit(void)
{
	platform_device_unregister(sh7760_ac97_snd_device);
}

module_init(sh7760_ac97_init);
module_exit(sh7760_ac97_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Generic SH7760 AC97 sound machine");
MODULE_AUTHOR("Manuel Lauss <mano@roarinelk.homelinux.net>");
