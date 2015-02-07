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
 * linux/arch/m32r/lib/ashxdi3.S
 *
 * Copyright (C) 2001,2002  Hiroyuki Kondo, and Hirokazu Takata
 *
 */

;
;      input   (r0,r1)  src
;      input    r2      shift val
;               r3      scratch
;      output  (r0,r1)
;

#ifdef CONFIG_ISA_DUAL_ISSUE

#ifndef __LITTLE_ENDIAN__

	.text
	.align	4
	.globl __ashrdi3
__ashrdi3:
	cmpz	r2		    ||	ldi	r3, #32
	jc	r14		    ||	cmpu	r2, r3
	bc      1f
    ;   case 32 =< shift
	mv      r1, r0		    ||	srai    r0, #31
	addi    r2, #-32
	sra     r1, r2
	jmp     r14
	.fillinsn
1:  ;   case shift <32
	mv      r3, r0		    ||	srl     r1, r2
	sra     r0, r2		    ||	neg     r2, r2
	sll     r3, r2
	or      r1, r3		    ||	jmp	r14

        .align  4
        .globl __ashldi3
        .globl __lshldi3
__ashldi3:
__lshldi3:
	cmpz	r2		    ||	ldi	r3, #32
	jc	r14		    ||	cmpu	r2, r3
	bc      1f
    ;   case 32 =< shift
	mv      r0, r1		    ||	addi    r2, #-32
	sll     r0, r2		    ||	ldi     r1, #0
	jmp     r14
	.fillinsn
1:  ;   case shift <32
	mv      r3, r1		    ||	sll     r0, r2
	sll     r1, r2		    ||	neg     r2, r2
	srl     r3, r2
	or      r0, r3		    ||	jmp	r14

	.align	4
	.globl __lshrdi3
__lshrdi3:
	cmpz	r2		    ||	ldi	r3, #32
	jc	r14		    ||	cmpu	r2, r3
	bc      1f
    ;   case 32 =< shift
	mv      r1, r0		    ||	addi    r2, #-32
	ldi	r0, #0		    ||	srl     r1, r2
	jmp     r14
	.fillinsn
1:  ;   case shift <32
	mv      r3, r0		    ||	srl     r1, r2
	srl     r0, r2		    ||	neg     r2, r2
	sll     r3, r2
	or      r1, r3		    ||	jmp	r14

#else /* LITTLE_ENDIAN */

	.text
	.align	4
	.globl __ashrdi3
__ashrdi3:
	cmpz	r2		    ||	ldi	r3, #32
	jc	r14		    ||	cmpu	r2, r3
	bc      1f
    ;   case 32 =< shift
	mv      r0, r1		    ||	srai    r1, #31
	addi    r2, #-32
	sra     r0, r2
	jmp     r14
	.fillinsn
1:  ;   case shift <32
	mv      r3, r1		    ||	srl     r0, r2
	sra     r1, r2		    ||	neg     r2, r2
	sll     r3, r2
	or      r0, r3		    ||	jmp	r14

        .align  4
        .globl __ashldi3
        .globl __lshldi3
__ashldi3:
__lshldi3:
	cmpz	r2		    ||	ldi	r3, #32
	jc	r14		    ||	cmpu	r2, r3
	bc      1f
    ;   case 32 =< shift
	mv      r1, r0		    ||	addi    r2, #-32
	sll     r1, r2		    ||	ldi     r0, #0
	jmp     r14
	.fillinsn
1:  ;   case shift <32
	mv      r3, r0		    ||	sll     r1, r2
	sll     r0, r2		    ||	neg     r2, r2
	srl     r3, r2
	or      r1, r3		    ||	jmp	r14

	.align	4
	.globl __lshrdi3
__lshrdi3:
	cmpz	r2		    ||	ldi	r3, #32
	jc	r14		    ||	cmpu	r2, r3
	bc      1f
    ;   case 32 =< shift
	mv      r0, r1		    ||	addi    r2, #-32
	ldi	r1, #0		    ||	srl     r0, r2
	jmp     r14
	.fillinsn
1:  ;   case shift <32
	mv      r3, r1		    ||	srl     r0, r2
	srl     r1, r2		    ||	neg     r2, r2
	sll     r3, r2
	or      r0, r3		    ||	jmp	r14

#endif

#else /* not CONFIG_ISA_DUAL_ISSUE */

#ifndef __LITTLE_ENDIAN__

	.text
	.align	4
	.globl __ashrdi3
__ashrdi3:
	beqz	r2, 2f
	cmpui   r2, #32
	bc      1f
    ;   case 32 =< shift
	mv      r1, r0
	srai    r0, #31
	addi    r2, #-32
	sra     r1, r2
	jmp     r14
	.fillinsn
1:  ;   case shift <32
	mv      r3, r0
	srl     r1, r2
	sra     r0, r2
	neg     r2, r2
	sll     r3, r2
	or      r1, r3
	.fillinsn
2:
	jmp	r14

        .align  4
        .globl __ashldi3
        .globl __lshldi3
__ashldi3:
__lshldi3:
	beqz	r2, 2f
	cmpui   r2, #32
	bc      1f
    ;   case 32 =< shift
	mv      r0, r1
	addi    r2, #-32
	sll     r0, r2
	ldi     r1, #0
	jmp     r14
	.fillinsn
1:  ;   case shift <32
	mv      r3, r1
	sll     r0, r2
	sll     r1, r2
	neg     r2, r2
	srl     r3, r2
	or      r0, r3
	.fillinsn
2:
	jmp	r14

	.align	4
	.globl __lshrdi3
__lshrdi3:
	beqz	r2, 2f
	cmpui   r2, #32
	bc      1f
    ;   case 32 =< shift
	mv      r1, r0
	ldi	r0, #0
	addi    r2, #-32
	srl     r1, r2
	jmp     r14
	.fillinsn
1:  ;   case shift <32
	mv      r3, r0
	srl     r1, r2
	srl     r0, r2
	neg     r2, r2
	sll     r3, r2
	or      r1, r3
	.fillinsn
2:
	jmp	r14

#else

	.text
	.align	4
	.globl __ashrdi3
__ashrdi3:
	beqz	r2, 2f
	cmpui   r2, #32
	bc      1f
    ;   case 32 =< shift
	mv      r0, r1
	srai    r1, #31
	addi    r2, #-32
	sra     r0, r2
	jmp     r14
	.fillinsn
1:  ;   case shift <32
	mv      r3, r1
	srl     r0, r2
	sra     r1, r2
	neg     r2, r2
	sll     r3, r2
	or      r0, r3
	.fillinsn
2:
	jmp	r14

        .align  4
        .globl __ashldi3
        .globl __lshldi3
__ashldi3:
__lshldi3:
	beqz	r2, 2f
	cmpui   r2, #32
	bc      1f
    ;   case 32 =< shift
	mv      r1, r0
	addi    r2, #-32
	sll     r1, r2
	ldi     r0, #0
	jmp     r14
	.fillinsn
1:  ;   case shift <32
	mv      r3, r0
	sll     r1, r2
	sll     r0, r2
	neg     r2, r2
	srl     r3, r2
	or      r1, r3
	.fillinsn
2:
	jmp	r14

	.align	4
	.globl __lshrdi3
__lshrdi3:
	beqz	r2, 2f
	cmpui   r2, #32
	bc      1f
    ;   case 32 =< shift
	mv      r0, r1
	ldi	r1, #0
	addi    r2, #-32
	srl     r0, r2
	jmp     r14
	.fillinsn
1:  ;   case shift <32
	mv      r3, r1
	srl     r0, r2
	srl     r1, r2
	neg     r2, r2
	sll     r3, r2
	or      r0, r3
	.fillinsn
2:
	jmp	r14

#endif

#endif /* not CONFIG_ISA_DUAL_ISSUE */

	.end
