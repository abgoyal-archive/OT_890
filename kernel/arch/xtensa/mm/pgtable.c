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
 * arch/xtensa/mm/pgtable.c
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2001 - 2005 Tensilica Inc.
 *
 * Chris Zankel <chris@zankel.net>
 */

#if (DCACHE_SIZE > PAGE_SIZE)

pte_t* pte_alloc_one_kernel(struct mm_struct *mm, unsigned long address)
{
	pte_t *pte = NULL, *p;
	int color = ADDR_COLOR(address);
	int i;

	p = (pte_t*) __get_free_pages(GFP_KERNEL|__GFP_REPEAT, COLOR_ORDER);

	if (likely(p)) {
		split_page(virt_to_page(p), COLOR_ORDER);

		for (i = 0; i < COLOR_SIZE; i++) {
			if (ADDR_COLOR(p) == color)
				pte = p;
			else
				free_page(p);
			p += PTRS_PER_PTE;
		}
		clear_page(pte);
	}
	return pte;
}

#ifdef PROFILING

int mask;
int hit;
int flush;

#endif

struct page* pte_alloc_one(struct mm_struct *mm, unsigned long address)
{
	struct page *page = NULL, *p;
	int color = ADDR_COLOR(address);

	p = alloc_pages(GFP_KERNEL | __GFP_REPEAT, PTE_ORDER);

	if (likely(p)) {
		split_page(p, COLOR_ORDER);

		for (i = 0; i < PAGE_ORDER; i++) {
			if (PADDR_COLOR(page_address(p)) == color)
				page = p;
			else
				__free_page(p);
			p++;
		}
		clear_highpage(page);
	}

	return page;
}

#endif



