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

#ifndef __ASM_SH_MMU_CONTEXT_64_H
#define __ASM_SH_MMU_CONTEXT_64_H

/*
 * sh64-specific mmu_context interface.
 *
 * Copyright (C) 2000, 2001  Paolo Alberelli
 * Copyright (C) 2003 - 2007  Paul Mundt
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */
#include <cpu/registers.h>
#include <asm/cacheflush.h>

#define SR_ASID_MASK		0xffffffffff00ffffULL
#define SR_ASID_SHIFT		16

/*
 * Destroy context related info for an mm_struct that is about
 * to be put to rest.
 */
static inline void destroy_context(struct mm_struct *mm)
{
	/* Well, at least free TLB entries */
	flush_tlb_mm(mm);
}

static inline unsigned long get_asid(void)
{
	unsigned long long sr;

	asm volatile ("getcon   " __SR ", %0\n\t"
		      : "=r" (sr));

	sr = (sr >> SR_ASID_SHIFT) & MMU_CONTEXT_ASID_MASK;
	return (unsigned long) sr;
}

/* Set ASID into SR */
static inline void set_asid(unsigned long asid)
{
	unsigned long long sr, pc;

	asm volatile ("getcon	" __SR ", %0" : "=r" (sr));

	sr = (sr & SR_ASID_MASK) | (asid << SR_ASID_SHIFT);

	/*
	 * It is possible that this function may be inlined and so to avoid
	 * the assembler reporting duplicate symbols we make use of the
	 * gas trick of generating symbols using numerics and forward
	 * reference.
	 */
	asm volatile ("movi	1, %1\n\t"
		      "shlli	%1, 28, %1\n\t"
		      "or	%0, %1, %1\n\t"
		      "putcon	%1, " __SR "\n\t"
		      "putcon	%0, " __SSR "\n\t"
		      "movi	1f, %1\n\t"
		      "ori	%1, 1 , %1\n\t"
		      "putcon	%1, " __SPC "\n\t"
		      "rte\n"
		      "1:\n\t"
		      : "=r" (sr), "=r" (pc) : "0" (sr));
}

/* arch/sh/kernel/cpu/sh5/entry.S */
extern unsigned long switch_and_save_asid(unsigned long new_asid);

/* No spare register to twiddle, so use a software cache */
extern pgd_t *mmu_pdtp_cache;

#define set_TTB(pgd)	(mmu_pdtp_cache = (pgd))
#define get_TTB()	(mmu_pdtp_cache)

#endif /* __ASM_SH_MMU_CONTEXT_64_H */
