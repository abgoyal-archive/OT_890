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

/* arch/arm/mach-s3c2410/include/mach/audio.h
 *
 * Copyright (c) 2004-2005 Simtec Electronics
 *	http://www.simtec.co.uk/products/SWLINUX/
 *	Ben Dooks <ben@simtec.co.uk>
 *
 * S3C24XX - Audio platfrom_device info
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#ifndef __ASM_ARCH_AUDIO_H
#define __ASM_ARCH_AUDIO_H __FILE__

/* struct s3c24xx_iis_ops
 *
 * called from the s3c24xx audio core to deal with the architecture
 * or the codec's setup and control.
 *
 * the pointer to itself is passed through in case the caller wants to
 * embed this in an larger structure for easy reference to it's context.
*/

struct s3c24xx_iis_ops {
	struct module *owner;

	int	(*startup)(struct s3c24xx_iis_ops *me);
	void	(*shutdown)(struct s3c24xx_iis_ops *me);
	int	(*suspend)(struct s3c24xx_iis_ops *me);
	int	(*resume)(struct s3c24xx_iis_ops *me);

	int	(*open)(struct s3c24xx_iis_ops *me, struct snd_pcm_substream *strm);
	int	(*close)(struct s3c24xx_iis_ops *me, struct snd_pcm_substream *strm);
	int	(*prepare)(struct s3c24xx_iis_ops *me, struct snd_pcm_substream *strm, struct snd_pcm_runtime *rt);
};

struct s3c24xx_platdata_iis {
	const char		*codec_clk;
	struct s3c24xx_iis_ops	*ops;
	int			(*match_dev)(struct device *dev);
};

#endif /* __ASM_ARCH_AUDIO_H */
