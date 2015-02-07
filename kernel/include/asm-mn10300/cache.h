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

/* MN10300 cache management registers
 *
 * Copyright (C) 2007 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version
 * 2 of the Licence, or (at your option) any later version.
 */

#ifndef _ASM_CACHE_H
#define _ASM_CACHE_H

#include <asm/cpu-regs.h>
#include <asm/proc/cache.h>

#ifndef __ASSEMBLY__
#define L1_CACHE_DISPARITY	(L1_CACHE_NENTRIES * L1_CACHE_BYTES)
#else
#define L1_CACHE_DISPARITY	L1_CACHE_NENTRIES * L1_CACHE_BYTES
#endif

/* data cache purge registers
 * - read from the register to unconditionally purge that cache line
 * - write address & 0xffffff00 to conditionally purge that cache line
 *   - clear LSB to request invalidation as well
 */
#define DCACHE_PURGE(WAY, ENTRY) \
	__SYSREG(0xc8400000 + (WAY) * L1_CACHE_WAYDISP + \
		 (ENTRY) * L1_CACHE_BYTES, u32)

#define DCACHE_PURGE_WAY0(ENTRY) \
	__SYSREG(0xc8400000 + 0 * L1_CACHE_WAYDISP + (ENTRY) * L1_CACHE_BYTES, u32)
#define DCACHE_PURGE_WAY1(ENTRY) \
	__SYSREG(0xc8400000 + 1 * L1_CACHE_WAYDISP + (ENTRY) * L1_CACHE_BYTES, u32)
#define DCACHE_PURGE_WAY2(ENTRY) \
	__SYSREG(0xc8400000 + 2 * L1_CACHE_WAYDISP + (ENTRY) * L1_CACHE_BYTES, u32)
#define DCACHE_PURGE_WAY3(ENTRY) \
	__SYSREG(0xc8400000 + 3 * L1_CACHE_WAYDISP + (ENTRY) * L1_CACHE_BYTES, u32)

/* instruction cache access registers */
#define ICACHE_DATA(WAY, ENTRY, OFF) \
	__SYSREG(0xc8000000 + (WAY) * L1_CACHE_WAYDISP + (ENTRY) * 0x10 + (OFF) * 4, u32)
#define ICACHE_TAG(WAY, ENTRY)	 \
	__SYSREG(0xc8100000 + (WAY) * L1_CACHE_WAYDISP + (ENTRY) * 0x10, u32)

/* instruction cache access registers */
#define DCACHE_DATA(WAY, ENTRY, OFF) \
	__SYSREG(0xc8200000 + (WAY) * L1_CACHE_WAYDISP + (ENTRY) * 0x10 + (OFF) * 4, u32)
#define DCACHE_TAG(WAY, ENTRY)	 \
	__SYSREG(0xc8300000 + (WAY) * L1_CACHE_WAYDISP + (ENTRY) * 0x10, u32)

#endif /* _ASM_CACHE_H */
