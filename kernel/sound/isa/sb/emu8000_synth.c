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
 *  Copyright (c) by Jaroslav Kysela <perex@perex.cz>
 *     and (c) 1999 Steve Ratcliffe <steve@parabola.demon.co.uk>
 *  Copyright (C) 1999-2000 Takashi Iwai <tiwai@suse.de>
 *
 *  Emu8000 synth plug-in routine
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

#include "emu8000_local.h"
#include <linux/init.h>
#include <sound/initval.h>

MODULE_AUTHOR("Takashi Iwai, Steve Ratcliffe");
MODULE_DESCRIPTION("Emu8000 synth plug-in routine");
MODULE_LICENSE("GPL");

/*----------------------------------------------------------------*/

/*
 * create a new hardware dependent device for Emu8000
 */
static int snd_emu8000_new_device(struct snd_seq_device *dev)
{
	struct snd_emu8000 *hw;
	struct snd_emux *emu;

	hw = *(struct snd_emu8000**)SNDRV_SEQ_DEVICE_ARGPTR(dev);
	if (hw == NULL)
		return -EINVAL;

	if (hw->emu)
		return -EBUSY; /* already exists..? */

	if (snd_emux_new(&emu) < 0)
		return -ENOMEM;

	hw->emu = emu;
	snd_emu8000_ops_setup(hw);

	emu->hw = hw;
	emu->max_voices = EMU8000_DRAM_VOICES;
	emu->num_ports = hw->seq_ports;

	if (hw->memhdr) {
		snd_printk(KERN_ERR "memhdr is already initialized!?\n");
		snd_util_memhdr_free(hw->memhdr);
	}
	hw->memhdr = snd_util_memhdr_new(hw->mem_size);
	if (hw->memhdr == NULL) {
		snd_emux_free(emu);
		hw->emu = NULL;
		return -ENOMEM;
	}

	emu->memhdr = hw->memhdr;
	emu->midi_ports = hw->seq_ports < 2 ? hw->seq_ports : 2; /* number of virmidi ports */
	emu->midi_devidx = 1;
	emu->linear_panning = 1;
	emu->hwdep_idx = 2; /* FIXED */

	if (snd_emux_register(emu, dev->card, hw->index, "Emu8000") < 0) {
		snd_emux_free(emu);
		snd_util_memhdr_free(hw->memhdr);
		hw->emu = NULL;
		hw->memhdr = NULL;
		return -ENOMEM;
	}

	if (hw->mem_size > 0)
		snd_emu8000_pcm_new(dev->card, hw, 1);

	dev->driver_data = hw;

	return 0;
}


/*
 * free all resources
 */
static int snd_emu8000_delete_device(struct snd_seq_device *dev)
{
	struct snd_emu8000 *hw;

	if (dev->driver_data == NULL)
		return 0; /* no synth was allocated actually */

	hw = dev->driver_data;
	if (hw->pcm)
		snd_device_free(dev->card, hw->pcm);
	if (hw->emu)
		snd_emux_free(hw->emu);
	if (hw->memhdr)
		snd_util_memhdr_free(hw->memhdr);
	hw->emu = NULL;
	hw->memhdr = NULL;
	return 0;
}

/*
 *  INIT part
 */

static int __init alsa_emu8000_init(void)
{
	
	static struct snd_seq_dev_ops ops = {
		snd_emu8000_new_device,
		snd_emu8000_delete_device,
	};
	return snd_seq_device_register_driver(SNDRV_SEQ_DEV_ID_EMU8000, &ops,
					      sizeof(struct snd_emu8000*));
}

static void __exit alsa_emu8000_exit(void)
{
	snd_seq_device_unregister_driver(SNDRV_SEQ_DEV_ID_EMU8000);
}

module_init(alsa_emu8000_init)
module_exit(alsa_emu8000_exit)
