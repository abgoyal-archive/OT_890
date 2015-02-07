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
 *  linux/arch/arm/mm/nommu.c
 *
 * ARM uCLinux supporting functions.
 */
#include <linux/module.h>
#include <linux/mm.h>
#include <linux/pagemap.h>
#include <linux/bootmem.h>
#include <linux/io.h>

#include <asm/cacheflush.h>
#include <asm/sections.h>
#include <asm/page.h>
#include <asm/mach/arch.h>

#include "mm.h"

/*
 * Reserve the various regions of node 0
 */
void __init reserve_node_zero(pg_data_t *pgdat)
{
	/*
	 * Register the kernel text and data with bootmem.
	 * Note that this can only be in node 0.
	 */
#ifdef CONFIG_XIP_KERNEL
	reserve_bootmem_node(pgdat, __pa(_data), _end - _data,
			BOOTMEM_DEFAULT);
#else
	reserve_bootmem_node(pgdat, __pa(_stext), _end - _stext,
			BOOTMEM_DEFAULT);
#endif

	/*
	 * Register the exception vector page.
	 * some architectures which the DRAM is the exception vector to trap,
	 * alloc_page breaks with error, although it is not NULL, but "0."
	 */
	reserve_bootmem_node(pgdat, CONFIG_VECTORS_BASE, PAGE_SIZE,
			BOOTMEM_DEFAULT);
}

/*
 * paging_init() sets up the page tables, initialises the zone memory
 * maps, and sets up the zero page, bad page and bad page tables.
 */
void __init paging_init(struct machine_desc *mdesc)
{
	bootmem_init();
}

/*
 * We don't need to do anything here for nommu machines.
 */
void setup_mm_for_reboot(char mode)
{
}

void flush_dcache_page(struct page *page)
{
	__cpuc_flush_dcache_page(page_address(page));
}
EXPORT_SYMBOL(flush_dcache_page);

void __iomem *__arm_ioremap_pfn(unsigned long pfn, unsigned long offset,
				size_t size, unsigned int mtype)
{
	if (pfn >= (0x100000000ULL >> PAGE_SHIFT))
		return NULL;
	return (void __iomem *) (offset + (pfn << PAGE_SHIFT));
}
EXPORT_SYMBOL(__arm_ioremap_pfn);

void __iomem *__arm_ioremap(unsigned long phys_addr, size_t size,
			    unsigned int mtype)
{
	return (void __iomem *)phys_addr;
}
EXPORT_SYMBOL(__arm_ioremap);

void __iounmap(volatile void __iomem *addr)
{
}
EXPORT_SYMBOL(__iounmap);
