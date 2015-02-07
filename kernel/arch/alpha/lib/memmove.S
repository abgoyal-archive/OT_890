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
 * arch/alpha/lib/memmove.S
 *
 * Barely optimized memmove routine for Alpha EV5.
 *
 * This is hand-massaged output from the original memcpy.c.  We defer to
 * memcpy whenever possible; the backwards copy loops are not unrolled.
 */
        
	.set noat
	.set noreorder
	.text

	.align 4
	.globl memmove
	.ent memmove
memmove:
	ldgp $29, 0($27)
	unop
	nop
	.prologue 1

	addq $16,$18,$4
	addq $17,$18,$5
	cmpule $4,$17,$1		/*  dest + n <= src  */
	cmpule $5,$16,$2		/*  dest >= src + n  */

	bis $1,$2,$1
	mov $16,$0
	xor $16,$17,$2
	bne $1,memcpy			!samegp

	and $2,7,$2			/* Test for src/dest co-alignment.  */
	and $16,7,$1
	cmpule $16,$17,$3
	bne $3,$memmove_up		/* dest < src */

	and $4,7,$1
	bne $2,$misaligned_dn
	unop
	beq $1,$skip_aligned_byte_loop_head_dn

$aligned_byte_loop_head_dn:
	lda $4,-1($4)
	lda $5,-1($5)
	unop
	ble $18,$egress

	ldq_u $3,0($5)
	ldq_u $2,0($4)
	lda $18,-1($18)
	extbl $3,$5,$1

	insbl $1,$4,$1
	mskbl $2,$4,$2
	bis $1,$2,$1
	and $4,7,$6

	stq_u $1,0($4)
	bne $6,$aligned_byte_loop_head_dn

$skip_aligned_byte_loop_head_dn:
	lda $18,-8($18)
	blt $18,$skip_aligned_word_loop_dn

$aligned_word_loop_dn:
	ldq $1,-8($5)
	nop
	lda $5,-8($5)
	lda $18,-8($18)

	stq $1,-8($4)
	nop
	lda $4,-8($4)
	bge $18,$aligned_word_loop_dn

$skip_aligned_word_loop_dn:
	lda $18,8($18)
	bgt $18,$byte_loop_tail_dn
	unop
	ret $31,($26),1

	.align 4
$misaligned_dn:
	nop
	fnop
	unop
	beq $18,$egress

$byte_loop_tail_dn:
	ldq_u $3,-1($5)
	ldq_u $2,-1($4)
	lda $5,-1($5)
	lda $4,-1($4)

	lda $18,-1($18)
	extbl $3,$5,$1
	insbl $1,$4,$1
	mskbl $2,$4,$2

	bis $1,$2,$1
	stq_u $1,0($4)
	bgt $18,$byte_loop_tail_dn
	br $egress

$memmove_up:
	mov $16,$4
	mov $17,$5
	bne $2,$misaligned_up
	beq $1,$skip_aligned_byte_loop_head_up

$aligned_byte_loop_head_up:
	unop
	ble $18,$egress
	ldq_u $3,0($5)
	ldq_u $2,0($4)

	lda $18,-1($18)
	extbl $3,$5,$1
	insbl $1,$4,$1
	mskbl $2,$4,$2

	bis $1,$2,$1
	lda $5,1($5)
	stq_u $1,0($4)
	lda $4,1($4)

	and $4,7,$6
	bne $6,$aligned_byte_loop_head_up

$skip_aligned_byte_loop_head_up:
	lda $18,-8($18)
	blt $18,$skip_aligned_word_loop_up

$aligned_word_loop_up:
	ldq $1,0($5)
	nop
	lda $5,8($5)
	lda $18,-8($18)

	stq $1,0($4)
	nop
	lda $4,8($4)
	bge $18,$aligned_word_loop_up

$skip_aligned_word_loop_up:
	lda $18,8($18)
	bgt $18,$byte_loop_tail_up
	unop
	ret $31,($26),1

	.align 4
$misaligned_up:
	nop
	fnop
	unop
	beq $18,$egress

$byte_loop_tail_up:
	ldq_u $3,0($5)
	ldq_u $2,0($4)
	lda $18,-1($18)
	extbl $3,$5,$1

	insbl $1,$4,$1
	mskbl $2,$4,$2
	bis $1,$2,$1
	stq_u $1,0($4)

	lda $5,1($5)
	lda $4,1($4)
	nop
	bgt $18,$byte_loop_tail_up

$egress:
	ret $31,($26),1
	nop
	nop
	nop

	.end memmove
