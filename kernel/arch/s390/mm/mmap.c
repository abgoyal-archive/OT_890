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
 *  linux/arch/s390/mm/mmap.c
 *
 *  flexible mmap layout support
 *
 * Copyright 2003-2004 Red Hat Inc., Durham, North Carolina.
 * All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *
 * Started by Ingo Molnar <mingo@elte.hu>
 */

#include <linux/personality.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <asm/pgalloc.h>

/*
 * Top of mmap area (just below the process stack).
 *
 * Leave an at least ~128 MB hole.
 */
#define MIN_GAP (128*1024*1024)
#define MAX_GAP (STACK_TOP/6*5)

static inline unsigned long mmap_base(void)
{
	unsigned long gap = current->signal->rlim[RLIMIT_STACK].rlim_cur;

	if (gap < MIN_GAP)
		gap = MIN_GAP;
	else if (gap > MAX_GAP)
		gap = MAX_GAP;

	return STACK_TOP - (gap & PAGE_MASK);
}

static inline int mmap_is_legacy(void)
{
#ifdef CONFIG_64BIT
	/*
	 * Force standard allocation for 64 bit programs.
	 */
	if (!test_thread_flag(TIF_31BIT))
		return 1;
#endif
	return sysctl_legacy_va_layout ||
	    (current->personality & ADDR_COMPAT_LAYOUT) ||
	    current->signal->rlim[RLIMIT_STACK].rlim_cur == RLIM_INFINITY;
}

#ifndef CONFIG_64BIT

/*
 * This function, called very early during the creation of a new
 * process VM image, sets up which VM layout function to use:
 */
void arch_pick_mmap_layout(struct mm_struct *mm)
{
	/*
	 * Fall back to the standard layout if the personality
	 * bit is set, or if the expected stack growth is unlimited:
	 */
	if (mmap_is_legacy()) {
		mm->mmap_base = TASK_UNMAPPED_BASE;
		mm->get_unmapped_area = arch_get_unmapped_area;
		mm->unmap_area = arch_unmap_area;
	} else {
		mm->mmap_base = mmap_base();
		mm->get_unmapped_area = arch_get_unmapped_area_topdown;
		mm->unmap_area = arch_unmap_area_topdown;
	}
}
EXPORT_SYMBOL_GPL(arch_pick_mmap_layout);

#else

int s390_mmap_check(unsigned long addr, unsigned long len)
{
	if (!test_thread_flag(TIF_31BIT) &&
	    len >= TASK_SIZE && TASK_SIZE < (1UL << 53))
		return crst_table_upgrade(current->mm, 1UL << 53);
	return 0;
}

static unsigned long
s390_get_unmapped_area(struct file *filp, unsigned long addr,
		unsigned long len, unsigned long pgoff, unsigned long flags)
{
	struct mm_struct *mm = current->mm;
	unsigned long area;
	int rc;

	area = arch_get_unmapped_area(filp, addr, len, pgoff, flags);
	if (!(area & ~PAGE_MASK))
		return area;
	if (area == -ENOMEM &&
	    !test_thread_flag(TIF_31BIT) && TASK_SIZE < (1UL << 53)) {
		/* Upgrade the page table to 4 levels and retry. */
		rc = crst_table_upgrade(mm, 1UL << 53);
		if (rc)
			return (unsigned long) rc;
		area = arch_get_unmapped_area(filp, addr, len, pgoff, flags);
	}
	return area;
}

static unsigned long
s390_get_unmapped_area_topdown(struct file *filp, const unsigned long addr,
			  const unsigned long len, const unsigned long pgoff,
			  const unsigned long flags)
{
	struct mm_struct *mm = current->mm;
	unsigned long area;
	int rc;

	area = arch_get_unmapped_area_topdown(filp, addr, len, pgoff, flags);
	if (!(area & ~PAGE_MASK))
		return area;
	if (area == -ENOMEM &&
	    !test_thread_flag(TIF_31BIT) && TASK_SIZE < (1UL << 53)) {
		/* Upgrade the page table to 4 levels and retry. */
		rc = crst_table_upgrade(mm, 1UL << 53);
		if (rc)
			return (unsigned long) rc;
		area = arch_get_unmapped_area_topdown(filp, addr, len,
						      pgoff, flags);
	}
	return area;
}
/*
 * This function, called very early during the creation of a new
 * process VM image, sets up which VM layout function to use:
 */
void arch_pick_mmap_layout(struct mm_struct *mm)
{
	/*
	 * Fall back to the standard layout if the personality
	 * bit is set, or if the expected stack growth is unlimited:
	 */
	if (mmap_is_legacy()) {
		mm->mmap_base = TASK_UNMAPPED_BASE;
		mm->get_unmapped_area = s390_get_unmapped_area;
		mm->unmap_area = arch_unmap_area;
	} else {
		mm->mmap_base = mmap_base();
		mm->get_unmapped_area = s390_get_unmapped_area_topdown;
		mm->unmap_area = arch_unmap_area_topdown;
	}
}
EXPORT_SYMBOL_GPL(arch_pick_mmap_layout);

#endif
