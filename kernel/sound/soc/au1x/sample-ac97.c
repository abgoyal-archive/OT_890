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
 * Sample Au12x0/Au1550 PSC AC97 sound machine.
 *
 * Copyright (c) 2007-2008 Manuel Lauss <mano@roarinelk.homelinux.net>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms outlined in the file COPYING at the root of this
 *  source archive.
 *
 * This is a very generic AC97 sound machine driver for boards which
 * have (AC97) audio at PSC1 (e.g. DB1200 demoboards).
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/timer.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/soc.h>
#include <sound/soc-dapm.h>
#include <asm/mach-au1x00/au1000.h>
#include <asm/mach-au1x00/au1xxx_psc.h>
#include <asm/mach-au1x00/au1xxx_dbdma.h>

#include "../codecs/ac97.h"
#include "psc.h"

static int au1xpsc_sample_ac97_init(struct snd_soc_codec *codec)
{
	snd_soc_dapm_sync(codec);
	return 0;
}

static struct snd_soc_dai_link au1xpsc_sample_ac97_dai = {
	.name		= "AC97",
	.stream_name	= "AC97 HiFi",
	.cpu_dai	= &au1xpsc_ac97_dai,	/* see psc-ac97.c */
	.codec_dai	= &ac97_dai,		/* see codecs/ac97.c */
	.init		= au1xpsc_sample_ac97_init,
	.ops		= NULL,
};

static struct snd_soc_card au1xpsc_sample_ac97_machine = {
	.name		= "Au1xxx PSC AC97 Audio",
	.dai_link	= &au1xpsc_sample_ac97_dai,
	.num_links	= 1,
};

static struct snd_soc_device au1xpsc_sample_ac97_devdata = {
	.card		= &au1xpsc_sample_ac97_machine,
	.platform	= &au1xpsc_soc_platform, /* see dbdma2.c */
	.codec_dev	= &soc_codec_dev_ac97,
};

static struct resource au1xpsc_psc1_res[] = {
	[0] = {
		.start	= CPHYSADDR(PSC1_BASE_ADDR),
		.end	= CPHYSADDR(PSC1_BASE_ADDR) + 0x000fffff,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
#ifdef CONFIG_SOC_AU1200
		.start	= AU1200_PSC1_INT,
		.end	= AU1200_PSC1_INT,
#elif defined(CONFIG_SOC_AU1550)
		.start	= AU1550_PSC1_INT,
		.end	= AU1550_PSC1_INT,
#endif
		.flags	= IORESOURCE_IRQ,
	},
	[2] = {
		.start	= DSCR_CMD0_PSC1_TX,
		.end	= DSCR_CMD0_PSC1_TX,
		.flags	= IORESOURCE_DMA,
	},
	[3] = {
		.start	= DSCR_CMD0_PSC1_RX,
		.end	= DSCR_CMD0_PSC1_RX,
		.flags	= IORESOURCE_DMA,
	},
};

static struct platform_device *au1xpsc_sample_ac97_dev;

static int __init au1xpsc_sample_ac97_load(void)
{
	int ret;

#ifdef CONFIG_SOC_AU1200
	unsigned long io;

	/* modify sys_pinfunc for AC97 on PSC1 */
	io = au_readl(SYS_PINFUNC);
	io |= SYS_PINFUNC_P1C;
	io &= ~(SYS_PINFUNC_P1A | SYS_PINFUNC_P1B);
	au_writel(io, SYS_PINFUNC);
	au_sync();
#endif

	ret = -ENOMEM;

	/* setup PSC clock source for AC97 part: external clock provided
	 * by codec.  The psc-ac97.c driver depends on this setting!
	 */
	au_writel(PSC_SEL_CLK_SERCLK, PSC1_BASE_ADDR + PSC_SEL_OFFSET);
	au_sync();

	au1xpsc_sample_ac97_dev = platform_device_alloc("soc-audio", -1);
	if (!au1xpsc_sample_ac97_dev)
		goto out;

	au1xpsc_sample_ac97_dev->resource =
		kmemdup(au1xpsc_psc1_res, sizeof(struct resource) *
			ARRAY_SIZE(au1xpsc_psc1_res), GFP_KERNEL);
	au1xpsc_sample_ac97_dev->num_resources = ARRAY_SIZE(au1xpsc_psc1_res);
	au1xpsc_sample_ac97_dev->id = 1;

	platform_set_drvdata(au1xpsc_sample_ac97_dev,
			     &au1xpsc_sample_ac97_devdata);
	au1xpsc_sample_ac97_devdata.dev = &au1xpsc_sample_ac97_dev->dev;
	ret = platform_device_add(au1xpsc_sample_ac97_dev);

	if (ret) {
		platform_device_put(au1xpsc_sample_ac97_dev);
		au1xpsc_sample_ac97_dev = NULL;
	}

out:
	return ret;
}

static void __exit au1xpsc_sample_ac97_exit(void)
{
	platform_device_unregister(au1xpsc_sample_ac97_dev);
}

module_init(au1xpsc_sample_ac97_load);
module_exit(au1xpsc_sample_ac97_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Au1xxx PSC sample AC97 machine");
MODULE_AUTHOR("Manuel Lauss <mano@roarinelk.homelinux.net>");
