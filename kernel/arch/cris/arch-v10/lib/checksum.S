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
 * Copyright (c) 1998-2001 Axis Communications AB
 *
 * csum_partial(const unsigned char * buff, int len, unsigned int sum)
 */

	.globl	csum_partial
csum_partial:
	
	;; r10 - src
	;; r11 - length
	;; r12 - checksum

	;; check for breakeven length between movem and normal word looping versions
	;; we also do _NOT_ want to compute a checksum over more than the 
	;; actual length when length < 40
	
	cmpu.w	80,$r11
	blo	_word_loop
	nop

	;; need to save the registers we use below in the movem loop
	;; this overhead is why we have a check above for breakeven length
	;; only r0 - r8 have to be saved, the other ones are clobber-able
	;; according to the ABI
	
	subq	9*4,$sp
	movem	$r8,[$sp]
	
	;; do a movem checksum

	subq	10*4,$r11	; update length for the first loop
	
_mloop:	movem	[$r10+],$r9	; read 10 longwords

	;; perform dword checksumming on the 10 longwords
	
	add.d	$r0,$r12
	ax
	add.d	$r1,$r12
	ax
	add.d	$r2,$r12
	ax
	add.d	$r3,$r12
	ax
	add.d	$r4,$r12
	ax
	add.d	$r5,$r12
	ax
	add.d	$r6,$r12
	ax
	add.d	$r7,$r12
	ax
	add.d	$r8,$r12
	ax
	add.d	$r9,$r12

	;; fold the carry into the checksum, to avoid having to loop the carry
	;; back into the top
	
	ax
	addq	0,$r12

	subq	10*4,$r11
	bge	_mloop
	nop

	addq	10*4,$r11	; compensate for last loop underflowing length

	movem	[$sp+],$r8	; restore regs

_word_loop:
	;; only fold if there is anything to fold.

	cmpq	0,$r12
	beq	_no_fold

	;; fold 32-bit checksum into a 16-bit checksum, to avoid carries below.
	;; r9 and r13 can be used as temporaries.
	
	moveq	-1,$r9		; put 0xffff in r9, faster than move.d 0xffff,r9
	lsrq	16,$r9
	
	move.d	$r12,$r13
	lsrq	16,$r13		; r13 = checksum >> 16
	and.d	$r9,$r12		; checksum = checksum & 0xffff
	add.d	$r13,$r12		; checksum += r13

_no_fold:
	cmpq	2,$r11
	blt	_no_words
	nop
	
	;; checksum the rest of the words
	
	subq	2,$r11
	
_wloop:	subq	2,$r11
	bge	_wloop
	addu.w	[$r10+],$r12
	
	addq	2,$r11
		
_no_words:
	;; see if we have one odd byte more
	cmpq	1,$r11
	beq	_do_byte
	nop
	ret
	move.d	$r12, $r10

_do_byte:	
	;; copy and checksum the last byte
	addu.b	[$r10],$r12
	ret
	move.d	$r12, $r10
		
