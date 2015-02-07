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
 *  linux/arch/arm/plat-mxc/include/mach/dma-mx1-mx2.h
 *
 *  i.MX DMA registration and IRQ dispatching
 *
 * Copyright 2006 Pavel Pisa <pisa@cmp.felk.cvut.cz>
 * Copyright 2008 Juergen Beisert, <kernel@pengutronix.de>
 * Copyright 2008 Sascha Hauer, <s.hauer@pengutronix.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

#ifndef __ASM_ARCH_MXC_DMA_H
#define __ASM_ARCH_MXC_DMA_H

#define IMX_DMA_CHANNELS  16

#define DMA_MODE_READ		0
#define DMA_MODE_WRITE		1
#define DMA_MODE_MASK		1

#define DMA_BASE IO_ADDRESS(DMA_BASE_ADDR)

#define IMX_DMA_MEMSIZE_32	(0 << 4)
#define IMX_DMA_MEMSIZE_8	(1 << 4)
#define IMX_DMA_MEMSIZE_16	(2 << 4)
#define IMX_DMA_TYPE_LINEAR	(0 << 10)
#define IMX_DMA_TYPE_2D		(1 << 10)
#define IMX_DMA_TYPE_FIFO	(2 << 10)

#define IMX_DMA_ERR_BURST     (1 << 0)
#define IMX_DMA_ERR_REQUEST   (1 << 1)
#define IMX_DMA_ERR_TRANSFER  (1 << 2)
#define IMX_DMA_ERR_BUFFER    (1 << 3)
#define IMX_DMA_ERR_TIMEOUT   (1 << 4)

int
imx_dma_config_channel(int channel, unsigned int config_port,
	unsigned int config_mem, unsigned int dmareq, int hw_chaining);

void
imx_dma_config_burstlen(int channel, unsigned int burstlen);

int
imx_dma_setup_single(int channel, dma_addr_t dma_address,
		unsigned int dma_length, unsigned int dev_addr,
		unsigned int dmamode);

int
imx_dma_setup_sg(int channel, struct scatterlist *sg,
		unsigned int sgcount, unsigned int dma_length,
		unsigned int dev_addr, unsigned int dmamode);

int
imx_dma_setup_handlers(int channel,
		void (*irq_handler) (int, void *),
		void (*err_handler) (int, void *, int), void *data);

int
imx_dma_setup_progression_handler(int channel,
		void (*prog_handler) (int, void*, struct scatterlist*));

void imx_dma_enable(int channel);

void imx_dma_disable(int channel);

int imx_dma_request(int channel, const char *name);

void imx_dma_free(int channel);

enum imx_dma_prio {
	DMA_PRIO_HIGH = 0,
	DMA_PRIO_MEDIUM = 1,
	DMA_PRIO_LOW = 2
};

int imx_dma_request_by_prio(const char *name, enum imx_dma_prio prio);

#endif	/* _ASM_ARCH_MXC_DMA_H */
