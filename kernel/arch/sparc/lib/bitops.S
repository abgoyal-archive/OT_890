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

/* bitops.S: Sparc64 atomic bit operations.
 *
 * Copyright (C) 2000, 2007 David S. Miller (davem@davemloft.net)
 */

#include <asm/asi.h>
#include <asm/backoff.h>

	.text

	.globl	test_and_set_bit
	.type	test_and_set_bit,#function
test_and_set_bit:	/* %o0=nr, %o1=addr */
	BACKOFF_SETUP(%o3)
	srlx	%o0, 6, %g1
	mov	1, %o2
	sllx	%g1, 3, %g3
	and	%o0, 63, %g2
	sllx	%o2, %g2, %o2
	add	%o1, %g3, %o1
1:	ldx	[%o1], %g7
	or	%g7, %o2, %g1
	casx	[%o1], %g7, %g1
	cmp	%g7, %g1
	bne,pn	%xcc, 2f
	 and	%g7, %o2, %g2
	clr	%o0
	movrne	%g2, 1, %o0
	retl
	 nop
2:	BACKOFF_SPIN(%o3, %o4, 1b)
	.size	test_and_set_bit, .-test_and_set_bit

	.globl	test_and_clear_bit
	.type	test_and_clear_bit,#function
test_and_clear_bit:	/* %o0=nr, %o1=addr */
	BACKOFF_SETUP(%o3)
	srlx	%o0, 6, %g1
	mov	1, %o2
	sllx	%g1, 3, %g3
	and	%o0, 63, %g2
	sllx	%o2, %g2, %o2
	add	%o1, %g3, %o1
1:	ldx	[%o1], %g7
	andn	%g7, %o2, %g1
	casx	[%o1], %g7, %g1
	cmp	%g7, %g1
	bne,pn	%xcc, 2f
	 and	%g7, %o2, %g2
	clr	%o0
	movrne	%g2, 1, %o0
	retl
	 nop
2:	BACKOFF_SPIN(%o3, %o4, 1b)
	.size	test_and_clear_bit, .-test_and_clear_bit

	.globl	test_and_change_bit
	.type	test_and_change_bit,#function
test_and_change_bit:	/* %o0=nr, %o1=addr */
	BACKOFF_SETUP(%o3)
	srlx	%o0, 6, %g1
	mov	1, %o2
	sllx	%g1, 3, %g3
	and	%o0, 63, %g2
	sllx	%o2, %g2, %o2
	add	%o1, %g3, %o1
1:	ldx	[%o1], %g7
	xor	%g7, %o2, %g1
	casx	[%o1], %g7, %g1
	cmp	%g7, %g1
	bne,pn	%xcc, 2f
	 and	%g7, %o2, %g2
	clr	%o0
	movrne	%g2, 1, %o0
	retl
	 nop
2:	BACKOFF_SPIN(%o3, %o4, 1b)
	.size	test_and_change_bit, .-test_and_change_bit

	.globl	set_bit
	.type	set_bit,#function
set_bit:		/* %o0=nr, %o1=addr */
	BACKOFF_SETUP(%o3)
	srlx	%o0, 6, %g1
	mov	1, %o2
	sllx	%g1, 3, %g3
	and	%o0, 63, %g2
	sllx	%o2, %g2, %o2
	add	%o1, %g3, %o1
1:	ldx	[%o1], %g7
	or	%g7, %o2, %g1
	casx	[%o1], %g7, %g1
	cmp	%g7, %g1
	bne,pn	%xcc, 2f
	 nop
	retl
	 nop
2:	BACKOFF_SPIN(%o3, %o4, 1b)
	.size	set_bit, .-set_bit

	.globl	clear_bit
	.type	clear_bit,#function
clear_bit:		/* %o0=nr, %o1=addr */
	BACKOFF_SETUP(%o3)
	srlx	%o0, 6, %g1
	mov	1, %o2
	sllx	%g1, 3, %g3
	and	%o0, 63, %g2
	sllx	%o2, %g2, %o2
	add	%o1, %g3, %o1
1:	ldx	[%o1], %g7
	andn	%g7, %o2, %g1
	casx	[%o1], %g7, %g1
	cmp	%g7, %g1
	bne,pn	%xcc, 2f
	 nop
	retl
	 nop
2:	BACKOFF_SPIN(%o3, %o4, 1b)
	.size	clear_bit, .-clear_bit

	.globl	change_bit
	.type	change_bit,#function
change_bit:		/* %o0=nr, %o1=addr */
	BACKOFF_SETUP(%o3)
	srlx	%o0, 6, %g1
	mov	1, %o2
	sllx	%g1, 3, %g3
	and	%o0, 63, %g2
	sllx	%o2, %g2, %o2
	add	%o1, %g3, %o1
1:	ldx	[%o1], %g7
	xor	%g7, %o2, %g1
	casx	[%o1], %g7, %g1
	cmp	%g7, %g1
	bne,pn	%xcc, 2f
	 nop
	retl
	 nop
2:	BACKOFF_SPIN(%o3, %o4, 1b)
	.size	change_bit, .-change_bit
