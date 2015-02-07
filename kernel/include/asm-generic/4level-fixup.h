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

#ifndef _4LEVEL_FIXUP_H
#define _4LEVEL_FIXUP_H

#define __ARCH_HAS_4LEVEL_HACK
#define __PAGETABLE_PUD_FOLDED

#define PUD_SIZE			PGDIR_SIZE
#define PUD_MASK			PGDIR_MASK
#define PTRS_PER_PUD			1

#define pud_t				pgd_t

#define pmd_alloc(mm, pud, address) \
	((unlikely(pgd_none(*(pud))) && __pmd_alloc(mm, pud, address))? \
 		NULL: pmd_offset(pud, address))

#define pud_alloc(mm, pgd, address)	(pgd)
#define pud_offset(pgd, start)		(pgd)
#define pud_none(pud)			0
#define pud_bad(pud)			0
#define pud_present(pud)		1
#define pud_ERROR(pud)			do { } while (0)
#define pud_clear(pud)			pgd_clear(pud)
#define pud_val(pud)			pgd_val(pud)
#define pud_populate(mm, pud, pmd)	pgd_populate(mm, pud, pmd)
#define pud_page(pud)			pgd_page(pud)
#define pud_page_vaddr(pud)		pgd_page_vaddr(pud)

#undef pud_free_tlb
#define pud_free_tlb(tlb, x)            do { } while (0)
#define pud_free(mm, x)			do { } while (0)
#define __pud_free_tlb(tlb, x)		do { } while (0)

#undef  pud_addr_end
#define pud_addr_end(addr, end)		(end)

#endif
