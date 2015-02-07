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

/* Optimised simple memory fill
 *
 * Copyright (C) 2007 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version
 * 2 of the Licence, or (at your option) any later version.
 */
#include <asm/cache.h>

        .section .text
        .balign	L1_CACHE_BYTES

###############################################################################
#
# void *memset(void *dst, int c, size_t n)
#
###############################################################################
	.globl	memset
        .type	memset,@function
memset:
	movm	[d2,d3],(sp)
	mov	d0,(12,sp)
	mov	d1,(16,sp)
	mov	(20,sp),d2			# count
	mov	d0,a0				# dst
	mov	d0,e3				# the return value

	cmp	+0,d2
	beq	memset_done			# return if zero-length fill

	# see if the region parameters are four-byte aligned
	or	d0,d2,d3
	and	+3,d3
	bne	memset_1			# jump if not

	extbu	d1
	mov_asl	d1,d3,8,d1
	or_asl	d1,d3,8,d1
	or_asl	d1,d3,8,d1
	or	d3,d1

	# we want to transfer as much as we can in chunks of 32 bytes
	cmp	+31,d2
	bls	memset_4_remainder		# 4-byte aligned remainder

	add	-32,d2
	mov	+32,d3

memset_4_loop:
	mov	d1,(a0+)
	mov	d1,(a0+)
	mov	d1,(a0+)
	mov	d1,(a0+)
	mov	d1,(a0+)
	mov	d1,(a0+)
	mov	d1,(a0+)
	mov	d1,(a0+)

	sub	d3,d2
	bcc	memset_4_loop

	add	d3,d2
	beq	memset_4_no_remainder

memset_4_remainder:
	# cut 4-7 words down to 0-3
	cmp	+16,d2
	bcs	memset_4_three_or_fewer_words
	mov	d1,(a0+)
	mov	d1,(a0+)
	mov	d1,(a0+)
	mov	d1,(a0+)
	add	-16,d2
	beq	memset_4_no_remainder

	# copy the remaining 1, 2 or 3 words
memset_4_three_or_fewer_words:
	cmp	+8,d2
	bcs	memset_4_one_word
	beq	memset_4_two_words
	mov	d1,(a0+)
memset_4_two_words:
	mov	d1,(a0+)
memset_4_one_word:
	mov	d1,(a0+)

memset_4_no_remainder:
	# check we set the correct amount
	# TODO: REMOVE CHECK
	sub	e3,a0,d2
	mov	(20,sp),d1
	cmp	d2,d1
	beq	memset_done
	break
	break
	break

memset_done:
	mov	e3,a0
	ret	[d2,d3],8

	# handle misaligned copying
memset_1:
	add	-1,d2
	mov	+1,d3
	setlb					# setlb requires the next insns
						# to occupy exactly 4 bytes

	sub	d3,d2
	movbu	d1,(a0)
	inc	a0
	lcc

	mov	e3,a0
	ret	[d2,d3],8

memset_end:
	.size	memset, memset_end-memset
