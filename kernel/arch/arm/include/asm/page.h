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
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

/*
 *  arch/arm/include/asm/page.h
 *
 *  Copyright (C) 1995-2003 Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef _ASMARM_PAGE_H
#define _ASMARM_PAGE_H

/* PAGE_SHIFT determines the page size */
#define PAGE_SHIFT		12

#ifdef CONFIG_HAVE_TCM
#define PAGE_SIZE		(_AC(1,UL) << PAGE_SHIFT)
#else
#define PAGE_SIZE              (1UL << PAGE_SHIFT)
#endif

#define PAGE_MASK		(~(PAGE_SIZE-1))

#ifndef __ASSEMBLY__

#ifndef CONFIG_MMU

#include "page-nommu.h"

#else

#include <asm/glue.h>

/*
 *	User Space Model
 *	================
 *
 *	This section selects the correct set of functions for dealing with
 *	page-based copying and clearing for user space for the particular
 *	processor(s) we're building for.
 *
 *	We have the following to choose from:
 *	  v3		- ARMv3
 *	  v4wt		- ARMv4 with writethrough cache, without minicache
 *	  v4wb		- ARMv4 with writeback cache, without minicache
 *	  v4_mc		- ARMv4 with minicache
 *	  xscale	- Xscale
 *	  xsc3		- XScalev3
 */
#undef _USER
#undef MULTI_USER

#ifdef CONFIG_CPU_COPY_V3
# ifdef _USER
#  define MULTI_USER 1
# else
#  define _USER v3
# endif
#endif

#ifdef CONFIG_CPU_COPY_V4WT
# ifdef _USER
#  define MULTI_USER 1
# else
#  define _USER v4wt
# endif
#endif

#ifdef CONFIG_CPU_COPY_V4WB
# ifdef _USER
#  define MULTI_USER 1
# else
#  define _USER v4wb
# endif
#endif

#ifdef CONFIG_CPU_COPY_FEROCEON
# ifdef _USER
#  define MULTI_USER 1
# else
#  define _USER feroceon
# endif
#endif

#ifdef CONFIG_CPU_SA1100
# ifdef _USER
#  define MULTI_USER 1
# else
#  define _USER v4_mc
# endif
#endif

#ifdef CONFIG_CPU_XSCALE
# ifdef _USER
#  define MULTI_USER 1
# else
#  define _USER xscale_mc
# endif
#endif

#ifdef CONFIG_CPU_XSC3
# ifdef _USER
#  define MULTI_USER 1
# else
#  define _USER xsc3_mc
# endif
#endif

#ifdef CONFIG_CPU_COPY_V6
# define MULTI_USER 1
#endif

#if !defined(_USER) && !defined(MULTI_USER)
#error Unknown user operations model
#endif

struct page;

struct cpu_user_fns {
	void (*cpu_clear_user_highpage)(struct page *page, unsigned long vaddr);
	void (*cpu_copy_user_highpage)(struct page *to, struct page *from,
			unsigned long vaddr);
};

#ifdef MULTI_USER
extern struct cpu_user_fns cpu_user;

#define __cpu_clear_user_highpage	cpu_user.cpu_clear_user_highpage
#define __cpu_copy_user_highpage	cpu_user.cpu_copy_user_highpage

#else

#define __cpu_clear_user_highpage	__glue(_USER,_clear_user_highpage)
#define __cpu_copy_user_highpage	__glue(_USER,_copy_user_highpage)

extern void __cpu_clear_user_highpage(struct page *page, unsigned long vaddr);
extern void __cpu_copy_user_highpage(struct page *to, struct page *from,
			unsigned long vaddr);
#endif

#define clear_user_highpage(page,vaddr)		\
	 __cpu_clear_user_highpage(page, vaddr)

#define __HAVE_ARCH_COPY_USER_HIGHPAGE
#define copy_user_highpage(to,from,vaddr,vma)	\
	__cpu_copy_user_highpage(to, from, vaddr)

#define clear_page(page)	memset((void *)(page), 0, PAGE_SIZE)
extern void copy_page(void *to, const void *from);

#undef STRICT_MM_TYPECHECKS

#ifdef STRICT_MM_TYPECHECKS
/*
 * These are used to make use of C type-checking..
 */
typedef struct { unsigned long pte; } pte_t;
typedef struct { unsigned long pmd; } pmd_t;
typedef struct { unsigned long pgd[2]; } pgd_t;
typedef struct { unsigned long pgprot; } pgprot_t;

#define pte_val(x)      ((x).pte)
#define pmd_val(x)      ((x).pmd)
#define pgd_val(x)	((x).pgd[0])
#define pgprot_val(x)   ((x).pgprot)

#define __pte(x)        ((pte_t) { (x) } )
#define __pmd(x)        ((pmd_t) { (x) } )
#define __pgprot(x)     ((pgprot_t) { (x) } )

#else
/*
 * .. while these make it easier on the compiler
 */
typedef unsigned long pte_t;
typedef unsigned long pmd_t;
typedef unsigned long pgd_t[2];
typedef unsigned long pgprot_t;

#define pte_val(x)      (x)
#define pmd_val(x)      (x)
#define pgd_val(x)	((x)[0])
#define pgprot_val(x)   (x)

#define __pte(x)        (x)
#define __pmd(x)        (x)
#define __pgprot(x)     (x)

#endif /* STRICT_MM_TYPECHECKS */

#endif /* CONFIG_MMU */

typedef struct page *pgtable_t;

#include <asm/memory.h>

#endif /* !__ASSEMBLY__ */

#define VM_DATA_DEFAULT_FLAGS \
	(((current->personality & READ_IMPLIES_EXEC) ? VM_EXEC : 0) | \
	 VM_READ | VM_WRITE | VM_MAYREAD | VM_MAYWRITE | VM_MAYEXEC)

/*
 * With EABI on ARMv5 and above we must have 64-bit aligned slab pointers.
 */
#if defined(CONFIG_AEABI) && (__LINUX_ARM_ARCH__ >= 5)
#define ARCH_SLAB_MINALIGN 8
#endif

#include <asm-generic/page.h>

#endif
