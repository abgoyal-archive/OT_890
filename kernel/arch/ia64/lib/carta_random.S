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
 * Fast, simple, yet decent quality random number generator based on
 * a paper by David G. Carta ("Two Fast Implementations of the
 * `Minimal Standard' Random Number Generator," Communications of the
 * ACM, January, 1990).
 *
 * Copyright (C) 2002 Hewlett-Packard Co
 *	David Mosberger-Tang <davidm@hpl.hp.com>
 */

#include <asm/asmmacro.h>

#define a	r2
#define m	r3
#define lo	r8
#define hi	r9
#define t0	r16
#define t1	r17
#define	seed	r32

GLOBAL_ENTRY(carta_random32)
	movl	a = (16807 << 16) | 16807
	;;
	pmpyshr2.u t0 = a, seed, 0
	pmpyshr2.u t1 = a, seed, 16
	;;
	unpack2.l t0 = t1, t0
	dep	m = -1, r0, 0, 31
	;;
	zxt4	lo = t0
	shr.u	hi = t0, 32
	;;
	dep	t0 = 0, hi, 15, 49	// t0 = (hi & 0x7fff)
	;;
	shl	t0 = t0, 16		// t0 = (hi & 0x7fff) << 16
	shr	t1 = hi, 15		// t1 = (hi >> 15)
	;;
	add	lo = lo, t0
	;;
	cmp.gtu	p6, p0 = lo, m
	;;
(p6)	and	lo = lo, m
	;;
(p6)	add	lo = 1, lo
	;;
	add	lo = lo, t1
	;;
	cmp.gtu p6, p0 = lo, m
	;;
(p6)	and	lo = lo, m
	;;
(p6)	add	lo = 1, lo
	br.ret.sptk.many rp
END(carta_random32)
