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
 *  linux/arch/arm/kernel/dma-ebsa285.c
 *
 *  Copyright (C) 1998 Phil Blundell
 *
 * DMA functions specific to EBSA-285/CATS architectures
 *
 *  Changelog:
 *   09-Nov-1998 RMK	Split out ISA DMA functions to dma-isa.c
 *   17-Mar-1999 RMK	Allow any EBSA285-like architecture to have
 *			ISA DMA controllers.
 */
#include <linux/init.h>
#include <linux/io.h>
#include <linux/spinlock.h>

#include <asm/dma.h>
#include <asm/scatterlist.h>

#include <asm/mach/dma.h>
#include <asm/hardware/dec21285.h>

#if 0
static int fb_dma_request(dmach_t channel, dma_t *dma)
{
	return -EINVAL;
}

static void fb_dma_enable(dmach_t channel, dma_t *dma)
{
}

static void fb_dma_disable(dmach_t channel, dma_t *dma)
{
}

static struct dma_ops fb_dma_ops = {
	.type		= "fb",
	.request	= fb_dma_request,
	.enable		= fb_dma_enable,
	.disable	= fb_dma_disable,
};
#endif

void __init arch_dma_init(dma_t *dma)
{
#if 0
	dma[_DC21285_DMA(0)].d_ops = &fb_dma_ops;
	dma[_DC21285_DMA(1)].d_ops = &fb_dma_ops;
#endif
#ifdef CONFIG_ISA_DMA
	if (footbridge_cfn_mode())
		isa_init_dma(dma + _ISA_DMA(0));
#endif
}
