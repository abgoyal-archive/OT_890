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

/* MN10300 CPU core caching routines
 *
 * Copyright (C) 2007 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version
 * 2 of the Licence, or (at your option) any later version.
 */

#include <linux/sys.h>
#include <linux/linkage.h>
#include <asm/smp.h>
#include <asm/page.h>
#include <asm/cache.h>

	.am33_2
	.globl mn10300_dcache_flush
	.globl mn10300_dcache_flush_page
	.globl mn10300_dcache_flush_range
	.globl mn10300_dcache_flush_range2
	.globl mn10300_dcache_flush_inv
	.globl mn10300_dcache_flush_inv_page
	.globl mn10300_dcache_flush_inv_range
	.globl mn10300_dcache_flush_inv_range2

###############################################################################
#
# void mn10300_dcache_flush(void)
# Flush the entire data cache back to RAM
#
###############################################################################
	ALIGN
mn10300_dcache_flush:
	movhu	(CHCTR),d0
	btst	CHCTR_DCEN,d0
	beq	mn10300_dcache_flush_end

	# read the addresses tagged in the cache's tag RAM and attempt to flush
	# those addresses specifically
	# - we rely on the hardware to filter out invalid tag entry addresses
	mov	DCACHE_TAG(0,0),a0		# dcache tag RAM access address
	mov	DCACHE_PURGE(0,0),a1		# dcache purge request address
	mov	L1_CACHE_NWAYS*L1_CACHE_NENTRIES,d1  # total number of entries

mn10300_dcache_flush_loop:
	mov	(a0),d0
	and	L1_CACHE_TAG_ADDRESS|L1_CACHE_TAG_ENTRY,d0
	or	L1_CACHE_TAG_VALID,d0		# retain valid entries in the
						# cache
	mov	d0,(a1)				# conditional purge

mn10300_dcache_flush_skip:
	add	L1_CACHE_BYTES,a0
	add	L1_CACHE_BYTES,a1
	add	-1,d1
	bne	mn10300_dcache_flush_loop

mn10300_dcache_flush_end:
	ret	[],0

###############################################################################
#
# void mn10300_dcache_flush_page(unsigned start)
# void mn10300_dcache_flush_range(unsigned start, unsigned end)
# void mn10300_dcache_flush_range2(unsigned start, unsigned size)
# Flush a range of addresses on a page in the dcache
#
###############################################################################
	ALIGN
mn10300_dcache_flush_page:
	mov	PAGE_SIZE,d1
mn10300_dcache_flush_range2:
	add	d0,d1
mn10300_dcache_flush_range:
	movm	[d2,d3],(sp)

	movhu	(CHCTR),d2
	btst	CHCTR_DCEN,d2
	beq	mn10300_dcache_flush_range_end

	# round start addr down
	and	L1_CACHE_TAG_ADDRESS|L1_CACHE_TAG_ENTRY,d0
	mov	d0,a1

	add	L1_CACHE_BYTES,d1			# round end addr up
	and	L1_CACHE_TAG_ADDRESS|L1_CACHE_TAG_ENTRY,d1

	# write a request to flush all instances of an address from the cache
	mov	DCACHE_PURGE(0,0),a0
	mov	a1,d0
	and	L1_CACHE_TAG_ENTRY,d0
	add	d0,a0				# starting dcache purge control
						# reg address

	sub	a1,d1
	lsr	L1_CACHE_SHIFT,d1		# total number of entries to
						# examine

	or	L1_CACHE_TAG_VALID,a1		# retain valid entries in the
						# cache

mn10300_dcache_flush_range_loop:
	mov	a1,(L1_CACHE_WAYDISP*0,a0)	# conditionally purge this line
						# all ways

	add	L1_CACHE_BYTES,a0
	add	L1_CACHE_BYTES,a1
	and	~L1_CACHE_WAYDISP,a0		# make sure way stay on way 0
	add	-1,d1
	bne	mn10300_dcache_flush_range_loop

mn10300_dcache_flush_range_end:
	ret	[d2,d3],8

###############################################################################
#
# void mn10300_dcache_flush_inv(void)
# Flush the entire data cache and invalidate all entries
#
###############################################################################
	ALIGN
mn10300_dcache_flush_inv:
	movhu	(CHCTR),d0
	btst	CHCTR_DCEN,d0
	beq	mn10300_dcache_flush_inv_end

	# hit each line in the dcache with an unconditional purge
	mov	DCACHE_PURGE(0,0),a1		# dcache purge request address
	mov	L1_CACHE_NWAYS*L1_CACHE_NENTRIES,d1  # total number of entries

mn10300_dcache_flush_inv_loop:
	mov	(a1),d0				# unconditional purge

	add	L1_CACHE_BYTES,a1
	add	-1,d1
	bne	mn10300_dcache_flush_inv_loop

mn10300_dcache_flush_inv_end:
	ret	[],0

###############################################################################
#
# void mn10300_dcache_flush_inv_page(unsigned start)
# void mn10300_dcache_flush_inv_range(unsigned start, unsigned end)
# void mn10300_dcache_flush_inv_range2(unsigned start, unsigned size)
# Flush and invalidate a range of addresses on a page in the dcache
#
###############################################################################
	ALIGN
mn10300_dcache_flush_inv_page:
	mov	PAGE_SIZE,d1
mn10300_dcache_flush_inv_range2:
	add	d0,d1
mn10300_dcache_flush_inv_range:
	movm	[d2,d3],(sp)
	movhu	(CHCTR),d2
	btst	CHCTR_DCEN,d2
	beq	mn10300_dcache_flush_inv_range_end

	and	L1_CACHE_TAG_ADDRESS|L1_CACHE_TAG_ENTRY,d0	# round start
								# addr down
	mov	d0,a1

	add	L1_CACHE_BYTES,d1			# round end addr up
	and	L1_CACHE_TAG_ADDRESS|L1_CACHE_TAG_ENTRY,d1

	# write a request to flush and invalidate all instances of an address
	# from the cache
	mov	DCACHE_PURGE(0,0),a0
	mov	a1,d0
	and	L1_CACHE_TAG_ENTRY,d0
	add	d0,a0				# starting dcache purge control
						# reg address

	sub	a1,d1
	lsr	L1_CACHE_SHIFT,d1		# total number of entries to
						# examine

mn10300_dcache_flush_inv_range_loop:
	mov	a1,(L1_CACHE_WAYDISP*0,a0)	# conditionally purge this line
						# in all ways

	add	L1_CACHE_BYTES,a0
	add	L1_CACHE_BYTES,a1
	and	~L1_CACHE_WAYDISP,a0		# make sure way stay on way 0
	add	-1,d1
	bne	mn10300_dcache_flush_inv_range_loop

mn10300_dcache_flush_inv_range_end:
	ret	[d2,d3],8
