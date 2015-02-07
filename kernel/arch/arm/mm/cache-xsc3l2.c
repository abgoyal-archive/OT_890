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
 * arch/arm/mm/cache-xsc3l2.c - XScale3 L2 cache controller support
 *
 * Copyright (C) 2007 ARM Limited
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */
#include <linux/init.h>
#include <linux/spinlock.h>
#include <linux/io.h>

#include <asm/system.h>
#include <asm/cputype.h>
#include <asm/cacheflush.h>

#define CR_L2	(1 << 26)

#define CACHE_LINE_SIZE		32
#define CACHE_LINE_SHIFT	5
#define CACHE_WAY_PER_SET	8

#define CACHE_WAY_SIZE(l2ctype)	(8192 << (((l2ctype) >> 8) & 0xf))
#define CACHE_SET_SIZE(l2ctype)	(CACHE_WAY_SIZE(l2ctype) >> CACHE_LINE_SHIFT)

static inline int xsc3_l2_present(void)
{
	unsigned long l2ctype;

	__asm__("mrc p15, 1, %0, c0, c0, 1" : "=r" (l2ctype));

	return !!(l2ctype & 0xf8);
}

static inline void xsc3_l2_clean_mva(unsigned long addr)
{
	__asm__("mcr p15, 1, %0, c7, c11, 1" : : "r" (addr));
}

static inline void xsc3_l2_clean_pa(unsigned long addr)
{
	xsc3_l2_clean_mva(__phys_to_virt(addr));
}

static inline void xsc3_l2_inv_mva(unsigned long addr)
{
	__asm__("mcr p15, 1, %0, c7, c7, 1" : : "r" (addr));
}

static inline void xsc3_l2_inv_pa(unsigned long addr)
{
	xsc3_l2_inv_mva(__phys_to_virt(addr));
}

static inline void xsc3_l2_inv_all(void)
{
	unsigned long l2ctype, set_way;
	int set, way;

	__asm__("mrc p15, 1, %0, c0, c0, 1" : "=r" (l2ctype));

	for (set = 0; set < CACHE_SET_SIZE(l2ctype); set++) {
		for (way = 0; way < CACHE_WAY_PER_SET; way++) {
			set_way = (way << 29) | (set << 5);
			__asm__("mcr p15, 1, %0, c7, c11, 2" : : "r"(set_way));
		}
	}

	dsb();
}

static void xsc3_l2_inv_range(unsigned long start, unsigned long end)
{
	if (start == 0 && end == -1ul) {
		xsc3_l2_inv_all();
		return;
	}

	/*
	 * Clean and invalidate partial first cache line.
	 */
	if (start & (CACHE_LINE_SIZE - 1)) {
		xsc3_l2_clean_pa(start & ~(CACHE_LINE_SIZE - 1));
		xsc3_l2_inv_pa(start & ~(CACHE_LINE_SIZE - 1));
		start = (start | (CACHE_LINE_SIZE - 1)) + 1;
	}

	/*
	 * Clean and invalidate partial last cache line.
	 */
	if (start < end && (end & (CACHE_LINE_SIZE - 1))) {
		xsc3_l2_clean_pa(end & ~(CACHE_LINE_SIZE - 1));
		xsc3_l2_inv_pa(end & ~(CACHE_LINE_SIZE - 1));
		end &= ~(CACHE_LINE_SIZE - 1);
	}

	/*
	 * Invalidate all full cache lines between 'start' and 'end'.
	 */
	while (start < end) {
		xsc3_l2_inv_pa(start);
		start += CACHE_LINE_SIZE;
	}

	dsb();
}

static void xsc3_l2_clean_range(unsigned long start, unsigned long end)
{
	start &= ~(CACHE_LINE_SIZE - 1);
	while (start < end) {
		xsc3_l2_clean_pa(start);
		start += CACHE_LINE_SIZE;
	}

	dsb();
}

/*
 * optimize L2 flush all operation by set/way format
 */
static inline void xsc3_l2_flush_all(void)
{
	unsigned long l2ctype, set_way;
	int set, way;

	__asm__("mrc p15, 1, %0, c0, c0, 1" : "=r" (l2ctype));

	for (set = 0; set < CACHE_SET_SIZE(l2ctype); set++) {
		for (way = 0; way < CACHE_WAY_PER_SET; way++) {
			set_way = (way << 29) | (set << 5);
			__asm__("mcr p15, 1, %0, c7, c15, 2" : : "r"(set_way));
		}
	}

	dsb();
}

static void xsc3_l2_flush_range(unsigned long start, unsigned long end)
{
	if (start == 0 && end == -1ul) {
		xsc3_l2_flush_all();
		return;
	}

	start &= ~(CACHE_LINE_SIZE - 1);
	while (start < end) {
		xsc3_l2_clean_pa(start);
		xsc3_l2_inv_pa(start);
		start += CACHE_LINE_SIZE;
	}

	dsb();
}

static int __init xsc3_l2_init(void)
{
	if (!cpu_is_xsc3() || !xsc3_l2_present())
		return 0;

	if (!(get_cr() & CR_L2)) {
		pr_info("XScale3 L2 cache enabled.\n");
		adjust_cr(CR_L2, CR_L2);
		xsc3_l2_inv_all();
	}

	outer_cache.inv_range = xsc3_l2_inv_range;
	outer_cache.clean_range = xsc3_l2_clean_range;
	outer_cache.flush_range = xsc3_l2_flush_range;

	return 0;
}
core_initcall(xsc3_l2_init);
