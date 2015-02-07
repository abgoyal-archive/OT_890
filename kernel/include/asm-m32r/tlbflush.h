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

#ifndef _ASM_M32R_TLBFLUSH_H
#define _ASM_M32R_TLBFLUSH_H

#include <asm/m32r.h>

/*
 * TLB flushing:
 *
 *  - flush_tlb() flushes the current mm struct TLBs
 *  - flush_tlb_all() flushes all processes TLBs
 *  - flush_tlb_mm(mm) flushes the specified mm context TLB's
 *  - flush_tlb_page(vma, vmaddr) flushes one page
 *  - flush_tlb_range(vma, start, end) flushes a range of pages
 *  - flush_tlb_kernel_range(start, end) flushes a range of kernel pages
 */

extern void local_flush_tlb_all(void);
extern void local_flush_tlb_mm(struct mm_struct *);
extern void local_flush_tlb_page(struct vm_area_struct *, unsigned long);
extern void local_flush_tlb_range(struct vm_area_struct *, unsigned long,
	unsigned long);

#ifndef CONFIG_SMP
#ifdef CONFIG_MMU
#define flush_tlb_all()			local_flush_tlb_all()
#define flush_tlb_mm(mm)		local_flush_tlb_mm(mm)
#define flush_tlb_page(vma, page)	local_flush_tlb_page(vma, page)
#define flush_tlb_range(vma, start, end)	\
	local_flush_tlb_range(vma, start, end)
#define flush_tlb_kernel_range(start, end)	local_flush_tlb_all()
#else	/* CONFIG_MMU */
#define flush_tlb_all()			do { } while (0)
#define flush_tlb_mm(mm)		do { } while (0)
#define flush_tlb_page(vma, vmaddr)	do { } while (0)
#define flush_tlb_range(vma, start, end)	do { } while (0)
#endif	/* CONFIG_MMU */
#else	/* CONFIG_SMP */
extern void smp_flush_tlb_all(void);
extern void smp_flush_tlb_mm(struct mm_struct *);
extern void smp_flush_tlb_page(struct vm_area_struct *, unsigned long);
extern void smp_flush_tlb_range(struct vm_area_struct *, unsigned long,
	unsigned long);

#define flush_tlb_all()			smp_flush_tlb_all()
#define flush_tlb_mm(mm)		smp_flush_tlb_mm(mm)
#define flush_tlb_page(vma, page)	smp_flush_tlb_page(vma, page)
#define flush_tlb_range(vma, start, end)	\
	smp_flush_tlb_range(vma, start, end)
#define flush_tlb_kernel_range(start, end)	smp_flush_tlb_all()
#endif	/* CONFIG_SMP */

static __inline__ void __flush_tlb_page(unsigned long page)
{
	unsigned int tmpreg0, tmpreg1, tmpreg2;

	__asm__ __volatile__ (
		"seth	%0, #high(%4)	\n\t"
		"st	%3, @(%5, %0)	\n\t"
		"ldi	%1, #1		\n\t"
		"st	%1, @(%6, %0)	\n\t"
		"add3	%1, %0, %7	\n\t"
		".fillinsn		\n"
		"1:			\n\t"
		"ld	%2, @(%6, %0)	\n\t"
		"bnez	%2, 1b		\n\t"
		"ld	%0, @%1+	\n\t"
		"ld	%1, @%1		\n\t"
		"st	%2, @+%0	\n\t"
		"st	%2, @+%1	\n\t"
		: "=&r" (tmpreg0), "=&r" (tmpreg1), "=&r" (tmpreg2)
		: "r" (page), "i" (MMU_REG_BASE), "i" (MSVA_offset),
		"i" (MTOP_offset), "i" (MIDXI_offset)
		: "memory"
	);
}

static __inline__ void __flush_tlb_all(void)
{
	unsigned int tmpreg0, tmpreg1;

	__asm__ __volatile__ (
		"seth	%0, #high(%2)		\n\t"
		"or3	%0, %0, #low(%2)	\n\t"
		"ldi	%1, #0xc		\n\t"
		"st	%1, @%0			\n\t"
		".fillinsn			\n"
		"1:				\n\t"
		"ld	%1, @%0			\n\t"
		"bnez	%1, 1b			\n\t"
		: "=&r" (tmpreg0), "=&r" (tmpreg1)
		: "i" (MTOP) : "memory"
	);
}

extern void update_mmu_cache(struct vm_area_struct *, unsigned long, pte_t);

#endif	/* _ASM_M32R_TLBFLUSH_H */
