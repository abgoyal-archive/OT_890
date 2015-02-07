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
 * arch/sh/mm/cache-sh3.c
 *
 * Copyright (C) 1999, 2000  Niibe Yutaka
 * Copyright (C) 2002 Paul Mundt
 *
 * Released under the terms of the GNU GPL v2.0.
 */

#include <linux/init.h>
#include <linux/mman.h>
#include <linux/mm.h>
#include <linux/threads.h>
#include <asm/addrspace.h>
#include <asm/page.h>
#include <asm/pgtable.h>
#include <asm/processor.h>
#include <asm/cache.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/pgalloc.h>
#include <asm/mmu_context.h>
#include <asm/cacheflush.h>

/*
 * Write back the dirty D-caches, but not invalidate them.
 *
 * Is this really worth it, or should we just alias this routine
 * to __flush_purge_region too?
 *
 * START: Virtual Address (U0, P1, or P3)
 * SIZE: Size of the region.
 */

void __flush_wback_region(void *start, int size)
{
	unsigned long v, j;
	unsigned long begin, end;
	unsigned long flags;

	begin = (unsigned long)start & ~(L1_CACHE_BYTES-1);
	end = ((unsigned long)start + size + L1_CACHE_BYTES-1)
		& ~(L1_CACHE_BYTES-1);

	for (v = begin; v < end; v+=L1_CACHE_BYTES) {
		unsigned long addrstart = CACHE_OC_ADDRESS_ARRAY;
		for (j = 0; j < current_cpu_data.dcache.ways; j++) {
			unsigned long data, addr, p;

			p = __pa(v);
			addr = addrstart | (v & current_cpu_data.dcache.entry_mask);
			local_irq_save(flags);
			data = ctrl_inl(addr);

			if ((data & CACHE_PHYSADDR_MASK) ==
			    (p & CACHE_PHYSADDR_MASK)) {
				data &= ~SH_CACHE_UPDATED;
				ctrl_outl(data, addr);
				local_irq_restore(flags);
				break;
			}
			local_irq_restore(flags);
			addrstart += current_cpu_data.dcache.way_incr;
		}
	}
}

/*
 * Write back the dirty D-caches and invalidate them.
 *
 * START: Virtual Address (U0, P1, or P3)
 * SIZE: Size of the region.
 */
void __flush_purge_region(void *start, int size)
{
	unsigned long v;
	unsigned long begin, end;

	begin = (unsigned long)start & ~(L1_CACHE_BYTES-1);
	end = ((unsigned long)start + size + L1_CACHE_BYTES-1)
		& ~(L1_CACHE_BYTES-1);

	for (v = begin; v < end; v+=L1_CACHE_BYTES) {
		unsigned long data, addr;

		data = (v & 0xfffffc00); /* _Virtual_ address, ~U, ~V */
		addr = CACHE_OC_ADDRESS_ARRAY |
			(v & current_cpu_data.dcache.entry_mask) | SH_CACHE_ASSOC;
		ctrl_outl(data, addr);
	}
}

/*
 * No write back please
 *
 * Except I don't think there's any way to avoid the writeback. So we
 * just alias it to __flush_purge_region(). dwmw2.
 */
void __flush_invalidate_region(void *start, int size)
	__attribute__((alias("__flush_purge_region")));
