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
 * arch/alpha/lib/strncpy.S
 * Contributed by Richard Henderson (rth@tamu.edu)
 *
 * Copy no more than COUNT bytes of the null-terminated string from
 * SRC to DST.  If SRC does not cover all of COUNT, the balance is
 * zeroed.
 *
 * Or, rather, if the kernel cared about that weird ANSI quirk.  This
 * version has cropped that bit o' nastiness as well as assuming that
 * __stxncpy is in range of a branch.
 */

	.set noat
	.set noreorder

	.text

	.align 4
	.globl strncpy
	.ent strncpy
strncpy:
	.frame $30, 0, $26
	.prologue 0

	mov	$16, $0		# set return value now
	beq	$18, $zerolen
	unop
	bsr	$23, __stxncpy	# do the work of the copy

	unop
	bne	$18, $multiword	# do we have full words left?
	subq	$24, 1, $3	# nope
	subq	$27, 1, $4

	or	$3, $24, $3	# clear the bits between the last
	or	$4, $27, $4	# written byte and the last byte in COUNT
	andnot	$3, $4, $4
	zap	$1, $4, $1

	stq_u	$1, 0($16)
	ret

	.align	4
$multiword:
	subq	$27, 1, $2	# clear the final bits in the prev word
	or	$2, $27, $2
	zapnot	$1, $2, $1
	subq	$18, 1, $18

	stq_u	$1, 0($16)
	addq	$16, 8, $16
	unop
	beq	$18, 1f

	nop
	unop
	nop
	blbc	$18, 0f

	stq_u	$31, 0($16)	# zero one word
	subq	$18, 1, $18
	addq	$16, 8, $16
	beq	$18, 1f

0:	stq_u	$31, 0($16)	# zero two words
	subq	$18, 2, $18
	stq_u	$31, 8($16)
	addq	$16, 16, $16
	bne	$18, 0b

1:	ldq_u	$1, 0($16)	# clear the leading bits in the final word
	subq	$24, 1, $2
	or	$2, $24, $2

	zap	$1, $2, $1
	stq_u	$1, 0($16)
$zerolen:
	ret

	.end	strncpy
