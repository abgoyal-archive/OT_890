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
 * A fast checksum routine using movem
 * Copyright (c) 1998-2007 Axis Communications AB
 *
 * csum_partial(const unsigned char * buff, int len, unsigned int sum)
 */

	.globl	csum_partial
csum_partial:

	;; r10 - src
	;; r11 - length
	;; r12 - checksum

	;; Optimized for large packets
	subq	10*4, $r11
	blt	_word_loop
	move.d	$r11, $acr

	subq	9*4,$sp
	clearf	c
	movem	$r8,[$sp]

	;; do a movem checksum

_mloop:	movem	[$r10+],$r9	; read 10 longwords
	;; Loop count without touching the c flag.
	addoq	-10*4, $acr, $acr
	;; perform dword checksumming on the 10 longwords

	addc	$r0,$r12
	addc	$r1,$r12
	addc	$r2,$r12
	addc	$r3,$r12
	addc	$r4,$r12
	addc	$r5,$r12
	addc	$r6,$r12
	addc	$r7,$r12
	addc	$r8,$r12
	addc	$r9,$r12

	;; test $acr without trashing carry.
	move.d	$acr, $acr
	bpl	_mloop
	;; r11 <= acr  is not really needed in the mloop, just using the dslot
	;; to prepare for what is needed after mloop.
	move.d	$acr, $r11

	;; fold the last carry into r13
	addc	0, $r12
	movem	[$sp+],$r8	; restore regs

_word_loop:
	addq	10*4,$r11	; compensate for last loop underflowing length

	moveq	-1,$r9		; put 0xffff in r9, faster than move.d 0xffff,r9
	lsrq	16,$r9

	move.d	$r12,$r13
	lsrq	16,$r13		; r13 = checksum >> 16
	and.d	$r9,$r12	; checksum = checksum & 0xffff

_no_fold:
	subq	2,$r11
	blt	_no_words
	add.d	$r13,$r12	; checksum += r13

	;; checksum the rest of the words
_wloop:	subq	2,$r11
	bge	_wloop
	addu.w	[$r10+],$r12

_no_words:
	addq	2,$r11
	;; see if we have one odd byte more
	bne	_do_byte
	nop
	ret
	move.d	$r12,$r10

_do_byte:
	;; copy and checksum the last byte
	addu.b	[$r10],$r12
	ret
	move.d	$r12,$r10
