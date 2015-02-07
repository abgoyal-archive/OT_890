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
 * strlen.S (c) 1995 David Mosberger (davidm@cs.arizona.edu)
 *
 * Finds length of a 0-terminated string.  Optimized for the
 * Alpha architecture:
 *
 *	- memory accessed as aligned quadwords only
 *	- uses bcmpge to compare 8 bytes in parallel
 *	- does binary search to find 0 byte in last
 *	  quadword (HAKMEM needed 12 instructions to
 *	  do this instead of the 9 instructions that
 *	  binary search needs).
 */

	.set noreorder
	.set noat

	.align 3

	.globl	strlen
	.ent	strlen

strlen:
	ldq_u	$1, 0($16)	# load first quadword ($16  may be misaligned)
	lda	$2, -1($31)
	insqh	$2, $16, $2
	andnot	$16, 7, $0
	or	$2, $1, $1
	cmpbge	$31, $1, $2	# $2  <- bitmask: bit i == 1 <==> i-th byte == 0
	bne	$2, found

loop:	ldq	$1, 8($0)
	addq	$0, 8, $0	# addr += 8
	nop			# helps dual issue last two insns
	cmpbge	$31, $1, $2
	beq	$2, loop

found:	blbs	$2, done	# make aligned case fast
	negq	$2, $3
	and	$2, $3, $2

	and	$2, 0x0f, $1
	addq	$0, 4, $3
	cmoveq	$1, $3, $0

	and	$2, 0x33, $1
	addq	$0, 2, $3
	cmoveq	$1, $3, $0

	and	$2, 0x55, $1
	addq	$0, 1, $3
	cmoveq	$1, $3, $0

done:	subq	$0, $16, $0
	ret	$31, ($26)

	.end	strlen
