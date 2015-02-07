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
 *  linux/arch/m32r/mm/ioremap.c
 *
 *  Copyright (c) 2001, 2002  Hiroyuki Kondo
 *
 *  Taken from mips version.
 *    (C) Copyright 1995 1996 Linus Torvalds
 *    (C) Copyright 2001 Ralf Baechle
 */

/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 */

#include <linux/module.h>
#include <asm/addrspace.h>
#include <asm/byteorder.h>

#include <linux/vmalloc.h>
#include <linux/io.h>
#include <asm/pgalloc.h>

/*
 * Generic mapping function (not visible outside):
 */

/*
 * Remap an arbitrary physical address space into the kernel virtual
 * address space. Needed when the kernel wants to access high addresses
 * directly.
 *
 * NOTE! We need to allow non-page-aligned mappings too: we will obviously
 * have to convert them into an offset in a page-aligned mapping, but the
 * caller shouldn't need to know that small detail.
 */

#define IS_LOW512(addr) (!((unsigned long)(addr) & ~0x1fffffffUL))

void __iomem *
__ioremap(unsigned long phys_addr, unsigned long size, unsigned long flags)
{
	void __iomem * addr;
	struct vm_struct * area;
	unsigned long offset, last_addr;
	pgprot_t pgprot;

	/* Don't allow wraparound or zero size */
	last_addr = phys_addr + size - 1;
	if (!size || last_addr < phys_addr)
		return NULL;

	/*
	 * Map objects in the low 512mb of address space using KSEG1, otherwise
	 * map using page tables.
	 */
	if (IS_LOW512(phys_addr) && IS_LOW512(phys_addr + size - 1))
		return (void *) KSEG1ADDR(phys_addr);

	/*
	 * Don't allow anybody to remap normal RAM that we're using..
	 */
	if (phys_addr < virt_to_phys(high_memory)) {
		char *t_addr, *t_end;
		struct page *page;

		t_addr = __va(phys_addr);
		t_end = t_addr + (size - 1);

		for(page = virt_to_page(t_addr); page <= virt_to_page(t_end); page++)
			if(!PageReserved(page))
				return NULL;
	}

	pgprot = __pgprot(_PAGE_GLOBAL | _PAGE_PRESENT | _PAGE_READ
			  | _PAGE_WRITE | flags);

	/*
	 * Mappings have to be page-aligned
	 */
	offset = phys_addr & ~PAGE_MASK;
	phys_addr &= PAGE_MASK;
	size = PAGE_ALIGN(last_addr + 1) - phys_addr;

	/*
	 * Ok, go for it..
	 */
	area = get_vm_area(size, VM_IOREMAP);
	if (!area)
		return NULL;
	area->phys_addr = phys_addr;
	addr = (void __iomem *) area->addr;
	if (ioremap_page_range((unsigned long)addr, (unsigned long)addr + size,
			       phys_addr, pgprot)) {
		vunmap((void __force *) addr);
		return NULL;
	}

	return (void __iomem *) (offset + (char __iomem *)addr);
}

#define IS_KSEG1(addr) (((unsigned long)(addr) & ~0x1fffffffUL) == KSEG1)

void iounmap(volatile void __iomem *addr)
{
	if (!IS_KSEG1(addr))
		vfree((void *) (PAGE_MASK & (unsigned long) addr));
}

