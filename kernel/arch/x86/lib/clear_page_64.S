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

#include <linux/linkage.h>
#include <asm/dwarf2.h>

/*
 * Zero a page. 	
 * rdi	page
 */			
	ALIGN
clear_page_c:
	CFI_STARTPROC
	movl $4096/8,%ecx
	xorl %eax,%eax
	rep stosq
	ret
	CFI_ENDPROC
ENDPROC(clear_page)

ENTRY(clear_page)
	CFI_STARTPROC
	xorl   %eax,%eax
	movl   $4096/64,%ecx
	.p2align 4
.Lloop:
	decl	%ecx
#define PUT(x) movq %rax,x*8(%rdi)
	movq %rax,(%rdi)
	PUT(1)
	PUT(2)
	PUT(3)
	PUT(4)
	PUT(5)
	PUT(6)
	PUT(7)
	leaq	64(%rdi),%rdi
	jnz	.Lloop
	nop
	ret
	CFI_ENDPROC
.Lclear_page_end:
ENDPROC(clear_page)

	/* Some CPUs run faster using the string instructions.
	   It is also a lot simpler. Use this when possible */

#include <asm/cpufeature.h>

	.section .altinstr_replacement,"ax"
1:	.byte 0xeb					/* jmp <disp8> */
	.byte (clear_page_c - clear_page) - (2f - 1b)	/* offset */
2:
	.previous
	.section .altinstructions,"a"
	.align 8
	.quad clear_page
	.quad 1b
	.byte X86_FEATURE_REP_GOOD
	.byte .Lclear_page_end - clear_page
	.byte 2b - 1b
	.previous
