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
 *  linux/arch/m32r/lib/memcpy.S
 *
 *  Copyright (C) 2001  Hiroyuki Kondo, and Hirokazu Takata
 *  Copyright (C) 2004  Hirokazu Takata
 *
 *  void *memcopy(void *dst, const void *src, int n);
 *
 *        dst: r0
 *        src: r1
 *        n  : r2
 */

	.text
#include <linux/linkage.h>
#include <asm/assembler.h>

#ifdef CONFIG_ISA_DUAL_ISSUE

	.text
ENTRY(memcpy)
memcopy:
	mv	r4, r0		    ||	mv	r7, r0
	or	r7, r1		    ||	cmpz	r2
	jc	r14		    ||	cmpeq	r0, r1	; return if r2=0
	jc	r14					; return if r0=r1

	and3	r7, r7, #3
	bnez	r7, byte_copy
	srl3	r3, r2, #2
	and3	r2, r2, #3
	beqz	r3, byte_copy
	addi	r4, #-4
word_copy:
	ld	r7, @r1+	    ||	addi	r3, #-1
	st	r7, @+r4	    ||	cmpz	r2
	bnez	r3, word_copy
	addi	r4, #4		    ||	jc	r14	; return if r2=0
#if defined(CONFIG_ISA_M32R2)
byte_copy:
	ldb	r7, @r1		    ||	addi	r1, #1
	addi	r2, #-1		    ||	stb	r7, @r4+
	bnez	r2, byte_copy
#elif defined(CONFIG_ISA_M32R)
byte_copy:
	ldb	r7, @r1		    ||	addi	r1, #1
	addi	r2, #-1		    ||	stb	r7, @r4
	addi	r4, #1
	bnez	r2, byte_copy
#else
#error unknown isa configuration
#endif
end_memcopy:
	jmp	r14

#else /* not CONFIG_ISA_DUAL_ISSUE */

	.text
ENTRY(memcpy)
memcopy:
	mv	r4, r0
	mv	r7, r0
	or	r7, r1
	beq	r0, r1, end_memcopy
	beqz	r2, end_memcopy

	and3	r7, r7, #3
	bnez	r7, byte_copy
	srl3	r3, r2, #2
	and3	r2, r2, #3
	beqz	r3, byte_copy
	addi	r4, #-4
word_copy:
	ld	r7, @r1+
	addi	r3, #-1
	st	r7, @+r4
	bnez	r3, word_copy
	beqz	r2, end_memcopy
	addi	r4, #4
byte_copy:
	ldb	r7, @r1
	addi	r1, #1
	addi	r2, #-1
	stb	r7, @r4
	addi	r4, #1
	bnez	r2, byte_copy
end_memcopy:
	jmp	r14

#endif /* not CONFIG_ISA_DUAL_ISSUE */

	.end
