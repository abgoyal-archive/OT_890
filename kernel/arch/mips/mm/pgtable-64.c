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
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1999, 2000 by Silicon Graphics
 * Copyright (C) 2003 by Ralf Baechle
 */
#include <linux/init.h>
#include <linux/mm.h>
#include <asm/fixmap.h>
#include <asm/pgtable.h>
#include <asm/pgalloc.h>

void pgd_init(unsigned long page)
{
	unsigned long *p, *end;

 	p = (unsigned long *) page;
	end = p + PTRS_PER_PGD;

	while (p < end) {
		p[0] = (unsigned long) invalid_pmd_table;
		p[1] = (unsigned long) invalid_pmd_table;
		p[2] = (unsigned long) invalid_pmd_table;
		p[3] = (unsigned long) invalid_pmd_table;
		p[4] = (unsigned long) invalid_pmd_table;
		p[5] = (unsigned long) invalid_pmd_table;
		p[6] = (unsigned long) invalid_pmd_table;
		p[7] = (unsigned long) invalid_pmd_table;
		p += 8;
	}
}

void pmd_init(unsigned long addr, unsigned long pagetable)
{
	unsigned long *p, *end;

 	p = (unsigned long *) addr;
	end = p + PTRS_PER_PMD;

	while (p < end) {
		p[0] = (unsigned long)pagetable;
		p[1] = (unsigned long)pagetable;
		p[2] = (unsigned long)pagetable;
		p[3] = (unsigned long)pagetable;
		p[4] = (unsigned long)pagetable;
		p[5] = (unsigned long)pagetable;
		p[6] = (unsigned long)pagetable;
		p[7] = (unsigned long)pagetable;
		p += 8;
	}
}

void __init pagetable_init(void)
{
	unsigned long vaddr;
	pgd_t *pgd_base;

	/* Initialize the entire pgd.  */
	pgd_init((unsigned long)swapper_pg_dir);
#ifdef MODULE_START
	pgd_init((unsigned long)module_pg_dir);
#endif
	pmd_init((unsigned long)invalid_pmd_table, (unsigned long)invalid_pte_table);

	pgd_base = swapper_pg_dir;
	/*
	 * Fixed mappings:
	 */
	vaddr = __fix_to_virt(__end_of_fixed_addresses - 1) & PMD_MASK;
	fixrange_init(vaddr, 0, pgd_base);
}
