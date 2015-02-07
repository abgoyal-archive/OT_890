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
 *	TLB shootdown specifics for powerpc
 *
 * Copyright (C) 2002 Anton Blanchard, IBM Corp.
 * Copyright (C) 2002 Paul Mackerras, IBM Corp.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */
#ifndef _ASM_POWERPC_TLB_H
#define _ASM_POWERPC_TLB_H
#ifdef __KERNEL__

#ifndef __powerpc64__
#include <asm/pgtable.h>
#endif
#include <asm/pgalloc.h>
#include <asm/tlbflush.h>
#ifndef __powerpc64__
#include <asm/page.h>
#include <asm/mmu.h>
#endif

#include <linux/pagemap.h>

struct mmu_gather;

#define tlb_start_vma(tlb, vma)	do { } while (0)
#define tlb_end_vma(tlb, vma)	do { } while (0)

#if !defined(CONFIG_PPC_STD_MMU)

#define tlb_flush(tlb)			flush_tlb_mm((tlb)->mm)

#elif defined(__powerpc64__)

extern void pte_free_finish(void);

static inline void tlb_flush(struct mmu_gather *tlb)
{
	struct ppc64_tlb_batch *tlbbatch = &__get_cpu_var(ppc64_tlb_batch);

	/* If there's a TLB batch pending, then we must flush it because the
	 * pages are going to be freed and we really don't want to have a CPU
	 * access a freed page because it has a stale TLB
	 */
	if (tlbbatch->index)
		__flush_tlb_pending(tlbbatch);

	pte_free_finish();
}

#else

extern void tlb_flush(struct mmu_gather *tlb);

#endif

/* Get the generic bits... */
#include <asm-generic/tlb.h>

#if !defined(CONFIG_PPC_STD_MMU) || defined(__powerpc64__)

#define __tlb_remove_tlb_entry(tlb, pte, address) do { } while (0)

#else
extern void flush_hash_entry(struct mm_struct *mm, pte_t *ptep,
			     unsigned long address);

static inline void __tlb_remove_tlb_entry(struct mmu_gather *tlb, pte_t *ptep,
					unsigned long address)
{
	if (pte_val(*ptep) & _PAGE_HASHPTE)
		flush_hash_entry(tlb->mm, ptep, address);
}

#endif
#endif /* __KERNEL__ */
#endif /* __ASM_POWERPC_TLB_H */
