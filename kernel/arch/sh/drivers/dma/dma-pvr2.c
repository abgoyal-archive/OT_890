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
 * arch/sh/drivers/dma/dma-pvr2.c
 *
 * NEC PowerVR 2 (Dreamcast) DMA support
 *
 * Copyright (C) 2003, 2004  Paul Mundt
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <mach/sysasic.h>
#include <mach/dma.h>
#include <asm/dma.h>
#include <asm/io.h>

static unsigned int xfer_complete;
static int count;

static irqreturn_t pvr2_dma_interrupt(int irq, void *dev_id)
{
	if (get_dma_residue(PVR2_CASCADE_CHAN)) {
		printk(KERN_WARNING "DMA: SH DMAC did not complete transfer "
		       "on channel %d, waiting..\n", PVR2_CASCADE_CHAN);
		dma_wait_for_completion(PVR2_CASCADE_CHAN);
	}

	if (count++ < 10)
		pr_debug("Got a pvr2 dma interrupt for channel %d\n",
			 irq - HW_EVENT_PVR2_DMA);

	xfer_complete = 1;

	return IRQ_HANDLED;
}

static int pvr2_request_dma(struct dma_channel *chan)
{
	if (ctrl_inl(PVR2_DMA_MODE) != 0)
		return -EBUSY;

	ctrl_outl(0, PVR2_DMA_LMMODE0);

	return 0;
}

static int pvr2_get_dma_residue(struct dma_channel *chan)
{
	return xfer_complete == 0;
}

static int pvr2_xfer_dma(struct dma_channel *chan)
{
	if (chan->sar || !chan->dar)
		return -EINVAL;

	xfer_complete = 0;

	ctrl_outl(chan->dar, PVR2_DMA_ADDR);
	ctrl_outl(chan->count, PVR2_DMA_COUNT);
	ctrl_outl(chan->mode & DMA_MODE_MASK, PVR2_DMA_MODE);

	return 0;
}

static struct irqaction pvr2_dma_irq = {
	.name		= "pvr2 DMA handler",
	.handler	= pvr2_dma_interrupt,
	.flags		= IRQF_DISABLED,
};

static struct dma_ops pvr2_dma_ops = {
	.request	= pvr2_request_dma,
	.get_residue	= pvr2_get_dma_residue,
	.xfer		= pvr2_xfer_dma,
};

static struct dma_info pvr2_dma_info = {
	.name		= "pvr2_dmac",
	.nr_channels	= 1,
	.ops		= &pvr2_dma_ops,
	.flags		= DMAC_CHANNELS_TEI_CAPABLE,
};

static int __init pvr2_dma_init(void)
{
	setup_irq(HW_EVENT_PVR2_DMA, &pvr2_dma_irq);
	request_dma(PVR2_CASCADE_CHAN, "pvr2 cascade");

	return register_dmac(&pvr2_dma_info);
}

static void __exit pvr2_dma_exit(void)
{
	free_dma(PVR2_CASCADE_CHAN);
	free_irq(HW_EVENT_PVR2_DMA, 0);
	unregister_dmac(&pvr2_dma_info);
}

subsys_initcall(pvr2_dma_init);
module_exit(pvr2_dma_exit);

MODULE_AUTHOR("Paul Mundt <lethal@linux-sh.org>");
MODULE_DESCRIPTION("NEC PowerVR 2 DMA driver");
MODULE_LICENSE("GPL");
