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

#ifndef _ASM_POWERPC_ABS_ADDR_H
#define _ASM_POWERPC_ABS_ADDR_H
#ifdef __KERNEL__


/*
 * c 2001 PPC 64 Team, IBM Corp
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#include <linux/lmb.h>

#include <asm/types.h>
#include <asm/page.h>
#include <asm/prom.h>
#include <asm/firmware.h>

struct mschunks_map {
        unsigned long num_chunks;
        unsigned long chunk_size;
        unsigned long chunk_shift;
        unsigned long chunk_mask;
        u32 *mapping;
};

extern struct mschunks_map mschunks_map;

/* Chunks are 256 KB */
#define MSCHUNKS_CHUNK_SHIFT	(18)
#define MSCHUNKS_CHUNK_SIZE	(1UL << MSCHUNKS_CHUNK_SHIFT)
#define MSCHUNKS_OFFSET_MASK	(MSCHUNKS_CHUNK_SIZE - 1)

static inline unsigned long chunk_to_addr(unsigned long chunk)
{
	return chunk << MSCHUNKS_CHUNK_SHIFT;
}

static inline unsigned long addr_to_chunk(unsigned long addr)
{
	return addr >> MSCHUNKS_CHUNK_SHIFT;
}

static inline unsigned long phys_to_abs(unsigned long pa)
{
	unsigned long chunk;

	/* This is a no-op on non-iSeries */
	if (!firmware_has_feature(FW_FEATURE_ISERIES))
		return pa;

	chunk = addr_to_chunk(pa);

	if (chunk < mschunks_map.num_chunks)
		chunk = mschunks_map.mapping[chunk];

	return chunk_to_addr(chunk) + (pa & MSCHUNKS_OFFSET_MASK);
}

/* Convenience macros */
#define virt_to_abs(va) phys_to_abs(__pa(va))
#define abs_to_virt(aa) __va(aa)

/*
 * Converts Virtual Address to Real Address for
 * Legacy iSeries Hypervisor calls
 */
#define iseries_hv_addr(virtaddr)	\
	(0x8000000000000000 | virt_to_abs(virtaddr))

#endif /* __KERNEL__ */
#endif /* _ASM_POWERPC_ABS_ADDR_H */
