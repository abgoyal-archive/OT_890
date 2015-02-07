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
 * arch/sh/mm/tlb-nommu.c
 *
 * TLB Operations for MMUless SH.
 *
 * Copyright (C) 2002 Paul Mundt
 *
 * Released under the terms of the GNU GPL v2.0.
 */
#include <linux/kernel.h>
#include <linux/mm.h>
#include <asm/pgtable.h>
#include <asm/tlbflush.h>

/*
 * Nothing too terribly exciting here ..
 */
void local_flush_tlb_all(void)
{
	BUG();
}

void local_flush_tlb_mm(struct mm_struct *mm)
{
	BUG();
}

void local_flush_tlb_range(struct vm_area_struct *vma, unsigned long start,
			    unsigned long end)
{
	BUG();
}

void local_flush_tlb_page(struct vm_area_struct *vma, unsigned long page)
{
	BUG();
}

void local_flush_tlb_one(unsigned long asid, unsigned long page)
{
	BUG();
}

void local_flush_tlb_kernel_range(unsigned long start, unsigned long end)
{
	BUG();
}

void update_mmu_cache(struct vm_area_struct * vma,
		      unsigned long address, pte_t pte)
{
	BUG();
}

void __init page_table_range_init(unsigned long start, unsigned long end,
				  pgd_t *pgd_base)
{
}

void __set_fixmap(enum fixed_addresses idx, unsigned long phys, pgprot_t prot)
{
}
