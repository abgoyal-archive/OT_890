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
 * linux/arch/m68k/mm/sun3mmu.c
 *
 * Implementations of mm routines specific to the sun3 MMU.
 *
 * Moved here 8/20/1999 Sam Creasey
 *
 */

#include <linux/signal.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/swap.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/types.h>
#include <linux/init.h>
#include <linux/bootmem.h>

#include <asm/setup.h>
#include <asm/uaccess.h>
#include <asm/page.h>
#include <asm/pgtable.h>
#include <asm/system.h>
#include <asm/machdep.h>
#include <asm/io.h>

extern void mmu_emu_init (unsigned long bootmem_end);

const char bad_pmd_string[] = "Bad pmd in pte_alloc: %08lx\n";

extern unsigned long num_pages;

void free_initmem(void)
{
}

/* For the sun3 we try to follow the i386 paging_init() more closely */
/* start_mem and end_mem have PAGE_OFFSET added already */
/* now sets up tables using sun3 PTEs rather than i386 as before. --m */
void __init paging_init(void)
{
	pgd_t * pg_dir;
	pte_t * pg_table;
	int i;
	unsigned long address;
	unsigned long next_pgtable;
	unsigned long bootmem_end;
	unsigned long zones_size[MAX_NR_ZONES] = { 0, };
	unsigned long size;

#ifdef TEST_VERIFY_AREA
	wp_works_ok = 0;
#endif
	empty_zero_page = alloc_bootmem_pages(PAGE_SIZE);

	address = PAGE_OFFSET;
	pg_dir = swapper_pg_dir;
	memset (swapper_pg_dir, 0, sizeof (swapper_pg_dir));
	memset (kernel_pg_dir,  0, sizeof (kernel_pg_dir));

	size = num_pages * sizeof(pte_t);
	size = (size + PAGE_SIZE) & ~(PAGE_SIZE-1);

	next_pgtable = (unsigned long)alloc_bootmem_pages(size);
	bootmem_end = (next_pgtable + size + PAGE_SIZE) & PAGE_MASK;

	/* Map whole memory from PAGE_OFFSET (0x0E000000) */
	pg_dir += PAGE_OFFSET >> PGDIR_SHIFT;

	while (address < (unsigned long)high_memory) {
		pg_table = (pte_t *) __pa (next_pgtable);
		next_pgtable += PTRS_PER_PTE * sizeof (pte_t);
		pgd_val(*pg_dir) = (unsigned long) pg_table;
		pg_dir++;

		/* now change pg_table to kernel virtual addresses */
		pg_table = (pte_t *) __va ((unsigned long) pg_table);
		for (i=0; i<PTRS_PER_PTE; ++i, ++pg_table) {
			pte_t pte = pfn_pte(virt_to_pfn(address), PAGE_INIT);
			if (address >= (unsigned long)high_memory)
				pte_val (pte) = 0;
			set_pte (pg_table, pte);
			address += PAGE_SIZE;
		}
	}

	mmu_emu_init(bootmem_end);

	current->mm = NULL;

	/* memory sizing is a hack stolen from motorola.c..  hope it works for us */
	zones_size[ZONE_DMA] = ((unsigned long)high_memory - PAGE_OFFSET) >> PAGE_SHIFT;

	/* I really wish I knew why the following change made things better...  -- Sam */
/*	free_area_init(zones_size); */
	free_area_init_node(0, zones_size,
			    (__pa(PAGE_OFFSET) >> PAGE_SHIFT) + 1, NULL);


}


