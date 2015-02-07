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

/* Copyright 2002 Andi Kleen, SuSE Labs */

#include <linux/linkage.h>
#include <asm/dwarf2.h>

/*
 * ISO C memset - set a memory block to a byte value.
 *	
 * rdi   destination
 * rsi   value (char) 
 * rdx   count (bytes) 
 * 
 * rax   original destination
 */	
	ALIGN
memset_c:
	CFI_STARTPROC
	movq %rdi,%r9
	movl %edx,%r8d
	andl $7,%r8d
	movl %edx,%ecx
	shrl $3,%ecx
	/* expand byte value  */
	movzbl %sil,%esi
	movabs $0x0101010101010101,%rax
	mulq %rsi		/* with rax, clobbers rdx */
	rep stosq
	movl %r8d,%ecx
	rep stosb
	movq %r9,%rax
	ret
	CFI_ENDPROC
ENDPROC(memset_c)

ENTRY(memset)
ENTRY(__memset)
	CFI_STARTPROC
	movq %rdi,%r10
	movq %rdx,%r11

	/* expand byte value  */
	movzbl %sil,%ecx
	movabs $0x0101010101010101,%rax
	mul    %rcx		/* with rax, clobbers rdx */

	/* align dst */
	movl  %edi,%r9d
	andl  $7,%r9d
	jnz  .Lbad_alignment
	CFI_REMEMBER_STATE
.Lafter_bad_alignment:

	movl %r11d,%ecx
	shrl $6,%ecx
	jz	 .Lhandle_tail

	.p2align 4
.Lloop_64:
	decl   %ecx
	movq  %rax,(%rdi)
	movq  %rax,8(%rdi)
	movq  %rax,16(%rdi)
	movq  %rax,24(%rdi)
	movq  %rax,32(%rdi)
	movq  %rax,40(%rdi)
	movq  %rax,48(%rdi)
	movq  %rax,56(%rdi)
	leaq  64(%rdi),%rdi
	jnz    .Lloop_64

	/* Handle tail in loops. The loops should be faster than hard
	   to predict jump tables. */
	.p2align 4
.Lhandle_tail:
	movl	%r11d,%ecx
	andl    $63&(~7),%ecx
	jz 		.Lhandle_7
	shrl	$3,%ecx
	.p2align 4
.Lloop_8:
	decl   %ecx
	movq  %rax,(%rdi)
	leaq  8(%rdi),%rdi
	jnz    .Lloop_8

.Lhandle_7:
	movl	%r11d,%ecx
	andl	$7,%ecx
	jz      .Lende
	.p2align 4
.Lloop_1:
	decl    %ecx
	movb 	%al,(%rdi)
	leaq	1(%rdi),%rdi
	jnz     .Lloop_1

.Lende:
	movq	%r10,%rax
	ret

	CFI_RESTORE_STATE
.Lbad_alignment:
	cmpq $7,%r11
	jbe	.Lhandle_7
	movq %rax,(%rdi)	/* unaligned store */
	movq $8,%r8
	subq %r9,%r8
	addq %r8,%rdi
	subq %r8,%r11
	jmp .Lafter_bad_alignment
.Lfinal:
	CFI_ENDPROC
ENDPROC(memset)
ENDPROC(__memset)

	/* Some CPUs run faster using the string instructions.
	   It is also a lot simpler. Use this when possible */

#include <asm/cpufeature.h>

	.section .altinstr_replacement,"ax"
1:	.byte 0xeb				/* jmp <disp8> */
	.byte (memset_c - memset) - (2f - 1b)	/* offset */
2:
	.previous
	.section .altinstructions,"a"
	.align 8
	.quad memset
	.quad 1b
	.byte X86_FEATURE_REP_GOOD
	.byte .Lfinal - memset
	.byte 2b - 1b
	.previous
