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
 * blockops.S: Common block zero optimized routines.
 *
 * Copyright (C) 1996 David S. Miller (davem@caip.rutgers.edu)
 */

#include <asm/page.h>

	/* Zero out 64 bytes of memory at (buf + offset).
	 * Assumes %g1 contains zero.
	 */
#define BLAST_BLOCK(buf, offset) \
	std	%g0, [buf + offset + 0x38]; \
	std	%g0, [buf + offset + 0x30]; \
	std	%g0, [buf + offset + 0x28]; \
	std	%g0, [buf + offset + 0x20]; \
	std	%g0, [buf + offset + 0x18]; \
	std	%g0, [buf + offset + 0x10]; \
	std	%g0, [buf + offset + 0x08]; \
	std	%g0, [buf + offset + 0x00];

	/* Copy 32 bytes of memory at (src + offset) to
	 * (dst + offset).
	 */
#define MIRROR_BLOCK(dst, src, offset, t0, t1, t2, t3, t4, t5, t6, t7) \
	ldd	[src + offset + 0x18], t0; \
	ldd	[src + offset + 0x10], t2; \
	ldd	[src + offset + 0x08], t4; \
	ldd	[src + offset + 0x00], t6; \
	std	t0, [dst + offset + 0x18]; \
	std	t2, [dst + offset + 0x10]; \
	std	t4, [dst + offset + 0x08]; \
	std	t6, [dst + offset + 0x00];

	/* Profiling evidence indicates that memset() is
	 * commonly called for blocks of size PAGE_SIZE,
	 * and (2 * PAGE_SIZE) (for kernel stacks)
	 * and with a second arg of zero.  We assume in
	 * all of these cases that the buffer is aligned
	 * on at least an 8 byte boundary.
	 *
	 * Therefore we special case them to make them
	 * as fast as possible.
	 */

	.text
	.align	4
	.globl	bzero_1page, __copy_1page

bzero_1page:
/* NOTE: If you change the number of insns of this routine, please check
 * arch/sparc/mm/hypersparc.S */
	/* %o0 = buf */
	or	%g0, %g0, %g1
	or	%o0, %g0, %o1
	or	%g0, (PAGE_SIZE >> 8), %g2
1:
	BLAST_BLOCK(%o0, 0x00)
	BLAST_BLOCK(%o0, 0x40)
	BLAST_BLOCK(%o0, 0x80)
	BLAST_BLOCK(%o0, 0xc0)
	subcc	%g2, 1, %g2
	bne	1b
	 add	%o0, 0x100, %o0

	retl
	 nop

__copy_1page:
/* NOTE: If you change the number of insns of this routine, please check
 * arch/sparc/mm/hypersparc.S */
	/* %o0 = dst, %o1 = src */
	or	%g0, (PAGE_SIZE >> 8), %g1
1:
	MIRROR_BLOCK(%o0, %o1, 0x00, %o2, %o3, %o4, %o5, %g2, %g3, %g4, %g5)
	MIRROR_BLOCK(%o0, %o1, 0x20, %o2, %o3, %o4, %o5, %g2, %g3, %g4, %g5)
	MIRROR_BLOCK(%o0, %o1, 0x40, %o2, %o3, %o4, %o5, %g2, %g3, %g4, %g5)
	MIRROR_BLOCK(%o0, %o1, 0x60, %o2, %o3, %o4, %o5, %g2, %g3, %g4, %g5)
	MIRROR_BLOCK(%o0, %o1, 0x80, %o2, %o3, %o4, %o5, %g2, %g3, %g4, %g5)
	MIRROR_BLOCK(%o0, %o1, 0xa0, %o2, %o3, %o4, %o5, %g2, %g3, %g4, %g5)
	MIRROR_BLOCK(%o0, %o1, 0xc0, %o2, %o3, %o4, %o5, %g2, %g3, %g4, %g5)
	MIRROR_BLOCK(%o0, %o1, 0xe0, %o2, %o3, %o4, %o5, %g2, %g3, %g4, %g5)
	subcc	%g1, 1, %g1
	add	%o0, 0x100, %o0
	bne	1b
	 add	%o1, 0x100, %o1

	retl
	 nop
