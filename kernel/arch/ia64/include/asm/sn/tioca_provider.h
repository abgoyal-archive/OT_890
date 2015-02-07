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
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (c) 2003-2005 Silicon Graphics, Inc. All rights reserved.
 */

#ifndef _ASM_IA64_SN_TIO_CA_AGP_PROVIDER_H
#define _ASM_IA64_SN_TIO_CA_AGP_PROVIDER_H

#include <asm/sn/tioca.h>

/*
 * WAR enables
 * Defines for individual WARs. Each is a bitmask of applicable
 * part revision numbers. (1 << 1) == rev A, (1 << 2) == rev B,
 * (3 << 1) == (rev A or rev B), etc
 */

#define TIOCA_WAR_ENABLED(pv, tioca_common) \
	((1 << tioca_common->ca_rev) & pv)

  /* TIO:ICE:FRZ:Freezer loses a PIO data ucred on PIO RD RSP with CW error */
#define PV907908 (1 << 1)
  /* ATI config space problems after BIOS execution starts */
#define PV908234 (1 << 1)
  /* CA:AGPDMA write request data mismatch with ABC1CL merge */
#define PV895469 (1 << 1)
  /* TIO:CA TLB invalidate of written GART entries possibly not occurring in CA*/
#define PV910244 (1 << 1)

struct tioca_dmamap{
	struct list_head	cad_list;	/* headed by ca_list */

	dma_addr_t		cad_dma_addr;	/* Linux dma handle */
	uint			cad_gart_entry; /* start entry in ca_gart_pagemap */
	uint			cad_gart_size;	/* #entries for this map */
};

/*
 * Kernel only fields.  Prom may look at this stuff for debugging only.
 * Access this structure through the ca_kernel_private ptr.
 */

struct tioca_common ;

struct tioca_kernel {
	struct tioca_common	*ca_common;	/* tioca this belongs to */
	struct list_head	ca_list;	/* list of all ca's */
	struct list_head	ca_dmamaps;
	spinlock_t		ca_lock;	/* Kernel lock */
	cnodeid_t		ca_closest_node;
	struct list_head	*ca_devices;	/* bus->devices */

	/*
	 * General GART stuff
	 */
	u64	ca_ap_size;		/* size of aperature in bytes */
	u32	ca_gart_entries;	/* # u64 entries in gart */
	u32	ca_ap_pagesize; 	/* aperature page size in bytes */
	u64	ca_ap_bus_base; 	/* bus address of CA aperature */
	u64	ca_gart_size;		/* gart size in bytes */
	u64	*ca_gart;		/* gart table vaddr */
	u64	ca_gart_coretalk_addr;	/* gart coretalk addr */
	u8		ca_gart_iscoherent;	/* used in tioca_tlbflush */

	/* PCI GART convenience values */
	u64	ca_pciap_base;		/* pci aperature bus base address */
	u64	ca_pciap_size;		/* pci aperature size (bytes) */
	u64	ca_pcigart_base;	/* gfx GART bus base address */
	u64	*ca_pcigart;		/* gfx GART vm address */
	u32	ca_pcigart_entries;
	u32	ca_pcigart_start;	/* PCI start index in ca_gart */
	void		*ca_pcigart_pagemap;

	/* AGP GART convenience values */
	u64	ca_gfxap_base;		/* gfx aperature bus base address */
	u64	ca_gfxap_size;		/* gfx aperature size (bytes) */
	u64	ca_gfxgart_base;	/* gfx GART bus base address */
	u64	*ca_gfxgart;		/* gfx GART vm address */
	u32	ca_gfxgart_entries;
	u32	ca_gfxgart_start;	/* agpgart start index in ca_gart */
};

/*
 * Common tioca info shared between kernel and prom
 *
 * DO NOT CHANGE THIS STRUCT WITHOUT MAKING CORRESPONDING CHANGES
 * TO THE PROM VERSION.
 */

struct tioca_common {
	struct pcibus_bussoft	ca_common;	/* common pciio header */

	u32		ca_rev;
	u32		ca_closest_nasid;

	u64		ca_prom_private;
	u64		ca_kernel_private;
};

/**
 * tioca_paddr_to_gart - Convert an SGI coretalk address to a CA GART entry
 * @paddr: page address to convert
 *
 * Convert a system [coretalk] address to a GART entry.  GART entries are
 * formed using the following:
 *
 *     data = ( (1<<63) |  ( (REMAP_NODE_ID << 40) | (MD_CHIPLET_ID << 38) | 
 * (REMAP_SYS_ADDR) ) >> 12 )
 *
 * DATA written to 1 GART TABLE Entry in system memory is remapped system
 * addr for 1 page 
 *
 * The data is for coretalk address format right shifted 12 bits with a
 * valid bit.
 *
 *	GART_TABLE_ENTRY [ 25:0 ]  -- REMAP_SYS_ADDRESS[37:12].
 *	GART_TABLE_ENTRY [ 27:26 ] -- SHUB MD chiplet id.
 *	GART_TABLE_ENTRY [ 41:28 ] -- REMAP_NODE_ID.
 *	GART_TABLE_ENTRY [ 63 ]    -- Valid Bit 
 */
static inline u64
tioca_paddr_to_gart(unsigned long paddr)
{
	/*
	 * We are assuming right now that paddr already has the correct
	 * format since the address from xtalk_dmaXXX should already have
	 * NODE_ID, CHIPLET_ID, and SYS_ADDR in the correct locations.
	 */

	return ((paddr) >> 12) | (1UL << 63);
}

/**
 * tioca_physpage_to_gart - Map a host physical page for SGI CA based DMA
 * @page_addr: system page address to map
 */

static inline unsigned long
tioca_physpage_to_gart(u64 page_addr)
{
	u64 coretalk_addr;

	coretalk_addr = PHYS_TO_TIODMA(page_addr);
	if (!coretalk_addr) {
		return 0;
	}

	return tioca_paddr_to_gart(coretalk_addr);
}

/**
 * tioca_tlbflush - invalidate cached SGI CA GART TLB entries
 * @tioca_kernel: CA context 
 *
 * Invalidate tlb entries for a given CA GART.  Main complexity is to account
 * for revA bug.
 */
static inline void
tioca_tlbflush(struct tioca_kernel *tioca_kernel)
{
	volatile u64 tmp;
	volatile struct tioca __iomem *ca_base;
	struct tioca_common *tioca_common;

	tioca_common = tioca_kernel->ca_common;
	ca_base = (struct tioca __iomem *)tioca_common->ca_common.bs_base;

	/*
	 * Explicit flushes not needed if GART is in cached mode
	 */
	if (tioca_kernel->ca_gart_iscoherent) {
		if (TIOCA_WAR_ENABLED(PV910244, tioca_common)) {
			/*
			 * PV910244:  RevA CA needs explicit flushes.
			 * Need to put GART into uncached mode before
			 * flushing otherwise the explicit flush is ignored.
			 *
			 * Alternate WAR would be to leave GART cached and
			 * touch every CL aligned GART entry.
			 */

			__sn_clrq_relaxed(&ca_base->ca_control2, CA_GART_MEM_PARAM);
			__sn_setq_relaxed(&ca_base->ca_control2, CA_GART_FLUSH_TLB);
			__sn_setq_relaxed(&ca_base->ca_control2,
			    (0x2ull << CA_GART_MEM_PARAM_SHFT));
			tmp = __sn_readq_relaxed(&ca_base->ca_control2);
		}

		return;
	}

	/*
	 * Gart in uncached mode ... need an explicit flush.
	 */

	__sn_setq_relaxed(&ca_base->ca_control2, CA_GART_FLUSH_TLB);
	tmp = __sn_readq_relaxed(&ca_base->ca_control2);
}

extern u32	tioca_gart_found;
extern struct list_head tioca_list;
extern int tioca_init_provider(void);
extern void tioca_fastwrite_enable(struct tioca_kernel *tioca_kern);
#endif /* _ASM_IA64_SN_TIO_CA_AGP_PROVIDER_H */
