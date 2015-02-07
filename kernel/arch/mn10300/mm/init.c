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

/* MN10300 Memory management initialisation
 *
 * Copyright (C) 2007 Matsushita Electric Industrial Co., Ltd.
 * Copyright (C) 2007 Red Hat, Inc. All Rights Reserved.
 * Modified by David Howells (dhowells@redhat.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version
 * 2 of the Licence, or (at your option) any later version.
 */
#include <linux/signal.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/types.h>
#include <linux/ptrace.h>
#include <linux/mman.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/swap.h>
#include <linux/smp.h>
#include <linux/init.h>
#include <linux/initrd.h>
#include <linux/highmem.h>
#include <linux/pagemap.h>
#include <linux/bootmem.h>

#include <asm/processor.h>
#include <asm/system.h>
#include <asm/uaccess.h>
#include <asm/pgtable.h>
#include <asm/pgalloc.h>
#include <asm/dma.h>
#include <asm/tlb.h>
#include <asm/sections.h>

DEFINE_PER_CPU(struct mmu_gather, mmu_gathers);

unsigned long highstart_pfn, highend_pfn;

/*
 * set up paging
 */
void __init paging_init(void)
{
	unsigned long zones_size[MAX_NR_ZONES] = {0,};
	pte_t *ppte;
	int loop;

	/* main kernel space -> RAM mapping is handled as 1:1 transparent by
	 * the MMU */
	memset(swapper_pg_dir, 0, sizeof(swapper_pg_dir));
	memset(kernel_vmalloc_ptes, 0, sizeof(kernel_vmalloc_ptes));

	/* load the VMALLOC area PTE table addresses into the kernel PGD */
	ppte = kernel_vmalloc_ptes;
	for (loop = VMALLOC_START / (PAGE_SIZE * PTRS_PER_PTE);
	     loop < VMALLOC_END / (PAGE_SIZE * PTRS_PER_PTE);
	     loop++
	     ) {
		set_pgd(swapper_pg_dir + loop, __pgd(__pa(ppte) | _PAGE_TABLE));
		ppte += PAGE_SIZE / sizeof(pte_t);
	}

	/* declare the sizes of the RAM zones (only use the normal zone) */
	zones_size[ZONE_NORMAL] =
		contig_page_data.bdata->node_low_pfn -
		contig_page_data.bdata->node_min_pfn;

	/* pass the memory from the bootmem allocator to the main allocator */
	free_area_init(zones_size);

	__flush_tlb_all();
}

/*
 * transfer all the memory from the bootmem allocator to the runtime allocator
 */
void __init mem_init(void)
{
	int codesize, reservedpages, datasize, initsize;
	int tmp;

	if (!mem_map)
		BUG();

#define START_PFN	(contig_page_data.bdata->node_min_pfn)
#define MAX_LOW_PFN	(contig_page_data.bdata->node_low_pfn)

	max_mapnr = num_physpages = MAX_LOW_PFN - START_PFN;
	high_memory = (void *) __va(MAX_LOW_PFN * PAGE_SIZE);

	/* clear the zero-page */
	memset(empty_zero_page, 0, PAGE_SIZE);

	/* this will put all low memory onto the freelists */
	totalram_pages += free_all_bootmem();

	reservedpages = 0;
	for (tmp = 0; tmp < num_physpages; tmp++)
		if (PageReserved(&mem_map[tmp]))
			reservedpages++;

	codesize =  (unsigned long) &_etext - (unsigned long) &_stext;
	datasize =  (unsigned long) &_edata - (unsigned long) &_etext;
	initsize =  (unsigned long) &__init_end - (unsigned long) &__init_begin;

	printk(KERN_INFO
	       "Memory: %luk/%luk available"
	       " (%dk kernel code, %dk reserved, %dk data, %dk init,"
	       " %ldk highmem)\n",
	       (unsigned long) nr_free_pages() << (PAGE_SHIFT - 10),
	       max_mapnr << (PAGE_SHIFT - 10),
	       codesize >> 10,
	       reservedpages << (PAGE_SHIFT - 10),
	       datasize >> 10,
	       initsize >> 10,
	       (unsigned long) (totalhigh_pages << (PAGE_SHIFT - 10))
	       );
}

/*
 *
 */
void free_init_pages(char *what, unsigned long begin, unsigned long end)
{
	unsigned long addr;

	for (addr = begin; addr < end; addr += PAGE_SIZE) {
		ClearPageReserved(virt_to_page(addr));
		init_page_count(virt_to_page(addr));
		memset((void *) addr, 0xcc, PAGE_SIZE);
		free_page(addr);
		totalram_pages++;
	}
	printk(KERN_INFO "Freeing %s: %ldk freed\n", what, (end - begin) >> 10);
}

/*
 * recycle memory containing stuff only required for initialisation
 */
void free_initmem(void)
{
	free_init_pages("unused kernel memory",
			(unsigned long) &__init_begin,
			(unsigned long) &__init_end);
}

/*
 * dispose of the memory on which the initial ramdisk resided
 */
#ifdef CONFIG_BLK_DEV_INITRD
void free_initrd_mem(unsigned long start, unsigned long end)
{
	free_init_pages("initrd memory", start, end);
}
#endif
