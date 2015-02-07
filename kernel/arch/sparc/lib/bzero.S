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

/* bzero.S: Simple prefetching memset, bzero, and clear_user
 *          implementations.
 *
 * Copyright (C) 2005 David S. Miller <davem@davemloft.net>
 */

	.text

	.globl	__memset
	.type	__memset, #function
__memset:		/* %o0=buf, %o1=pat, %o2=len */

	.globl	memset
	.type	memset, #function
memset:			/* %o0=buf, %o1=pat, %o2=len */
	and		%o1, 0xff, %o3
	mov		%o2, %o1
	sllx		%o3, 8, %g1
	or		%g1, %o3, %o2
	sllx		%o2, 16, %g1
	or		%g1, %o2, %o2
	sllx		%o2, 32, %g1
	ba,pt		%xcc, 1f
	 or		%g1, %o2, %o2

	.globl	__bzero
	.type	__bzero, #function
__bzero:		/* %o0=buf, %o1=len */
	clr		%o2
1:	mov		%o0, %o3
	brz,pn		%o1, __bzero_done
	 cmp		%o1, 16
	bl,pn		%icc, __bzero_tiny
	 prefetch	[%o0 + 0x000], #n_writes
	andcc		%o0, 0x3, %g0
	be,pt		%icc, 2f
1:	 stb		%o2, [%o0 + 0x00]
	add		%o0, 1, %o0
	andcc		%o0, 0x3, %g0
	bne,pn		%icc, 1b
	 sub		%o1, 1, %o1
2:	andcc		%o0, 0x7, %g0
	be,pt		%icc, 3f
	 stw		%o2, [%o0 + 0x00]
	sub		%o1, 4, %o1
	add		%o0, 4, %o0
3:	and		%o1, 0x38, %g1
	cmp		%o1, 0x40
	andn		%o1, 0x3f, %o4
	bl,pn		%icc, 5f
	 and		%o1, 0x7, %o1
	prefetch	[%o0 + 0x040], #n_writes
	prefetch	[%o0 + 0x080], #n_writes
	prefetch	[%o0 + 0x0c0], #n_writes
	prefetch	[%o0 + 0x100], #n_writes
	prefetch	[%o0 + 0x140], #n_writes
4:	prefetch	[%o0 + 0x180], #n_writes
	stx		%o2, [%o0 + 0x00]
	stx		%o2, [%o0 + 0x08]
	stx		%o2, [%o0 + 0x10]
	stx		%o2, [%o0 + 0x18]
	stx		%o2, [%o0 + 0x20]
	stx		%o2, [%o0 + 0x28]
	stx		%o2, [%o0 + 0x30]
	stx		%o2, [%o0 + 0x38]
	subcc		%o4, 0x40, %o4
	bne,pt		%icc, 4b
	 add		%o0, 0x40, %o0
	brz,pn		%g1, 6f
	 nop
5:	stx		%o2, [%o0 + 0x00]
	subcc		%g1, 8, %g1
	bne,pt		%icc, 5b
	 add		%o0, 0x8, %o0
6:	brz,pt		%o1, __bzero_done
	 nop
__bzero_tiny:
1:	stb		%o2, [%o0 + 0x00]
	subcc		%o1, 1, %o1
	bne,pt		%icc, 1b
	 add		%o0, 1, %o0
__bzero_done:
	retl
	 mov		%o3, %o0
	.size		__bzero, .-__bzero
	.size		__memset, .-__memset
	.size		memset, .-memset

#define EX_ST(x,y)		\
98:	x,y;			\
	.section __ex_table,"a";\
	.align 4;		\
	.word 98b, __retl_o1;	\
	.text;			\
	.align 4;

	.globl	__clear_user
	.type	__clear_user, #function
__clear_user:		/* %o0=buf, %o1=len */
	brz,pn		%o1, __clear_user_done
	 cmp		%o1, 16
	bl,pn		%icc, __clear_user_tiny
	 EX_ST(prefetcha [%o0 + 0x00] %asi, #n_writes)
	andcc		%o0, 0x3, %g0
	be,pt		%icc, 2f
1:	 EX_ST(stba	%g0, [%o0 + 0x00] %asi)
	add		%o0, 1, %o0
	andcc		%o0, 0x3, %g0
	bne,pn		%icc, 1b
	 sub		%o1, 1, %o1
2:	andcc		%o0, 0x7, %g0
	be,pt		%icc, 3f
	 EX_ST(stwa	%g0, [%o0 + 0x00] %asi)
	sub		%o1, 4, %o1
	add		%o0, 4, %o0
3:	and		%o1, 0x38, %g1
	cmp		%o1, 0x40
	andn		%o1, 0x3f, %o4
	bl,pn		%icc, 5f
	 and		%o1, 0x7, %o1
	EX_ST(prefetcha	[%o0 + 0x040] %asi, #n_writes)
	EX_ST(prefetcha	[%o0 + 0x080] %asi, #n_writes)
	EX_ST(prefetcha	[%o0 + 0x0c0] %asi, #n_writes)
	EX_ST(prefetcha	[%o0 + 0x100] %asi, #n_writes)
	EX_ST(prefetcha	[%o0 + 0x140] %asi, #n_writes)
4:	EX_ST(prefetcha	[%o0 + 0x180] %asi, #n_writes)
	EX_ST(stxa	%g0, [%o0 + 0x00] %asi)
	EX_ST(stxa	%g0, [%o0 + 0x08] %asi)
	EX_ST(stxa	%g0, [%o0 + 0x10] %asi)
	EX_ST(stxa	%g0, [%o0 + 0x18] %asi)
	EX_ST(stxa	%g0, [%o0 + 0x20] %asi)
	EX_ST(stxa	%g0, [%o0 + 0x28] %asi)
	EX_ST(stxa	%g0, [%o0 + 0x30] %asi)
	EX_ST(stxa	%g0, [%o0 + 0x38] %asi)
	subcc		%o4, 0x40, %o4
	bne,pt		%icc, 4b
	 add		%o0, 0x40, %o0
	brz,pn		%g1, 6f
	 nop
5:	EX_ST(stxa	%g0, [%o0 + 0x00] %asi)
	subcc		%g1, 8, %g1
	bne,pt		%icc, 5b
	 add		%o0, 0x8, %o0
6:	brz,pt		%o1, __clear_user_done
	 nop
__clear_user_tiny:
1:	EX_ST(stba	%g0, [%o0 + 0x00] %asi)
	subcc		%o1, 1, %o1
	bne,pt		%icc, 1b
	 add		%o0, 1, %o0
__clear_user_done:
	retl
	 clr		%o0
	.size		__clear_user, .-__clear_user
