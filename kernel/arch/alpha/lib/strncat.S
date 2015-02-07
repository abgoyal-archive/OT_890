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
 * arch/alpha/lib/strncat.S
 * Contributed by Richard Henderson (rth@tamu.edu)
 *
 * Append no more than COUNT characters from the null-terminated string SRC
 * to the null-terminated string DST.  Always null-terminate the new DST.
 *
 * This differs slightly from the semantics in libc in that we never write
 * past count, whereas libc may write to count+1.  This follows the generic
 * implementation in lib/string.c and is, IMHO, more sensible.
 */

	.text

	.align 3
	.globl strncat
	.ent strncat
strncat:
	.frame $30, 0, $26
	.prologue 0

	mov	$16, $0		# set up return value
	beq	$18, $zerocount

	/* Find the end of the string.  */

	ldq_u   $1, 0($16)	# load first quadword ($16 may be misaligned)
	lda     $2, -1($31)
	insqh   $2, $16, $2
	andnot  $16, 7, $16
	or      $2, $1, $1
	cmpbge  $31, $1, $2	# bits set iff byte == 0
	bne     $2, $found

$loop:	ldq     $1, 8($16)
	addq    $16, 8, $16
	cmpbge  $31, $1, $2
	beq     $2, $loop

$found:	negq    $2, $3		# clear all but least set bit
	and     $2, $3, $2

	and     $2, 0xf0, $3	# binary search for that set bit
	and	$2, 0xcc, $4
	and	$2, 0xaa, $5
	cmovne	$3, 4, $3
	cmovne	$4, 2, $4
	cmovne	$5, 1, $5
	addq	$3, $4, $3
	addq	$16, $5, $16
	addq	$16, $3, $16

	/* Now do the append.  */

	bsr	$23, __stxncpy

	/* Worry about the null termination.  */

	zapnot	$1, $27, $2	# was last byte a null?
	bne	$2, 0f
	ret

0:	cmplt	$27, $24, $2	# did we fill the buffer completely?
	or	$2, $18, $2
	bne	$2, 2f

	and	$24, 0x80, $2	# no zero next byte
	bne	$2, 1f

	/* Here there are bytes left in the current word.  Clear one.  */
	addq	$24, $24, $24	# end-of-count bit <<= 1
2:	zap	$1, $24, $1
	stq_u	$1, 0($16)
	ret

1:	/* Here we must read the next DST word and clear the first byte.  */
	ldq_u	$1, 8($16)
	zap	$1, 1, $1
	stq_u	$1, 8($16)

$zerocount:
	ret

	.end strncat
