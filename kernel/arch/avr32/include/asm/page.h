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
#ifndef __ASM_AVR32_PAGE_H
#define __ASM_AVR32_PAGE_H

#include <linux/const.h>

/* PAGE_SHIFT determines the page size */
#define PAGE_SHIFT	12
#define PAGE_SIZE	(_AC(1, UL) << PAGE_SHIFT)
#define PAGE_MASK	(~(PAGE_SIZE-1))
#define PTE_MASK	PAGE_MASK

#ifndef __ASSEMBLY__

#include <asm/addrspace.h>

extern void clear_page(void *to);
extern void copy_page(void *to, void *from);

#define clear_user_page(page, vaddr, pg)	clear_page(page)
#define copy_user_page(to, from, vaddr, pg)	copy_page(to, from)

/*
 * These are used to make use of C type-checking..
 */
typedef struct { unsigned long pte; } pte_t;
typedef struct { unsigned long pgd; } pgd_t;
typedef struct { unsigned long pgprot; } pgprot_t;
typedef struct page *pgtable_t;

#define pte_val(x)		((x).pte)
#define pgd_val(x)		((x).pgd)
#define pgprot_val(x)		((x).pgprot)

#define __pte(x)		((pte_t) { (x) })
#define __pgd(x)		((pgd_t) { (x) })
#define __pgprot(x)		((pgprot_t) { (x) })

/* FIXME: These should be removed soon */
extern unsigned long memory_start, memory_end;

/* Pure 2^n version of get_order */
static inline int get_order(unsigned long size)
{
	unsigned lz;

	size = (size - 1) >> PAGE_SHIFT;
	asm("clz %0, %1" : "=r"(lz) : "r"(size));
	return 32 - lz;
}

#endif /* !__ASSEMBLY__ */

/*
 * The hardware maps the virtual addresses 0x80000000 -> 0x9fffffff
 * permanently to the physical addresses 0x00000000 -> 0x1fffffff when
 * segmentation is enabled. We want to make use of this in order to
 * minimize TLB pressure.
 */
#define PAGE_OFFSET		(0x80000000UL)

/*
 * ALSA uses virt_to_page() on DMA pages, which I'm not entirely sure
 * is a good idea. Anyway, we can't simply subtract PAGE_OFFSET here
 * in that case, so we'll have to mask out the three most significant
 * bits of the address instead...
 *
 * What's the difference between __pa() and virt_to_phys() anyway?
 */
#define __pa(x)		PHYSADDR(x)
#define __va(x)		((void *)(P1SEGADDR(x)))

#define MAP_NR(addr)	(((unsigned long)(addr) - PAGE_OFFSET) >> PAGE_SHIFT)

#define phys_to_page(phys)	(pfn_to_page(phys >> PAGE_SHIFT))
#define page_to_phys(page)	(page_to_pfn(page) << PAGE_SHIFT)

#ifndef CONFIG_NEED_MULTIPLE_NODES

#define PHYS_PFN_OFFSET		(CONFIG_PHYS_OFFSET >> PAGE_SHIFT)

#define pfn_to_page(pfn)	(mem_map + ((pfn) - PHYS_PFN_OFFSET))
#define page_to_pfn(page)	((unsigned long)((page) - mem_map) + PHYS_PFN_OFFSET)
#define pfn_valid(pfn)		((pfn) >= PHYS_PFN_OFFSET && (pfn) < (PHYS_PFN_OFFSET + max_mapnr))
#endif /* CONFIG_NEED_MULTIPLE_NODES */

#define virt_to_page(kaddr)	pfn_to_page(__pa(kaddr) >> PAGE_SHIFT)
#define virt_addr_valid(kaddr)	pfn_valid(__pa(kaddr) >> PAGE_SHIFT)

#define VM_DATA_DEFAULT_FLAGS	(VM_READ | VM_WRITE |	\
				 VM_MAYREAD | VM_MAYWRITE | VM_MAYEXEC)

/*
 * Memory above this physical address will be considered highmem.
 */
#define HIGHMEM_START		0x20000000UL

#endif /* __ASM_AVR32_PAGE_H */
