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
 * Dynamic DMA mapping support.
 */

#include <linux/types.h>
#include <linux/mm.h>
#include <linux/string.h>
#include <linux/pci.h>
#include <linux/module.h>
#include <linux/dmar.h>
#include <asm/iommu.h>
#include <asm/machvec.h>
#include <linux/dma-mapping.h>

#include <asm/system.h>

#ifdef CONFIG_DMAR

#include <linux/kernel.h>

#include <asm/page.h>

dma_addr_t bad_dma_address __read_mostly;
EXPORT_SYMBOL(bad_dma_address);

static int iommu_sac_force __read_mostly;

int no_iommu __read_mostly;
#ifdef CONFIG_IOMMU_DEBUG
int force_iommu __read_mostly = 1;
#else
int force_iommu __read_mostly;
#endif

/* Set this to 1 if there is a HW IOMMU in the system */
int iommu_detected __read_mostly;

/* Dummy device used for NULL arguments (normally ISA). Better would
   be probably a smaller DMA mask, but this is bug-to-bug compatible
   to i386. */
struct device fallback_dev = {
	.init_name = "fallback device",
	.coherent_dma_mask = DMA_32BIT_MASK,
	.dma_mask = &fallback_dev.coherent_dma_mask,
};

void __init pci_iommu_alloc(void)
{
	/*
	 * The order of these functions is important for
	 * fall-back/fail-over reasons
	 */
	detect_intel_iommu();

#ifdef CONFIG_SWIOTLB
	pci_swiotlb_init();
#endif
}

static int __init pci_iommu_init(void)
{
	if (iommu_detected)
		intel_iommu_init();

	return 0;
}

/* Must execute after PCI subsystem */
fs_initcall(pci_iommu_init);

void pci_iommu_shutdown(void)
{
	return;
}

void __init
iommu_dma_init(void)
{
	return;
}

struct dma_mapping_ops *dma_ops;
EXPORT_SYMBOL(dma_ops);

int iommu_dma_supported(struct device *dev, u64 mask)
{
	struct dma_mapping_ops *ops = get_dma_ops(dev);

	if (ops->dma_supported_op)
		return ops->dma_supported_op(dev, mask);

	/* Copied from i386. Doesn't make much sense, because it will
	   only work for pci_alloc_coherent.
	   The caller just has to use GFP_DMA in this case. */
	if (mask < DMA_24BIT_MASK)
		return 0;

	/* Tell the device to use SAC when IOMMU force is on.  This
	   allows the driver to use cheaper accesses in some cases.

	   Problem with this is that if we overflow the IOMMU area and
	   return DAC as fallback address the device may not handle it
	   correctly.

	   As a special case some controllers have a 39bit address
	   mode that is as efficient as 32bit (aic79xx). Don't force
	   SAC for these.  Assume all masks <= 40 bits are of this
	   type. Normally this doesn't make any difference, but gives
	   more gentle handling of IOMMU overflow. */
	if (iommu_sac_force && (mask >= DMA_40BIT_MASK)) {
		dev_info(dev, "Force SAC with mask %lx\n", mask);
		return 0;
	}

	return 1;
}
EXPORT_SYMBOL(iommu_dma_supported);

#endif
