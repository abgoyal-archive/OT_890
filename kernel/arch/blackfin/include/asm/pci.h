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

/* Changed from asm-m68k version, Lineo Inc. 	May 2001	*/

#ifndef _ASM_BFIN_PCI_H
#define _ASM_BFIN_PCI_H

#include <asm/scatterlist.h>

/*
 *
 * Written by Wout Klaren.
 */

/* Added by Chang Junxiao */
#define PCIBIOS_MIN_IO 0x00001000
#define PCIBIOS_MIN_MEM 0x10000000

#define PCI_DMA_BUS_IS_PHYS       (1)
struct pci_ops;

/*
 * Structure with hardware dependent information and functions of the
 * PCI bus.
 */
struct pci_bus_info {

	/*
	 * Resources of the PCI bus.
	 */
	struct resource mem_space;
	struct resource io_space;

	/*
	 * System dependent functions.
	 */
	struct pci_ops *bfin_pci_ops;
	void (*fixup) (int pci_modify);
	void (*conf_device) (unsigned char bus, unsigned char device_fn);
};

#define pcibios_assign_all_busses()	0
static inline void pcibios_set_master(struct pci_dev *dev)
{

	/* No special bus mastering setup handling */
}
static inline void pcibios_penalize_isa_irq(int irq)
{

	/* We don't do dynamic PCI IRQ allocation */
}
static inline dma_addr_t pci_map_single(struct pci_dev *hwdev, void *ptr,
					size_t size, int direction)
{
	if (direction == PCI_DMA_NONE)
		BUG();

	 /* return virt_to_bus(ptr); */
	return (dma_addr_t) ptr;
}

/* Unmap a single streaming mode DMA translation.  The dma_addr and size
 * must match what was provided for in a previous pci_map_single call.  All
 * other usages are undefined.
 *
 * After this call, reads by the cpu to the buffer are guarenteed to see
 * whatever the device wrote there.
 */
static inline void pci_unmap_single(struct pci_dev *hwdev, dma_addr_t dma_addr,
				    size_t size, int direction)
{
	if (direction == PCI_DMA_NONE)
		BUG();

	/* Nothing to do */
}

/* Map a set of buffers described by scatterlist in streaming
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
static inline int pci_map_sg(struct pci_dev *hwdev, struct scatterlist *sg,
			     int nents, int direction)
{
	if (direction == PCI_DMA_NONE)
		BUG();
	return nents;
}

/* Unmap a set of streaming mode DMA translations.
 * Again, cpu read rules concerning calls here are the same as for
 * pci_unmap_single() above.
 */
static inline void pci_unmap_sg(struct pci_dev *hwdev, struct scatterlist *sg,
				int nents, int direction)
{
	if (direction == PCI_DMA_NONE)
		BUG();

	/* Nothing to do */
}

/* Make physical memory consistent for a single
 * streaming mode DMA translation after a transfer.
 *
 * If you perform a pci_map_single() but wish to interrogate the
 * buffer using the cpu, yet do not wish to teardown the PCI dma
 * mapping, you must call this function before doing so.  At the
 * next point you give the PCI dma address back to the card, the
 * device again owns the buffer.
 */
static inline void pci_dma_sync_single(struct pci_dev *hwdev,
				       dma_addr_t dma_handle, size_t size,
				       int direction)
{
	if (direction == PCI_DMA_NONE)
		BUG();

	/* Nothing to do */
}

/* Make physical memory consistent for a set of streaming
 * mode DMA translations after a transfer.
 *
 * The same as pci_dma_sync_single but for a scatter-gather list,
 * same rules and usage.
 */
static inline void pci_dma_sync_sg(struct pci_dev *hwdev,
				   struct scatterlist *sg, int nelems,
				   int direction)
{
	if (direction == PCI_DMA_NONE)
		BUG();

	/* Nothing to do */
}

#endif				/* _ASM_BFIN_PCI_H */
