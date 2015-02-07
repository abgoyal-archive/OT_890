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
 * Copyright (C) 2004-2006 Atmel Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/highmem.h>
#include <linux/unistd.h>

#include <asm/cacheflush.h>
#include <asm/cachectl.h>
#include <asm/processor.h>
#include <asm/uaccess.h>
#include <asm/syscalls.h>

/*
 * If you attempt to flush anything more than this, you need superuser
 * privileges.  The value is completely arbitrary.
 */
#define CACHEFLUSH_MAX_LEN	1024

void invalidate_dcache_region(void *start, size_t size)
{
	unsigned long v, begin, end, linesz, mask;

	linesz = boot_cpu_data.dcache.linesz;
	mask = linesz - 1;

	/* when first and/or last cachelines are shared, flush them
	 * instead of invalidating ... never discard valid data!
	 */
	begin = (unsigned long)start;
	end = begin + size;

	if (begin & mask) {
		flush_dcache_line(start);
		begin += linesz;
	}
	if (end & mask) {
		flush_dcache_line((void *)end);
		end &= ~mask;
	}

	/* remaining cachelines only need invalidation */
	for (v = begin; v < end; v += linesz)
		invalidate_dcache_line((void *)v);
	flush_write_buffer();
}

void clean_dcache_region(void *start, size_t size)
{
	unsigned long v, begin, end, linesz;

	linesz = boot_cpu_data.dcache.linesz;
	begin = (unsigned long)start & ~(linesz - 1);
	end = ((unsigned long)start + size + linesz - 1) & ~(linesz - 1);

	for (v = begin; v < end; v += linesz)
		clean_dcache_line((void *)v);
	flush_write_buffer();
}

void flush_dcache_region(void *start, size_t size)
{
	unsigned long v, begin, end, linesz;

	linesz = boot_cpu_data.dcache.linesz;
	begin = (unsigned long)start & ~(linesz - 1);
	end = ((unsigned long)start + size + linesz - 1) & ~(linesz - 1);

	for (v = begin; v < end; v += linesz)
		flush_dcache_line((void *)v);
	flush_write_buffer();
}

void invalidate_icache_region(void *start, size_t size)
{
	unsigned long v, begin, end, linesz;

	linesz = boot_cpu_data.icache.linesz;
	begin = (unsigned long)start & ~(linesz - 1);
	end = ((unsigned long)start + size + linesz - 1) & ~(linesz - 1);

	for (v = begin; v < end; v += linesz)
		invalidate_icache_line((void *)v);
}

static inline void __flush_icache_range(unsigned long start, unsigned long end)
{
	unsigned long v, linesz;

	linesz = boot_cpu_data.dcache.linesz;
	for (v = start; v < end; v += linesz) {
		clean_dcache_line((void *)v);
		invalidate_icache_line((void *)v);
	}

	flush_write_buffer();
}

/*
 * This one is called after a module has been loaded.
 */
void flush_icache_range(unsigned long start, unsigned long end)
{
	unsigned long linesz;

	linesz = boot_cpu_data.dcache.linesz;
	__flush_icache_range(start & ~(linesz - 1),
			     (end + linesz - 1) & ~(linesz - 1));
}

/*
 * This one is called from do_no_page(), do_swap_page() and install_page().
 */
void flush_icache_page(struct vm_area_struct *vma, struct page *page)
{
	if (vma->vm_flags & VM_EXEC) {
		void *v = page_address(page);
		__flush_icache_range((unsigned long)v, (unsigned long)v + PAGE_SIZE);
	}
}

asmlinkage int sys_cacheflush(int operation, void __user *addr, size_t len)
{
	int ret;

	if (len > CACHEFLUSH_MAX_LEN) {
		ret = -EPERM;
		if (!capable(CAP_SYS_ADMIN))
			goto out;
	}

	ret = -EFAULT;
	if (!access_ok(VERIFY_WRITE, addr, len))
		goto out;

	switch (operation) {
	case CACHE_IFLUSH:
		flush_icache_range((unsigned long)addr,
				   (unsigned long)addr + len);
		ret = 0;
		break;
	default:
		ret = -EINVAL;
	}

out:
	return ret;
}

void copy_to_user_page(struct vm_area_struct *vma, struct page *page,
		unsigned long vaddr, void *dst, const void *src,
		unsigned long len)
{
	memcpy(dst, src, len);
	if (vma->vm_flags & VM_EXEC)
		flush_icache_range((unsigned long)dst,
				(unsigned long)dst + len);
}
