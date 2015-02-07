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
 * memscan.S: Optimized memscan for the Sparc.
 *
 * Copyright (C) 1996 David S. Miller (davem@caip.rutgers.edu)
 */

/* In essence, this is just a fancy strlen. */

#define LO_MAGIC 0x01010101
#define HI_MAGIC 0x80808080

	.text
	.align	4
	.globl	__memscan_zero, __memscan_generic
	.globl	memscan
__memscan_zero:
	/* %o0 = addr, %o1 = size */
	cmp	%o1, 0
	bne,a	1f
	 andcc	%o0, 3, %g0

	retl
	 nop

1:
	be	mzero_scan_word
	 sethi	%hi(HI_MAGIC), %g2

	ldsb	[%o0], %g3
mzero_still_not_word_aligned:
	cmp	%g3, 0
	bne	1f
	 add	%o0, 1, %o0

	retl
	 sub	%o0, 1, %o0

1:
	subcc	%o1, 1, %o1
	bne,a	1f
	 andcc	%o0, 3, %g0

	retl
	 nop

1:
	bne,a	mzero_still_not_word_aligned
	 ldsb	[%o0], %g3

	sethi	%hi(HI_MAGIC), %g2
mzero_scan_word:
	or	%g2, %lo(HI_MAGIC), %o3
	sethi	%hi(LO_MAGIC), %g3
	or	%g3, %lo(LO_MAGIC), %o2
mzero_next_word:
	ld	[%o0], %g2
mzero_next_word_preloaded:
	sub	%g2, %o2, %g2
mzero_next_word_preloaded_next:
	andcc	%g2, %o3, %g0
	bne	mzero_byte_zero
	 add	%o0, 4, %o0

mzero_check_out_of_fuel:
	subcc	%o1, 4, %o1
	bg,a	1f
	 ld	[%o0], %g2

	retl
	 nop

1:
	b	mzero_next_word_preloaded_next
	 sub	%g2, %o2, %g2

	/* Check every byte. */
mzero_byte_zero:
	ldsb	[%o0 - 4], %g2
	cmp	%g2, 0
	bne	mzero_byte_one
	 sub	%o0, 4, %g3

	retl
	 mov	%g3, %o0

mzero_byte_one:
	ldsb	[%o0 - 3], %g2
	cmp	%g2, 0
	bne,a	mzero_byte_two_and_three
	 ldsb	[%o0 - 2], %g2

	retl
	 sub	%o0, 3, %o0

mzero_byte_two_and_three:
	cmp	%g2, 0
	bne,a	1f
	 ldsb	[%o0 - 1], %g2

	retl
	 sub	%o0, 2, %o0

1:
	cmp	%g2, 0
	bne,a	mzero_next_word_preloaded
	 ld	[%o0], %g2

	retl
	 sub	%o0, 1, %o0

mzero_found_it:
	retl
	 sub	%o0, 2, %o0

memscan:
__memscan_generic:
	/* %o0 = addr, %o1 = c, %o2 = size */
	cmp	%o2, 0
	bne,a	0f
	 ldub	[%o0], %g2

	b,a	2f
1:
	ldub	[%o0], %g2
0:
	cmp	%g2, %o1
	be	2f
	 addcc	%o2, -1, %o2
	bne	1b
	 add	%o0, 1, %o0
2:
	retl
	 nop
