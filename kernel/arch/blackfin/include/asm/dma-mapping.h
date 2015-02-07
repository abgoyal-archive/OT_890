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

#ifndef _BLACKFIN_DMA_MAPPING_H
#define _BLACKFIN_DMA_MAPPING_H

#include <asm/scatterlist.h>

void dma_alloc_init(unsigned long start, unsigned long end);
void *dma_alloc_coherent(struct device *dev, size_t size,
			 dma_addr_t *dma_handle, gfp_t gfp);
void dma_free_coherent(struct device *dev, size_t size, void *vaddr,
		       dma_addr_t dma_handle);

/*
 * Now for the API extensions over the pci_ one
 */
#define dma_alloc_noncoherent(d, s, h, f) dma_alloc_coherent(d, s, h, f)
#define dma_free_noncoherent(d, s, v, h) dma_free_coherent(d, s, v, h)

static inline
int dma_mapping_error(struct device *dev, dma_addr_t dma_addr)
{
	return 0;
}

/*
 * Map a single buffer of the indicated size for DMA in streaming mode.
 * The 32-bit bus address to use is returned.
 *
 * Once the device is given the dma address, the device owns this memory
 * until either pci_unmap_single or pci_dma_sync_single is performed.
 */
extern dma_addr_t dma_map_single(struct device *dev, void *ptr, size_t size,
				 enum dma_data_direction direction);

static inline dma_addr_t
dma_map_page(struct device *dev, struct page *page,
	     unsigned long offset, size_t size,
	     enum dma_data_direction dir)
{
	return dma_map_single(dev, page_address(page) + offset, size, dir);
}

/*
 * Unmap a single streaming mode DMA translation.  The dma_addr and size
 * must match what was provided for in a previous pci_map_single call.  All
 * other usages are undefined.
 *
 * After this call, reads by the cpu to the buffer are guarenteed to see
 * whatever the device wrote there.
 */
extern void dma_unmap_single(struct device *dev, dma_addr_t dma_addr, size_t size,
			  enum dma_data_direction direction);

static inline void
dma_unmap_page(struct device *dev, dma_addr_t dma_addr, size_t size,
	       enum dma_data_direction dir)
{
	dma_unmap_single(dev, dma_addr, size, dir);
}

/*
 * Map a set of buffers described by scatterlist in streaming
 * mode for DMA.  This is the scather-gather version of the
 * above pci_map_single interface.  Here the scatter gather list
 * elements are each tagged with the appropriate dma address
 * and length.  They are obtained via sg_dma_{address,length}(SG).
 *
 * NOTE: An implementation may be able to use a smaller number of
 *       DMA address/length pairs than there are SG table elements.
 *       (for example via virtual mapping capabilities)
 *       The routine returns the number of addr/length pairs actually
 *       used, at most nents.
 *
 * Device ownership issues as mentioned above for pci_map_single are
 * the same here.
 */
extern int dma_map_sg(struct device *dev, struct scatterlist *sg, int nents,
		      enum dma_data_direction direction);

/*
 * Unmap a set of streaming mode DMA translations.
 * Again, cpu read rules concerning calls here are the same as for
 * pci_unmap_single() above.
 */
extern void dma_unmap_sg(struct device *dev, struct scatterlist *sg,
		      int nhwentries, enum dma_data_direction direction);

static inline void dma_sync_single_for_cpu(struct device *dev,
					dma_addr_t handle, size_t size,
					enum dma_data_direction dir)
{
}

static inline void dma_sync_single_for_device(struct device *dev,
					dma_addr_t handle, size_t size,
					enum dma_data_direction dir)
{
}
#endif				/* _BLACKFIN_DMA_MAPPING_H */
