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
 * memscan.S: Optimized memscan for Sparc64.
 *
 * Copyright (C) 1997,1998 Jakub Jelinek (jj@ultra.linux.cz)
 * Copyright (C) 1998 David S. Miller (davem@redhat.com)
 */

#define HI_MAGIC	0x8080808080808080
#define LO_MAGIC	0x0101010101010101
#define ASI_PL		0x88

	.text
	.align	32
	.globl		__memscan_zero, __memscan_generic
	.globl		memscan

__memscan_zero:
	/* %o0 = bufp, %o1 = size */
	brlez,pn	%o1, szzero
	 andcc		%o0, 7, %g0
	be,pt		%icc, we_are_aligned
	 sethi		%hi(HI_MAGIC), %o4
	ldub		[%o0], %o5
1:	subcc		%o1, 1, %o1
	brz,pn		%o5, 10f
	 add		%o0, 1, %o0

	be,pn		%xcc, szzero
	 andcc		%o0, 7, %g0
	bne,a,pn	%icc, 1b
	 ldub		[%o0], %o5
we_are_aligned:
	ldxa		[%o0] ASI_PL, %o5
	or		%o4, %lo(HI_MAGIC), %o3
	sllx		%o3, 32, %o4
	or		%o4, %o3, %o3

	srlx		%o3, 7, %o2
msloop:
	sub		%o1, 8, %o1
	add		%o0, 8, %o0
	sub		%o5, %o2, %o4
	xor		%o4, %o5, %o4
	andcc		%o4, %o3, %g3
	bne,pn		%xcc, check_bytes
	 srlx		%o4, 32, %g3

	brgz,a,pt	%o1, msloop
	 ldxa		[%o0] ASI_PL, %o5
check_bytes:
	bne,a,pn	%icc, 2f
	 andcc		%o5, 0xff, %g0
	add		%o0, -5, %g2
	ba,pt		%xcc, 3f
	 srlx		%o5, 32, %g7

2:	srlx		%o5, 8, %g7
	be,pn		%icc, 1f
	 add		%o0, -8, %g2
	andcc		%g7, 0xff, %g0
	srlx		%g7, 8, %g7
	be,pn		%icc, 1f
	 inc		%g2
	andcc		%g7, 0xff, %g0

	srlx		%g7, 8, %g7
	be,pn		%icc, 1f
	 inc		%g2
	andcc		%g7, 0xff, %g0
	srlx		%g7, 8, %g7
	be,pn		%icc, 1f
	 inc		%g2
	andcc		%g3, %o3, %g0

	be,a,pn		%icc, 2f
	 mov		%o0, %g2
3:	andcc		%g7, 0xff, %g0
	srlx		%g7, 8, %g7
	be,pn		%icc, 1f
	 inc		%g2
	andcc		%g7, 0xff, %g0
	srlx		%g7, 8, %g7

	be,pn		%icc, 1f
	 inc		%g2
	andcc		%g7, 0xff, %g0
	srlx		%g7, 8, %g7
	be,pn		%icc, 1f
	 inc		%g2
	andcc		%g7, 0xff, %g0
	srlx		%g7, 8, %g7

	be,pn		%icc, 1f
	 inc		%g2
2:	brgz,a,pt	%o1, msloop
	 ldxa		[%o0] ASI_PL, %o5
	inc		%g2
1:	add		%o0, %o1, %o0
	cmp		%g2, %o0
	retl

	 movle		%xcc, %g2, %o0
10:	retl
	 sub		%o0, 1, %o0
szzero:	retl
	 nop

memscan:
__memscan_generic:
	/* %o0 = addr, %o1 = c, %o2 = size */
	brz,pn		%o2, 3f
	 add		%o0, %o2, %o3
	ldub		[%o0], %o5
	sub		%g0, %o2, %o4
1:
	cmp		%o5, %o1
	be,pn		%icc, 2f
	 addcc		%o4, 1, %o4
	bne,a,pt 	%xcc, 1b
	 ldub		[%o3 + %o4], %o5
	retl
	/* The delay slot is the same as the next insn, this is just to make it look more awful */
2:
	 add		%o3, %o4, %o0
	retl
	 sub		%o0, 1, %o0
3:
	retl
	 nop
