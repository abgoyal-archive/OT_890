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

#
# arch/i386/setjmp.S
#
# setjmp/longjmp for the i386 architecture
#

#
# The jmp_buf is assumed to contain the following, in order:
#	%ebx
#	%esp
#	%ebp
#	%esi
#	%edi
#	<return address>
#

	.text
	.align 4
	.globl setjmp
	.type setjmp, @function
setjmp:
#ifdef _REGPARM
	movl %eax,%edx
#else
	movl 4(%esp),%edx
#endif
	popl %ecx			# Return address, and adjust the stack
	xorl %eax,%eax			# Return value
	movl %ebx,(%edx)
	movl %esp,4(%edx)		# Post-return %esp!
	pushl %ecx			# Make the call/return stack happy
	movl %ebp,8(%edx)
	movl %esi,12(%edx)
	movl %edi,16(%edx)
	movl %ecx,20(%edx)		# Return address
	ret

	.size setjmp,.-setjmp

	.text
	.align 4
	.globl longjmp
	.type longjmp, @function
longjmp:
#ifdef _REGPARM
	xchgl %eax,%edx
#else
	movl 4(%esp),%edx		# jmp_ptr address
	movl 8(%esp),%eax		# Return value
#endif
	movl (%edx),%ebx
	movl 4(%edx),%esp
	movl 8(%edx),%ebp
	movl 12(%edx),%esi
	movl 16(%edx),%edi
	jmp *20(%edx)

	.size longjmp,.-longjmp
