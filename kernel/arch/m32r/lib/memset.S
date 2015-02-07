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
 *  linux/arch/m32r/lib/memset.S
 *
 *  Copyright (C) 2001,2002  Hiroyuki Kondo, and Hirokazu Takata
 *  Copyright (C) 2004  Hirokazu Takata
 *
 *  void *memset(void *dst, int val, int len);
 *
 *        dst: r0
 *        val: r1
 *        len: r2
 *        ret: r0
 *
 */

	.text
	.global	memset

#ifdef CONFIG_ISA_DUAL_ISSUE

	.align 4
memset:
	mv      r4, r0		    ||	cmpz	r2
	jc	r14
	cmpui	r2, #16
	bnc	qword_align_check
	cmpui	r2, #4
	bc	byte_set
word_align_check:			/* len >= 4 */
	and3	r3, r4, #3
	beqz	r3, word_set
	addi	r3, #-4
	neg	r3, r3			/* r3 = -(r3 - 4) */
align_word:
	stb	r1, @r4		    ||	addi	r4, #1
	addi	r2, #-1		    ||	addi	r3, #-1
	bnez	r3, align_word
	cmpui	r2, #4
	bc	byte_set
word_set:
	and3	r1, r1, #0x00ff		/* r1: abababab <-- ??????ab */
	sll3	r3, r1, #8
	or	r1, r3		    ||	addi	r4, #-4
	sll3	r3, r1, #16
	or	r1, r3		    ||	addi	r2, #-4
word_set_loop:
	st	r1, @+r4	    ||	addi	r2, #-4
	bgtz	r2, word_set_loop
	bnez	r2, byte_set_wrap
	st	r1, @+r4
	jmp	r14

qword_align_check:			/* len >= 16 */
	and3	r3, r4, #15
	bnez	r3, word_align_check
qword_set:
	and3	r1, r1, #0x00ff		/* r1: abababab <-- ??????ab */
	sll3	r3, r1, #8
	or	r1, r3		    ||	addi	r4, #-4
	sll3	r3, r1, #16
	or	r1, r3		    ||	ldi	r5, #16
qword_set_loop:
	ld	r3, @(4,r4)		/* cache line allocate */
	st	r1, @+r4	    ||	addi	r2, #-16
	st	r1, @+r4	    ||	cmpu	r2, r5
	st	r1, @+r4
	st	r1, @+r4
	bnc	qword_set_loop	    ||  cmpz	r2
	jc	r14
set_remainder:
	cmpui	r2, #4
	bc	byte_set_wrap1
	addi	r2, #-4
	bra	word_set_loop

byte_set_wrap:
	addi	r2, #4
	cmpz	r2
	jc	r14
byte_set_wrap1:
	addi	r4, #4
#if defined(CONFIG_ISA_M32R2)
byte_set:
	addi	r2, #-1		    ||	stb	r1, @r4+
	bnez	r2, byte_set
#elif defined(CONFIG_ISA_M32R)
byte_set:
	addi	r2, #-1		    ||	stb	r1, @r4
	addi	r4, #1
	bnez	r2, byte_set
#else
#error unknown isa configuration
#endif
end_memset:
	jmp	r14

#else /* not CONFIG_ISA_DUAL_ISSUE */

	.align 4
memset:
	mv      r4, r0
	beqz	r2, end_memset
	cmpui	r2, #16
	bnc	qword_align_check
	cmpui	r2, #4
	bc	byte_set
word_align_check:			/* len >= 4 */
	and3	r3, r4, #3
	beqz	r3, word_set
	addi	r3, #-4
	neg	r3, r3			/* r3 = -(r3 - 4) */
align_word:
	stb	r1, @r4
	addi	r4, #1
	addi	r2, #-1
	addi	r3, #-1
	bnez	r3, align_word
	cmpui	r2, #4
	bc	byte_set
word_set:
	and3	r1, r1, #0x00ff		/* r1: abababab <-- ??????ab */
	sll3	r3, r1, #8
	or	r1, r3
	sll3	r3, r1, #16
	or	r1, r3
	addi	r2, #-4
	addi	r4, #-4
word_set_loop:
	st	r1, @+r4
	addi	r2, #-4
	bgtz    r2, word_set_loop
	bnez	r2, byte_set_wrap
	st	r1, @+r4
	jmp	r14

qword_align_check:			/* len >= 16 */
	and3	r3, r4, #15
	bnez	r3, word_align_check
qword_set:
	and3	r1, r1, #0x00ff		/* r1: abababab <-- ??????ab */
	sll3	r3, r1, #8
	or	r1, r3
	sll3	r3, r1, #16
	or	r1, r3
	addi	r4, #-4
qword_set_loop:
	ld	r3, @(4,r4)		/* cache line allocate */
	addi	r2, #-16
	st	r1, @+r4
	st	r1, @+r4
	cmpui	r2, #16
	st	r1, @+r4
	st	r1, @+r4
	bnc	qword_set_loop
	bnez	r2, set_remainder
	jmp	r14
set_remainder:
	cmpui	r2, #4
	bc	byte_set_wrap1
	addi	r2, #-4
	bra	word_set_loop

byte_set_wrap:
	addi	r2, #4
	beqz	r2, end_memset
byte_set_wrap1:
	addi	r4, #4
byte_set:
	addi	r2, #-1
	stb	r1, @r4
	addi	r4, #1
	bnez	r2, byte_set
end_memset:
	jmp	r14

#endif /* not CONFIG_ISA_DUAL_ISSUE */

	.end
