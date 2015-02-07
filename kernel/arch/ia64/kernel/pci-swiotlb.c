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

/* Glue code to lib/swiotlb.c */

#include <linux/pci.h>
#include <linux/cache.h>
#include <linux/module.h>
#include <linux/dma-mapping.h>

#include <asm/swiotlb.h>
#include <asm/dma.h>
#include <asm/iommu.h>
#include <asm/machvec.h>

int swiotlb __read_mostly;
EXPORT_SYMBOL(swiotlb);

struct dma_mapping_ops swiotlb_dma_ops = {
	.mapping_error = swiotlb_dma_mapping_error,
	.alloc_coherent = swiotlb_alloc_coherent,
	.free_coherent = swiotlb_free_coherent,
	.map_single = swiotlb_map_single,
	.unmap_single = swiotlb_unmap_single,
	.sync_single_for_cpu = swiotlb_sync_single_for_cpu,
	.sync_single_for_device = swiotlb_sync_single_for_device,
	.sync_single_range_for_cpu = swiotlb_sync_single_range_for_cpu,
	.sync_single_range_for_device = swiotlb_sync_single_range_for_device,
	.sync_sg_for_cpu = swiotlb_sync_sg_for_cpu,
	.sync_sg_for_device = swiotlb_sync_sg_for_device,
	.map_sg = swiotlb_map_sg,
	.unmap_sg = swiotlb_unmap_sg,
	.dma_supported_op = swiotlb_dma_supported,
};

void __init pci_swiotlb_init(void)
{
	if (!iommu_detected) {
#ifdef CONFIG_IA64_GENERIC
		swiotlb = 1;
		printk(KERN_INFO "PCI-DMA: Re-initialize machine vector.\n");
		machvec_init("dig");
		swiotlb_init();
		dma_ops = &swiotlb_dma_ops;
#else
		panic("Unable to find Intel IOMMU");
#endif
	}
}
