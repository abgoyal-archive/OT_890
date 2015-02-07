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
 * Apple Onboard Audio driver for Toonie codec
 *
 * Copyright 2006 Johannes Berg <johannes@sipsolutions.net>
 *
 * GPL v2, can be found in COPYING.
 *
 *
 * This is a driver for the toonie codec chip. This chip is present
 * on the Mac Mini and is nothing but a DAC.
 */
#include <linux/delay.h>
#include <linux/module.h>
MODULE_AUTHOR("Johannes Berg <johannes@sipsolutions.net>");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("toonie codec driver for snd-aoa");

#include "../aoa.h"
#include "../soundbus/soundbus.h"


#define PFX "snd-aoa-codec-toonie: "

struct toonie {
	struct aoa_codec	codec;
};
#define codec_to_toonie(c) container_of(c, struct toonie, codec)

static int toonie_dev_register(struct snd_device *dev)
{
	return 0;
}

static struct snd_device_ops ops = {
	.dev_register = toonie_dev_register,
};

static struct transfer_info toonie_transfers[] = {
	/* This thing *only* has analog output,
	 * the rates are taken from Info.plist
	 * from Darwin. */
	{
		.formats = SNDRV_PCM_FMTBIT_S16_BE |
			   SNDRV_PCM_FMTBIT_S24_BE,
		.rates = SNDRV_PCM_RATE_32000 |
			 SNDRV_PCM_RATE_44100 |
			 SNDRV_PCM_RATE_48000 |
			 SNDRV_PCM_RATE_88200 |
			 SNDRV_PCM_RATE_96000,
	},
	{}
};

static int toonie_usable(struct codec_info_item *cii,
			 struct transfer_info *ti,
			 struct transfer_info *out)
{
	return 1;
}

#ifdef CONFIG_PM
static int toonie_suspend(struct codec_info_item *cii, pm_message_t state)
{
	/* can we turn it off somehow? */
	return 0;
}

static int toonie_resume(struct codec_info_item *cii)
{
	return 0;
}
#endif /* CONFIG_PM */

static struct codec_info toonie_codec_info = {
	.transfers = toonie_transfers,
	.sysclock_factor = 256,
	.bus_factor = 64,
	.owner = THIS_MODULE,
	.usable = toonie_usable,
#ifdef CONFIG_PM
	.suspend = toonie_suspend,
	.resume = toonie_resume,
#endif
};

static int toonie_init_codec(struct aoa_codec *codec)
{
	struct toonie *toonie = codec_to_toonie(codec);

	/* nothing connected? what a joke! */
	if (toonie->codec.connected != 1)
		return -ENOTCONN;

	if (aoa_snd_device_new(SNDRV_DEV_LOWLEVEL, toonie, &ops)) {
		printk(KERN_ERR PFX "failed to create toonie snd device!\n");
		return -ENODEV;
	}

	if (toonie->codec.soundbus_dev->attach_codec(toonie->codec.soundbus_dev,
						     aoa_get_card(),
						     &toonie_codec_info, toonie)) {
		printk(KERN_ERR PFX "error creating toonie pcm\n");
		snd_device_free(aoa_get_card(), toonie);
		return -ENODEV;
	}

	return 0;
}

static void toonie_exit_codec(struct aoa_codec *codec)
{
	struct toonie *toonie = codec_to_toonie(codec);

	if (!toonie->codec.soundbus_dev) {
		printk(KERN_ERR PFX "toonie_exit_codec called without soundbus_dev!\n");
		return;
	}
	toonie->codec.soundbus_dev->detach_codec(toonie->codec.soundbus_dev, toonie);
}

static struct toonie *toonie;

static int __init toonie_init(void)
{
	toonie = kzalloc(sizeof(struct toonie), GFP_KERNEL);

	if (!toonie)
		return -ENOMEM;

	strlcpy(toonie->codec.name, "toonie", sizeof(toonie->codec.name));
	toonie->codec.owner = THIS_MODULE;
	toonie->codec.init = toonie_init_codec;
	toonie->codec.exit = toonie_exit_codec;

	if (aoa_codec_register(&toonie->codec)) {
		kfree(toonie);
		return -EINVAL;
	}

	return 0;
}

static void __exit toonie_exit(void)
{
	aoa_codec_unregister(&toonie->codec);
	kfree(toonie);
}

module_init(toonie_init);
module_exit(toonie_exit);
