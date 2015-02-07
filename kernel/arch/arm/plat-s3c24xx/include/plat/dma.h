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

/* linux/include/asm-arm/plat-s3c24xx/dma.h
 *
 * Copyright (C) 2006 Simtec Electronics
 *	Ben Dooks <ben@simtec.co.uk>
 *
 * Samsung S3C24XX DMA support
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

extern struct sysdev_class dma_sysclass;
extern struct s3c2410_dma_chan s3c2410_chans[S3C2410_DMA_CHANNELS];

#define DMA_CH_VALID		(1<<31)
#define DMA_CH_NEVER		(1<<30)

struct s3c24xx_dma_addr {
	unsigned long		from;
	unsigned long		to;
};

/* struct s3c24xx_dma_map
 *
 * this holds the mapping information for the channel selected
 * to be connected to the specified device
*/

struct s3c24xx_dma_map {
	const char		*name;
	struct s3c24xx_dma_addr  hw_addr;

	unsigned long		 channels[S3C2410_DMA_CHANNELS];
	unsigned long		 channels_rx[S3C2410_DMA_CHANNELS];
};

struct s3c24xx_dma_selection {
	struct s3c24xx_dma_map	*map;
	unsigned long		 map_size;
	unsigned long		 dcon_mask;

	void	(*select)(struct s3c2410_dma_chan *chan,
			  struct s3c24xx_dma_map *map);

	void	(*direction)(struct s3c2410_dma_chan *chan,
			     struct s3c24xx_dma_map *map,
			     enum s3c2410_dmasrc dir);
};

extern int s3c24xx_dma_init_map(struct s3c24xx_dma_selection *sel);

/* struct s3c24xx_dma_order_ch
 *
 * channel map for one of the `enum dma_ch` dma channels. the list
 * entry contains a set of low-level channel numbers, orred with
 * DMA_CH_VALID, which are checked in the order in the array.
*/

struct s3c24xx_dma_order_ch {
	unsigned int	list[S3C2410_DMA_CHANNELS];	/* list of channels */
	unsigned int	flags;				/* flags */
};

/* struct s3c24xx_dma_order
 *
 * information provided by either the core or the board to give the
 * dma system a hint on how to allocate channels
*/

struct s3c24xx_dma_order {
	struct s3c24xx_dma_order_ch	channels[DMACH_MAX];
};

extern int s3c24xx_dma_order_set(struct s3c24xx_dma_order *map);

/* DMA init code, called from the cpu support code */

extern int s3c2410_dma_init(void);

extern int s3c24xx_dma_init(unsigned int channels, unsigned int irq,
			    unsigned int stride);
