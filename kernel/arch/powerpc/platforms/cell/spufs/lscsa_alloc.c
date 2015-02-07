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
 * SPU local store allocation routines
 *
 * Copyright 2007 Benjamin Herrenschmidt, IBM Corp.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#undef DEBUG

#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/vmalloc.h>

#include <asm/spu.h>
#include <asm/spu_csa.h>
#include <asm/mmu.h>

#include "spufs.h"

static int spu_alloc_lscsa_std(struct spu_state *csa)
{
	struct spu_lscsa *lscsa;
	unsigned char *p;

	lscsa = vmalloc(sizeof(struct spu_lscsa));
	if (!lscsa)
		return -ENOMEM;
	memset(lscsa, 0, sizeof(struct spu_lscsa));
	csa->lscsa = lscsa;

	/* Set LS pages reserved to allow for user-space mapping. */
	for (p = lscsa->ls; p < lscsa->ls + LS_SIZE; p += PAGE_SIZE)
		SetPageReserved(vmalloc_to_page(p));

	return 0;
}

static void spu_free_lscsa_std(struct spu_state *csa)
{
	/* Clear reserved bit before vfree. */
	unsigned char *p;

	if (csa->lscsa == NULL)
		return;

	for (p = csa->lscsa->ls; p < csa->lscsa->ls + LS_SIZE; p += PAGE_SIZE)
		ClearPageReserved(vmalloc_to_page(p));

	vfree(csa->lscsa);
}

#ifdef CONFIG_SPU_FS_64K_LS

#define SPU_64K_PAGE_SHIFT	16
#define SPU_64K_PAGE_ORDER	(SPU_64K_PAGE_SHIFT - PAGE_SHIFT)
#define SPU_64K_PAGE_COUNT	(1ul << SPU_64K_PAGE_ORDER)

int spu_alloc_lscsa(struct spu_state *csa)
{
	struct page	**pgarray;
	unsigned char	*p;
	int		i, j, n_4k;

	/* Check availability of 64K pages */
	if (!spu_64k_pages_available())
		goto fail;

	csa->use_big_pages = 1;

	pr_debug("spu_alloc_lscsa(csa=0x%p), trying to allocate 64K pages\n",
		 csa);

	/* First try to allocate our 64K pages. We need 5 of them
	 * with the current implementation. In the future, we should try
	 * to separate the lscsa with the actual local store image, thus
	 * allowing us to require only 4 64K pages per context
	 */
	for (i = 0; i < SPU_LSCSA_NUM_BIG_PAGES; i++) {
		/* XXX This is likely to fail, we should use a special pool
		 *     similiar to what hugetlbfs does.
		 */
		csa->lscsa_pages[i] = alloc_pages(GFP_KERNEL,
						  SPU_64K_PAGE_ORDER);
		if (csa->lscsa_pages[i] == NULL)
			goto fail;
	}

	pr_debug(" success ! creating vmap...\n");

	/* Now we need to create a vmalloc mapping of these for the kernel
	 * and SPU context switch code to use. Currently, we stick to a
	 * normal kernel vmalloc mapping, which in our case will be 4K
	 */
	n_4k = SPU_64K_PAGE_COUNT * SPU_LSCSA_NUM_BIG_PAGES;
	pgarray = kmalloc(sizeof(struct page *) * n_4k, GFP_KERNEL);
	if (pgarray == NULL)
		goto fail;
	for (i = 0; i < SPU_LSCSA_NUM_BIG_PAGES; i++)
		for (j = 0; j < SPU_64K_PAGE_COUNT; j++)
			/* We assume all the struct page's are contiguous
			 * which should be hopefully the case for an order 4
			 * allocation..
			 */
			pgarray[i * SPU_64K_PAGE_COUNT + j] =
				csa->lscsa_pages[i] + j;
	csa->lscsa = vmap(pgarray, n_4k, VM_USERMAP, PAGE_KERNEL);
	kfree(pgarray);
	if (csa->lscsa == NULL)
		goto fail;

	memset(csa->lscsa, 0, sizeof(struct spu_lscsa));

	/* Set LS pages reserved to allow for user-space mapping.
	 *
	 * XXX isn't that a bit obsolete ? I think we should just
	 * make sure the page count is high enough. Anyway, won't harm
	 * for now
	 */
	for (p = csa->lscsa->ls; p < csa->lscsa->ls + LS_SIZE; p += PAGE_SIZE)
		SetPageReserved(vmalloc_to_page(p));

	pr_debug(" all good !\n");

	return 0;
fail:
	pr_debug("spufs: failed to allocate lscsa 64K pages, falling back\n");
	spu_free_lscsa(csa);
	return spu_alloc_lscsa_std(csa);
}

void spu_free_lscsa(struct spu_state *csa)
{
	unsigned char *p;
	int i;

	if (!csa->use_big_pages) {
		spu_free_lscsa_std(csa);
		return;
	}
	csa->use_big_pages = 0;

	if (csa->lscsa == NULL)
		goto free_pages;

	for (p = csa->lscsa->ls; p < csa->lscsa->ls + LS_SIZE; p += PAGE_SIZE)
		ClearPageReserved(vmalloc_to_page(p));

	vunmap(csa->lscsa);
	csa->lscsa = NULL;

 free_pages:

	for (i = 0; i < SPU_LSCSA_NUM_BIG_PAGES; i++)
		if (csa->lscsa_pages[i])
			__free_pages(csa->lscsa_pages[i], SPU_64K_PAGE_ORDER);
}

#else /* CONFIG_SPU_FS_64K_LS */

int spu_alloc_lscsa(struct spu_state *csa)
{
	return spu_alloc_lscsa_std(csa);
}

void spu_free_lscsa(struct spu_state *csa)
{
	spu_free_lscsa_std(csa);
}

#endif /* !defined(CONFIG_SPU_FS_64K_LS) */
