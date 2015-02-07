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

#ifndef PXA2XX_LIB_H
#define PXA2XX_LIB_H

#include <linux/platform_device.h>
#include <sound/ac97_codec.h>

/* PCM */

struct pxa2xx_pcm_dma_params {
	char *name;			/* stream identifier */
	u32 dcmd;			/* DMA descriptor dcmd field */
	volatile u32 *drcmr;		/* the DMA request channel to use */
	u32 dev_addr;			/* device physical address for DMA */
};

extern int __pxa2xx_pcm_hw_params(struct snd_pcm_substream *substream,
				struct snd_pcm_hw_params *params);
extern int __pxa2xx_pcm_hw_free(struct snd_pcm_substream *substream);
extern int pxa2xx_pcm_trigger(struct snd_pcm_substream *substream, int cmd);
extern snd_pcm_uframes_t pxa2xx_pcm_pointer(struct snd_pcm_substream *substream);
extern int __pxa2xx_pcm_prepare(struct snd_pcm_substream *substream);
extern void pxa2xx_pcm_dma_irq(int dma_ch, void *dev_id);
extern int __pxa2xx_pcm_open(struct snd_pcm_substream *substream);
extern int __pxa2xx_pcm_close(struct snd_pcm_substream *substream);
extern int pxa2xx_pcm_mmap(struct snd_pcm_substream *substream,
	struct vm_area_struct *vma);
extern int pxa2xx_pcm_preallocate_dma_buffer(struct snd_pcm *pcm, int stream);
extern void pxa2xx_pcm_free_dma_buffers(struct snd_pcm *pcm);

/* AC97 */

extern unsigned short pxa2xx_ac97_read(struct snd_ac97 *ac97, unsigned short reg);
extern void pxa2xx_ac97_write(struct snd_ac97 *ac97, unsigned short reg, unsigned short val);

extern bool pxa2xx_ac97_try_warm_reset(struct snd_ac97 *ac97);
extern bool pxa2xx_ac97_try_cold_reset(struct snd_ac97 *ac97);
extern void pxa2xx_ac97_finish_reset(struct snd_ac97 *ac97);

extern int pxa2xx_ac97_hw_suspend(void);
extern int pxa2xx_ac97_hw_resume(void);

extern int pxa2xx_ac97_hw_probe(struct platform_device *dev);
extern void pxa2xx_ac97_hw_remove(struct platform_device *dev);

#endif
