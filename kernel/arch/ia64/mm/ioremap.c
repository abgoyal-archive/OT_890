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
 * (c) Copyright 2006, 2007 Hewlett-Packard Development Company, L.P.
 *	Bjorn Helgaas <bjorn.helgaas@hp.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/compiler.h>
#include <linux/module.h>
#include <linux/efi.h>
#include <linux/io.h>
#include <linux/vmalloc.h>
#include <asm/io.h>
#include <asm/meminit.h>

static inline void __iomem *
__ioremap (unsigned long phys_addr)
{
	return (void __iomem *) (__IA64_UNCACHED_OFFSET | phys_addr);
}

void __iomem *
ioremap (unsigned long phys_addr, unsigned long size)
{
	void __iomem *addr;
	struct vm_struct *area;
	unsigned long offset;
	pgprot_t prot;
	u64 attr;
	unsigned long gran_base, gran_size;
	unsigned long page_base;

	/*
	 * For things in kern_memmap, we must use the same attribute
	 * as the rest of the kernel.  For more details, see
	 * Documentation/ia64/aliasing.txt.
	 */
	attr = kern_mem_attribute(phys_addr, size);
	if (attr & EFI_MEMORY_WB)
		return (void __iomem *) phys_to_virt(phys_addr);
	else if (attr & EFI_MEMORY_UC)
		return __ioremap(phys_addr);

	/*
	 * Some chipsets don't support UC access to memory.  If
	 * WB is supported for the whole granule, we prefer that.
	 */
	gran_base = GRANULEROUNDDOWN(phys_addr);
	gran_size = GRANULEROUNDUP(phys_addr + size) - gran_base;
	if (efi_mem_attribute(gran_base, gran_size) & EFI_MEMORY_WB)
		return (void __iomem *) phys_to_virt(phys_addr);

	/*
	 * WB is not supported for the whole granule, so we can't use
	 * the region 7 identity mapping.  If we can safely cover the
	 * area with kernel page table mappings, we can use those
	 * instead.
	 */
	page_base = phys_addr & PAGE_MASK;
	size = PAGE_ALIGN(phys_addr + size) - page_base;
	if (efi_mem_attribute(page_base, size) & EFI_MEMORY_WB) {
		prot = PAGE_KERNEL;

		/*
		 * Mappings have to be page-aligned
		 */
		offset = phys_addr & ~PAGE_MASK;
		phys_addr &= PAGE_MASK;

		/*
		 * Ok, go for it..
		 */
		area = get_vm_area(size, VM_IOREMAP);
		if (!area)
			return NULL;

		area->phys_addr = phys_addr;
		addr = (void __iomem *) area->addr;
		if (ioremap_page_range((unsigned long) addr,
				(unsigned long) addr + size, phys_addr, prot)) {
			vunmap((void __force *) addr);
			return NULL;
		}

		return (void __iomem *) (offset + (char __iomem *)addr);
	}

	return __ioremap(phys_addr);
}
EXPORT_SYMBOL(ioremap);

void __iomem *
ioremap_nocache (unsigned long phys_addr, unsigned long size)
{
	if (kern_mem_attribute(phys_addr, size) & EFI_MEMORY_WB)
		return NULL;

	return __ioremap(phys_addr);
}
EXPORT_SYMBOL(ioremap_nocache);

void
iounmap (volatile void __iomem *addr)
{
	if (REGION_NUMBER(addr) == RGN_GATE)
		vunmap((void *) ((unsigned long) addr & PAGE_MASK));
}
EXPORT_SYMBOL(iounmap);
