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
 * arch/alpha/lib/strchr.S
 * Contributed by Richard Henderson (rth@tamu.edu)
 *
 * Return the address of a given character within a null-terminated
 * string, or null if it is not found.
 */

#include <asm/regdef.h>

	.set noreorder
	.set noat

	.align 3
	.globl strchr
	.ent strchr
strchr:
	.frame sp, 0, ra
	.prologue 0

	zapnot	a1, 1, a1	# e0    : zero extend the search character
	ldq_u   t0, 0(a0)	# .. e1 : load first quadword
	sll	a1, 8, t5	# e0    : replicate the search character
	andnot  a0, 7, v0	# .. e1 : align our loop pointer
	or	t5, a1, a1	# e0    :
	lda	t4, -1		# .. e1 : build garbage mask
	sll	a1, 16, t5	# e0    :
	cmpbge  zero, t0, t2	# .. e1 : bits set iff byte == zero
	mskqh	t4, a0, t4	# e0    :
	or	t5, a1, a1	# .. e1 :
	sll	a1, 32, t5	# e0    :
	cmpbge	zero, t4, t4	# .. e1 : bits set iff byte is garbage
	or	t5, a1, a1	# e0    :
	xor	t0, a1, t1	# .. e1 : make bytes == c zero
	cmpbge  zero, t1, t3	# e0    : bits set iff byte == c
	or	t2, t3, t0	# e1    : bits set iff char match or zero match
	andnot	t0, t4, t0	# e0    : clear garbage bits
	bne	t0, $found	# .. e1 (zdb)

$loop:	ldq	t0, 8(v0)	# e0    :
	addq	v0, 8, v0	# .. e1 :
	nop			# e0    :
	xor	t0, a1, t1	# .. e1 (ev5 data stall)
	cmpbge	zero, t0, t2	# e0    : bits set iff byte == 0
	cmpbge	zero, t1, t3	# .. e1 : bits set iff byte == c
	or	t2, t3, t0	# e0    :
	beq	t0, $loop	# .. e1 (zdb)

$found:	negq    t0, t1		# e0    : clear all but least set bit
	and     t0, t1, t0	# e1 (stall)

	and	t0, t3, t1	# e0    : bit set iff byte was the char
	beq	t1, $retnull	# .. e1 (zdb)

	and     t0, 0xf0, t2	# e0    : binary search for that set bit
	and	t0, 0xcc, t3	# .. e1 :
	and	t0, 0xaa, t4	# e0    :
	cmovne	t2, 4, t2	# .. e1 :
	cmovne	t3, 2, t3	# e0    :
	cmovne	t4, 1, t4	# .. e1 :
	addq	t2, t3, t2	# e0    :
	addq	v0, t4, v0	# .. e1 :
	addq	v0, t2, v0	# e0    :
	ret			# .. e1 :

$retnull:
	mov	zero, v0	# e0    :
	ret			# .. e1 :

	.end strchr
