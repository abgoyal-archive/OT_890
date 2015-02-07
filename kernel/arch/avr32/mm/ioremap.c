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
 * Copyright (C) 2004-2006 Atmel Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/vmalloc.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/io.h>

#include <asm/pgtable.h>
#include <asm/addrspace.h>

/*
 * Re-map an arbitrary physical address space into the kernel virtual
 * address space. Needed when the kernel wants to access physical
 * memory directly.
 */
void __iomem *__ioremap(unsigned long phys_addr, size_t size,
			unsigned long flags)
{
	unsigned long addr;
	struct vm_struct *area;
	unsigned long offset, last_addr;
	pgprot_t prot;

	/*
	 * Check if we can simply use the P4 segment. This area is
	 * uncacheable, so if caching/buffering is requested, we can't
	 * use it.
	 */
	if ((phys_addr >= P4SEG) && (flags == 0))
		return (void __iomem *)phys_addr;

	/* Don't allow wraparound or zero size */
	last_addr = phys_addr + size - 1;
	if (!size || last_addr < phys_addr)
		return NULL;

	/*
	 * XXX: When mapping regular RAM, we'd better make damn sure
	 * it's never used for anything else.  But this is really the
	 * caller's responsibility...
	 */
	if (PHYSADDR(P2SEGADDR(phys_addr)) == phys_addr)
		return (void __iomem *)P2SEGADDR(phys_addr);

	/* Mappings have to be page-aligned */
	offset = phys_addr & ~PAGE_MASK;
	phys_addr &= PAGE_MASK;
	size = PAGE_ALIGN(last_addr + 1) - phys_addr;

	prot = __pgprot(_PAGE_PRESENT | _PAGE_GLOBAL | _PAGE_RW | _PAGE_DIRTY
			| _PAGE_ACCESSED | _PAGE_TYPE_SMALL | flags);

	/*
	 * Ok, go for it..
	 */
	area = get_vm_area(size, VM_IOREMAP);
	if (!area)
		return NULL;
	area->phys_addr = phys_addr;
	addr = (unsigned long )area->addr;
	if (ioremap_page_range(addr, addr + size, phys_addr, prot)) {
		vunmap((void *)addr);
		return NULL;
	}

	return (void __iomem *)(offset + (char *)addr);
}
EXPORT_SYMBOL(__ioremap);

void __iounmap(void __iomem *addr)
{
	struct vm_struct *p;

	if ((unsigned long)addr >= P4SEG)
		return;
	if (PXSEG(addr) == P2SEG)
		return;

	p = remove_vm_area((void *)(PAGE_MASK & (unsigned long __force)addr));
	if (unlikely(!p)) {
		printk (KERN_ERR "iounmap: bad address %p\n", addr);
		return;
	}

	kfree (p);
}
EXPORT_SYMBOL(__iounmap);
