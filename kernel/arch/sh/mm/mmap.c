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
 * arch/sh/mm/mmap.c
 *
 * Copyright (C) 2008  Paul Mundt
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */
#include <linux/io.h>
#include <linux/mm.h>
#include <linux/mman.h>
#include <linux/module.h>
#include <asm/page.h>
#include <asm/processor.h>

#ifdef CONFIG_MMU
unsigned long shm_align_mask = PAGE_SIZE - 1;	/* Sane caches */
EXPORT_SYMBOL(shm_align_mask);

/*
 * To avoid cache aliases, we map the shared page with same color.
 */
#define COLOUR_ALIGN(addr, pgoff)				\
	((((addr) + shm_align_mask) & ~shm_align_mask) +	\
	 (((pgoff) << PAGE_SHIFT) & shm_align_mask))

unsigned long arch_get_unmapped_area(struct file *filp, unsigned long addr,
	unsigned long len, unsigned long pgoff, unsigned long flags)
{
	struct mm_struct *mm = current->mm;
	struct vm_area_struct *vma;
	unsigned long start_addr;
	int do_colour_align;

	if (flags & MAP_FIXED) {
		/* We do not accept a shared mapping if it would violate
		 * cache aliasing constraints.
		 */
		if ((flags & MAP_SHARED) && (addr & shm_align_mask))
			return -EINVAL;
		return addr;
	}

	if (unlikely(len > TASK_SIZE))
		return -ENOMEM;

	do_colour_align = 0;
	if (filp || (flags & MAP_SHARED))
		do_colour_align = 1;

	if (addr) {
		if (do_colour_align)
			addr = COLOUR_ALIGN(addr, pgoff);
		else
			addr = PAGE_ALIGN(addr);

		vma = find_vma(mm, addr);
		if (TASK_SIZE - len >= addr &&
		    (!vma || addr + len <= vma->vm_start))
			return addr;
	}

	if (len > mm->cached_hole_size) {
		start_addr = addr = mm->free_area_cache;
	} else {
	        mm->cached_hole_size = 0;
		start_addr = addr = TASK_UNMAPPED_BASE;
	}

full_search:
	if (do_colour_align)
		addr = COLOUR_ALIGN(addr, pgoff);
	else
		addr = PAGE_ALIGN(mm->free_area_cache);

	for (vma = find_vma(mm, addr); ; vma = vma->vm_next) {
		/* At this point:  (!vma || addr < vma->vm_end). */
		if (unlikely(TASK_SIZE - len < addr)) {
			/*
			 * Start a new search - just in case we missed
			 * some holes.
			 */
			if (start_addr != TASK_UNMAPPED_BASE) {
				start_addr = addr = TASK_UNMAPPED_BASE;
				mm->cached_hole_size = 0;
				goto full_search;
			}
			return -ENOMEM;
		}
		if (likely(!vma || addr + len <= vma->vm_start)) {
			/*
			 * Remember the place where we stopped the search:
			 */
			mm->free_area_cache = addr + len;
			return addr;
		}
		if (addr + mm->cached_hole_size < vma->vm_start)
		        mm->cached_hole_size = vma->vm_start - addr;

		addr = vma->vm_end;
		if (do_colour_align)
			addr = COLOUR_ALIGN(addr, pgoff);
	}
}
#endif /* CONFIG_MMU */

/*
 * You really shouldn't be using read() or write() on /dev/mem.  This
 * might go away in the future.
 */
int valid_phys_addr_range(unsigned long addr, size_t count)
{
	if (addr < __MEMORY_START)
		return 0;
	if (addr + count > __pa(high_memory))
		return 0;

	return 1;
}

int valid_mmap_phys_addr_range(unsigned long pfn, size_t size)
{
	return 1;
}
