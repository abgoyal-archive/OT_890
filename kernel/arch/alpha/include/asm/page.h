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

#ifndef _ALPHA_PAGE_H
#define _ALPHA_PAGE_H

#include <linux/const.h>
#include <asm/pal.h>

/* PAGE_SHIFT determines the page size */
#define PAGE_SHIFT	13
#define PAGE_SIZE	(_AC(1,UL) << PAGE_SHIFT)
#define PAGE_MASK	(~(PAGE_SIZE-1))

#ifndef __ASSEMBLY__

#define STRICT_MM_TYPECHECKS

extern void clear_page(void *page);
#define clear_user_page(page, vaddr, pg)	clear_page(page)

#define __alloc_zeroed_user_highpage(movableflags, vma, vaddr) \
	alloc_page_vma(GFP_HIGHUSER | __GFP_ZERO | movableflags, vma, vmaddr)
#define __HAVE_ARCH_ALLOC_ZEROED_USER_HIGHPAGE

extern void copy_page(void * _to, void * _from);
#define copy_user_page(to, from, vaddr, pg)	copy_page(to, from)

#ifdef STRICT_MM_TYPECHECKS
/*
 * These are used to make use of C type-checking..
 */
typedef struct { unsigned long pte; } pte_t;
typedef struct { unsigned long pmd; } pmd_t;
typedef struct { unsigned long pgd; } pgd_t;
typedef struct { unsigned long pgprot; } pgprot_t;

#define pte_val(x)	((x).pte)
#define pmd_val(x)	((x).pmd)
#define pgd_val(x)	((x).pgd)
#define pgprot_val(x)	((x).pgprot)

#define __pte(x)	((pte_t) { (x) } )
#define __pmd(x)	((pmd_t) { (x) } )
#define __pgd(x)	((pgd_t) { (x) } )
#define __pgprot(x)	((pgprot_t) { (x) } )

#else
/*
 * .. while these make it easier on the compiler
 */
typedef unsigned long pte_t;
typedef unsigned long pmd_t;
typedef unsigned long pgd_t;
typedef unsigned long pgprot_t;

#define pte_val(x)	(x)
#define pmd_val(x)	(x)
#define pgd_val(x)	(x)
#define pgprot_val(x)	(x)

#define __pte(x)	(x)
#define __pgd(x)	(x)
#define __pgprot(x)	(x)

#endif /* STRICT_MM_TYPECHECKS */

typedef struct page *pgtable_t;

#ifdef USE_48_BIT_KSEG
#define PAGE_OFFSET		0xffff800000000000UL
#else
#define PAGE_OFFSET		0xfffffc0000000000UL
#endif

#else

#ifdef USE_48_BIT_KSEG
#define PAGE_OFFSET		0xffff800000000000
#else
#define PAGE_OFFSET		0xfffffc0000000000
#endif

#endif /* !__ASSEMBLY__ */

#define __pa(x)			((unsigned long) (x) - PAGE_OFFSET)
#define __va(x)			((void *)((unsigned long) (x) + PAGE_OFFSET))
#ifndef CONFIG_DISCONTIGMEM
#define virt_to_page(kaddr)	pfn_to_page(__pa(kaddr) >> PAGE_SHIFT)

#define pfn_valid(pfn)		((pfn) < max_mapnr)
#define virt_addr_valid(kaddr)	pfn_valid(__pa(kaddr) >> PAGE_SHIFT)
#endif /* CONFIG_DISCONTIGMEM */

#define VM_DATA_DEFAULT_FLAGS		(VM_READ | VM_WRITE | VM_EXEC | \
					 VM_MAYREAD | VM_MAYWRITE | VM_MAYEXEC)

#include <asm-generic/memory_model.h>
#include <asm-generic/page.h>

#endif /* _ALPHA_PAGE_H */
