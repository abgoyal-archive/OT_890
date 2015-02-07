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
 * Copyright 2002 Integrated Device Technology, Inc.
 *		All rights reserved.
 *
 * DMA register definition.
 *
 * Author : ryan.holmQVist@idt.com
 * Date   : 20011005
 */

#ifndef _ASM_RC32434_DMA_V_H_
#define _ASM_RC32434_DMA_V_H_

#include  <asm/mach-rc32434/dma.h>
#include  <asm/mach-rc32434/rc32434.h>

#define DMA_CHAN_OFFSET		0x14
#define IS_DMA_USED(X)		(((X) & \
				(DMA_DESC_FINI | DMA_DESC_DONE | DMA_DESC_TERM)) \
				!= 0)
#define DMA_COUNT(count)	((count) & DMA_DESC_COUNT_MSK)

#define DMA_HALT_TIMEOUT	500

static inline int rc32434_halt_dma(struct dma_reg *ch)
{
	int timeout = 1;
	if (__raw_readl(&ch->dmac) & DMA_CHAN_RUN_BIT) {
		__raw_writel(0, &ch->dmac);
		for (timeout = DMA_HALT_TIMEOUT; timeout > 0; timeout--) {
			if (__raw_readl(&ch->dmas) & DMA_STAT_HALT) {
				__raw_writel(0, &ch->dmas);
				break;
			}
		}
	}

	return timeout ? 0 : 1;
}

static inline void rc32434_start_dma(struct dma_reg *ch, u32 dma_addr)
{
	__raw_writel(0, &ch->dmandptr);
	__raw_writel(dma_addr, &ch->dmadptr);
}

static inline void rc32434_chain_dma(struct dma_reg *ch, u32 dma_addr)
{
	__raw_writel(dma_addr, &ch->dmandptr);
}

#endif  /* _ASM_RC32434_DMA_V_H_ */
