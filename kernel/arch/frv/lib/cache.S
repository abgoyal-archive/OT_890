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

/* cache.S: cache managment routines
 *
 * Copyright (C) 2004 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#include <asm/spr-regs.h>
#include <asm/cache.h>

        .text
        .p2align	4

###############################################################################
#
# Write back a range of dcache
# - void frv_dcache_writeback(unsigned long start [GR8], unsigned long size [GR9])
#
###############################################################################
        .globl		frv_dcache_writeback
        .type		frv_dcache_writeback,@function
frv_dcache_writeback:
	andi		gr8,~(L1_CACHE_BYTES-1),gr8

2:	dcf		@(gr8,gr0)
	addi		gr8,#L1_CACHE_BYTES,gr8
	cmp		gr9,gr8,icc0
	bhi		icc0,#2,2b

	membar
	bralr
	.size		frv_dcache_writeback, .-frv_dcache_writeback

##############################################################################
#
# Invalidate a range of dcache and icache
# - void frv_cache_invalidate(unsigned long start [GR8], unsigned long end [GR9]);
#
###############################################################################
        .globl		frv_cache_invalidate
        .type		frv_cache_invalidate,@function
frv_cache_invalidate:
	andi		gr8,~(L1_CACHE_BYTES-1),gr8

2:	dci		@(gr8,gr0)
	ici		@(gr8,gr0)
	addi		gr8,#L1_CACHE_BYTES,gr8
	cmp		gr9,gr8,icc0
	bhi		icc0,#2,2b

	membar
	bralr
	.size		frv_cache_invalidate, .-frv_cache_invalidate

##############################################################################
#
# Invalidate a range of icache
# - void frv_icache_invalidate(unsigned long start [GR8], unsigned long end [GR9]);
#
###############################################################################
        .globl		frv_icache_invalidate
        .type		frv_icache_invalidate,@function
frv_icache_invalidate:
	andi		gr8,~(L1_CACHE_BYTES-1),gr8

2:	ici		@(gr8,gr0)
	addi		gr8,#L1_CACHE_BYTES,gr8
	cmp		gr9,gr8,icc0
	bhi		icc0,#2,2b

	membar
	bralr
	.size		frv_icache_invalidate, .-frv_icache_invalidate

###############################################################################
#
# Write back and invalidate a range of dcache and icache
# - void frv_cache_wback_inv(unsigned long start [GR8], unsigned long end [GR9])
#
###############################################################################
        .globl		frv_cache_wback_inv
        .type		frv_cache_wback_inv,@function
frv_cache_wback_inv:
	andi		gr8,~(L1_CACHE_BYTES-1),gr8

2:	dcf		@(gr8,gr0)
	ici		@(gr8,gr0)
	addi		gr8,#L1_CACHE_BYTES,gr8
	cmp		gr9,gr8,icc0
	bhi		icc0,#2,2b

	membar
	bralr
	.size		frv_cache_wback_inv, .-frv_cache_wback_inv
