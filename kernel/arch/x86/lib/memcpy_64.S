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

/* Copyright 2002 Andi Kleen */

#include <linux/linkage.h>
#include <asm/dwarf2.h>
#include <asm/cpufeature.h>

/*
 * memcpy - Copy a memory block.
 *
 * Input:	
 * rdi destination
 * rsi source
 * rdx count
 * 
 * Output:
 * rax original destination
 */	

	ALIGN
memcpy_c:
	CFI_STARTPROC
	movq %rdi,%rax
	movl %edx,%ecx
	shrl $3,%ecx
	andl $7,%edx
	rep movsq
	movl %edx,%ecx
	rep movsb
	ret
	CFI_ENDPROC
ENDPROC(memcpy_c)

ENTRY(__memcpy)
ENTRY(memcpy)
	CFI_STARTPROC
	pushq %rbx
	CFI_ADJUST_CFA_OFFSET 8
	CFI_REL_OFFSET rbx, 0
	movq %rdi,%rax

	movl %edx,%ecx
	shrl $6,%ecx
	jz .Lhandle_tail

	.p2align 4
.Lloop_64:
	decl %ecx

	movq (%rsi),%r11
	movq 8(%rsi),%r8

	movq %r11,(%rdi)
	movq %r8,1*8(%rdi)

	movq 2*8(%rsi),%r9
	movq 3*8(%rsi),%r10

	movq %r9,2*8(%rdi)
	movq %r10,3*8(%rdi)

	movq 4*8(%rsi),%r11
	movq 5*8(%rsi),%r8

	movq %r11,4*8(%rdi)
	movq %r8,5*8(%rdi)

	movq 6*8(%rsi),%r9
	movq 7*8(%rsi),%r10

	movq %r9,6*8(%rdi)
	movq %r10,7*8(%rdi)

	leaq 64(%rsi),%rsi
	leaq 64(%rdi),%rdi
	jnz  .Lloop_64

.Lhandle_tail:
	movl %edx,%ecx
	andl $63,%ecx
	shrl $3,%ecx
	jz   .Lhandle_7
	.p2align 4
.Lloop_8:
	decl %ecx
	movq (%rsi),%r8
	movq %r8,(%rdi)
	leaq 8(%rdi),%rdi
	leaq 8(%rsi),%rsi
	jnz  .Lloop_8

.Lhandle_7:
	movl %edx,%ecx
	andl $7,%ecx
	jz .Lende
	.p2align 4
.Lloop_1:
	movb (%rsi),%r8b
	movb %r8b,(%rdi)
	incq %rdi
	incq %rsi
	decl %ecx
	jnz .Lloop_1

.Lende:
	popq %rbx
	CFI_ADJUST_CFA_OFFSET -8
	CFI_RESTORE rbx
	ret
.Lfinal:
	CFI_ENDPROC
ENDPROC(memcpy)
ENDPROC(__memcpy)

	/* Some CPUs run faster using the string copy instructions.
	   It is also a lot simpler. Use this when possible */

	.section .altinstr_replacement,"ax"
1:	.byte 0xeb				/* jmp <disp8> */
	.byte (memcpy_c - memcpy) - (2f - 1b)	/* offset */
2:
	.previous
	.section .altinstructions,"a"
	.align 8
	.quad memcpy
	.quad 1b
	.byte X86_FEATURE_REP_GOOD
	/* Replace only beginning, memcpy is used to apply alternatives, so it
	 * is silly to overwrite itself with nops - reboot is only outcome... */
	.byte 2b - 1b
	.byte 2b - 1b
	.previous
