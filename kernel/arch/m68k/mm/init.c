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
 *  linux/arch/m68k/mm/init.c
 *
 *  Copyright (C) 1995  Hamish Macdonald
 *
 *  Contains common initialization routines, specific init code moved
 *  to motorola.c and sun3mmu.c
 */

#include <linux/module.h>
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
#include <asm/pgalloc.h>
#include <asm/system.h>
#include <asm/machdep.h>
#include <asm/io.h>
#ifdef CONFIG_ATARI
#include <asm/atari_stram.h>
#endif
#include <asm/sections.h>
#include <asm/tlb.h>

DEFINE_PER_CPU(struct mmu_gather, mmu_gathers);

pg_data_t pg_data_map[MAX_NUMNODES];
EXPORT_SYMBOL(pg_data_map);

int m68k_virt_to_node_shift;

#ifndef CONFIG_SINGLE_MEMORY_CHUNK
pg_data_t *pg_data_table[65];
EXPORT_SYMBOL(pg_data_table);
#endif

void __init m68k_setup_node(int node)
{
#ifndef CONFIG_SINGLE_MEMORY_CHUNK
	struct mem_info *info = m68k_memory + node;
	int i, end;

	i = (unsigned long)phys_to_virt(info->addr) >> __virt_to_node_shift();
	end = (unsigned long)phys_to_virt(info->addr + info->size - 1) >> __virt_to_node_shift();
	for (; i <= end; i++) {
		if (pg_data_table[i])
			printk("overlap at %u for chunk %u\n", i, node);
		pg_data_table[i] = pg_data_map + node;
	}
#endif
	pg_data_map[node].bdata = bootmem_node_data + node;
	node_set_online(node);
}


/*
 * ZERO_PAGE is a special page that is used for zero-initialized
 * data and COW.
 */

void *empty_zero_page;
EXPORT_SYMBOL(empty_zero_page);

extern void init_pointer_table(unsigned long ptable);

/* References to section boundaries */

extern pmd_t *zero_pgtable;

void __init mem_init(void)
{
	pg_data_t *pgdat;
	int codepages = 0;
	int datapages = 0;
	int initpages = 0;
	int i;

#ifdef CONFIG_ATARI
	if (MACH_IS_ATARI)
		atari_stram_mem_init_hook();
#endif

	/* this will put all memory onto the freelists */
	totalram_pages = num_physpages = 0;
	for_each_online_pgdat(pgdat) {
		num_physpages += pgdat->node_present_pages;

		totalram_pages += free_all_bootmem_node(pgdat);
		for (i = 0; i < pgdat->node_spanned_pages; i++) {
			struct page *page = pgdat->node_mem_map + i;
			char *addr = page_to_virt(page);

			if (!PageReserved(page))
				continue;
			if (addr >= _text &&
			    addr < _etext)
				codepages++;
			else if (addr >= __init_begin &&
				 addr < __init_end)
				initpages++;
			else
				datapages++;
		}
	}

#ifndef CONFIG_SUN3
	/* insert pointer tables allocated so far into the tablelist */
	init_pointer_table((unsigned long)kernel_pg_dir);
	for (i = 0; i < PTRS_PER_PGD; i++) {
		if (pgd_present(kernel_pg_dir[i]))
			init_pointer_table(__pgd_page(kernel_pg_dir[i]));
	}

	/* insert also pointer table that we used to unmap the zero page */
	if (zero_pgtable)
		init_pointer_table((unsigned long)zero_pgtable);
#endif

	printk("Memory: %luk/%luk available (%dk kernel code, %dk data, %dk init)\n",
	       (unsigned long)nr_free_pages() << (PAGE_SHIFT-10),
	       totalram_pages << (PAGE_SHIFT-10),
	       codepages << (PAGE_SHIFT-10),
	       datapages << (PAGE_SHIFT-10),
	       initpages << (PAGE_SHIFT-10));
}

#ifdef CONFIG_BLK_DEV_INITRD
void free_initrd_mem(unsigned long start, unsigned long end)
{
	int pages = 0;
	for (; start < end; start += PAGE_SIZE) {
		ClearPageReserved(virt_to_page(start));
		init_page_count(virt_to_page(start));
		free_page(start);
		totalram_pages++;
		pages++;
	}
	printk ("Freeing initrd memory: %dk freed\n", pages);
}
#endif
