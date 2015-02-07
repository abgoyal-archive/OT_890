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
 *  linux/sound/arm/devdma.c
 *
 *  Copyright (C) 2003-2004 Russell King, All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  ARM DMA shim for ALSA.
 */
#include <linux/device.h>
#include <linux/dma-mapping.h>

#include <sound/core.h>
#include <sound/pcm.h>

#include "devdma.h"

void devdma_hw_free(struct device *dev, struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct snd_dma_buffer *buf = runtime->dma_buffer_p;

	if (runtime->dma_area == NULL)
		return;

	if (buf != &substream->dma_buffer) {
		dma_free_coherent(buf->dev.dev, buf->bytes, buf->area, buf->addr);
		kfree(runtime->dma_buffer_p);
	}

	snd_pcm_set_runtime_buffer(substream, NULL);
}

int devdma_hw_alloc(struct device *dev, struct snd_pcm_substream *substream, size_t size)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct snd_dma_buffer *buf = runtime->dma_buffer_p;
	int ret = 0;

	if (buf) {
		if (buf->bytes >= size)
			goto out;
		devdma_hw_free(dev, substream);
	}

	if (substream->dma_buffer.area != NULL && substream->dma_buffer.bytes >= size) {
		buf = &substream->dma_buffer;
	} else {
		buf = kmalloc(sizeof(struct snd_dma_buffer), GFP_KERNEL);
		if (!buf)
			goto nomem;

		buf->dev.type = SNDRV_DMA_TYPE_DEV;
		buf->dev.dev = dev;
		buf->area = dma_alloc_coherent(dev, size, &buf->addr, GFP_KERNEL);
		buf->bytes = size;
		buf->private_data = NULL;

		if (!buf->area)
			goto free;
	}
	snd_pcm_set_runtime_buffer(substream, buf);
	ret = 1;
 out:
	runtime->dma_bytes = size;
	return ret;

 free:
	kfree(buf);
 nomem:
	return -ENOMEM;
}

int devdma_mmap(struct device *dev, struct snd_pcm_substream *substream, struct vm_area_struct *vma)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	return dma_mmap_coherent(dev, vma, runtime->dma_area, runtime->dma_addr, runtime->dma_bytes);
}
