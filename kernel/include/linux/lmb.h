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

#ifndef _LINUX_LMB_H
#define _LINUX_LMB_H
#ifdef __KERNEL__

/*
 * Logical memory blocks.
 *
 * Copyright (C) 2001 Peter Bergner, IBM Corp.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#include <linux/init.h>
#include <linux/mm.h>

#define MAX_LMB_REGIONS 128

struct lmb_property {
	u64 base;
	u64 size;
};

struct lmb_region {
	unsigned long cnt;
	u64 size;
	struct lmb_property region[MAX_LMB_REGIONS+1];
};

struct lmb {
	unsigned long debug;
	u64 rmo_size;
	struct lmb_region memory;
	struct lmb_region reserved;
};

extern struct lmb lmb;

extern void __init lmb_init(void);
extern void __init lmb_analyze(void);
extern long lmb_add(u64 base, u64 size);
extern long lmb_remove(u64 base, u64 size);
extern long __init lmb_reserve(u64 base, u64 size);
extern u64 __init lmb_alloc_nid(u64 size, u64 align, int nid,
				u64 (*nid_range)(u64, u64, int *));
extern u64 __init lmb_alloc(u64 size, u64 align);
extern u64 __init lmb_alloc_base(u64 size,
		u64, u64 max_addr);
extern u64 __init __lmb_alloc_base(u64 size,
		u64 align, u64 max_addr);
extern u64 __init lmb_phys_mem_size(void);
extern u64 __init lmb_end_of_DRAM(void);
extern void __init lmb_enforce_memory_limit(u64 memory_limit);
extern int __init lmb_is_reserved(u64 addr);
extern int lmb_find(struct lmb_property *res);

extern void lmb_dump_all(void);

static inline u64
lmb_size_bytes(struct lmb_region *type, unsigned long region_nr)
{
	return type->region[region_nr].size;
}
static inline u64
lmb_size_pages(struct lmb_region *type, unsigned long region_nr)
{
	return lmb_size_bytes(type, region_nr) >> PAGE_SHIFT;
}
static inline u64
lmb_start_pfn(struct lmb_region *type, unsigned long region_nr)
{
	return type->region[region_nr].base >> PAGE_SHIFT;
}
static inline u64
lmb_end_pfn(struct lmb_region *type, unsigned long region_nr)
{
	return lmb_start_pfn(type, region_nr) +
	       lmb_size_pages(type, region_nr);
}

#include <asm/lmb.h>

#endif /* __KERNEL__ */

#endif /* _LINUX_LMB_H */
