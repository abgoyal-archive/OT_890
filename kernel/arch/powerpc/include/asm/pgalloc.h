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

#ifndef _ASM_POWERPC_PGALLOC_H
#define _ASM_POWERPC_PGALLOC_H
#ifdef __KERNEL__

#include <linux/mm.h>

static inline void pte_free_kernel(struct mm_struct *mm, pte_t *pte)
{
	free_page((unsigned long)pte);
}

static inline void pte_free(struct mm_struct *mm, pgtable_t ptepage)
{
	pgtable_page_dtor(ptepage);
	__free_page(ptepage);
}

typedef struct pgtable_free {
	unsigned long val;
} pgtable_free_t;

#define PGF_CACHENUM_MASK	0x7

static inline pgtable_free_t pgtable_free_cache(void *p, int cachenum,
						unsigned long mask)
{
	BUG_ON(cachenum > PGF_CACHENUM_MASK);

	return (pgtable_free_t){.val = ((unsigned long) p & ~mask) | cachenum};
}

#ifdef CONFIG_PPC64
#include <asm/pgalloc-64.h>
#else
#include <asm/pgalloc-32.h>
#endif

extern void pgtable_free_tlb(struct mmu_gather *tlb, pgtable_free_t pgf);

#ifdef CONFIG_SMP
#define __pte_free_tlb(tlb,ptepage)	\
do { \
	pgtable_page_dtor(ptepage); \
	pgtable_free_tlb(tlb, pgtable_free_cache(page_address(ptepage), \
		PTE_NONCACHE_NUM, PTE_TABLE_SIZE-1)); \
} while (0)
#else
#define __pte_free_tlb(tlb, pte)	pte_free((tlb)->mm, (pte))
#endif


#endif /* __KERNEL__ */
#endif /* _ASM_POWERPC_PGALLOC_H */
