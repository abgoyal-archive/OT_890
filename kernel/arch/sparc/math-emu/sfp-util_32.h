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

#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/types.h>
#include <asm/byteorder.h>

#define add_ssaaaa(sh, sl, ah, al, bh, bl) 				\
  __asm__ ("addcc %r4,%5,%1\n\t"						\
	   "addx %r2,%3,%0\n"						\
	   : "=r" ((USItype)(sh)),					\
	     "=&r" ((USItype)(sl))					\
	   : "%rJ" ((USItype)(ah)),					\
	     "rI" ((USItype)(bh)),					\
	     "%rJ" ((USItype)(al)),					\
	     "rI" ((USItype)(bl))					\
	   : "cc")
#define sub_ddmmss(sh, sl, ah, al, bh, bl) 				\
  __asm__ ("subcc %r4,%5,%1\n\t"						\
	   "subx %r2,%3,%0\n"						\
	   : "=r" ((USItype)(sh)),					\
	     "=&r" ((USItype)(sl))					\
	   : "rJ" ((USItype)(ah)),					\
	     "rI" ((USItype)(bh)),					\
	     "rJ" ((USItype)(al)),					\
	     "rI" ((USItype)(bl))					\
	   : "cc")

#define umul_ppmm(w1, w0, u, v) \
  __asm__ ("! Inlined umul_ppmm\n\t"					\
	"wr	%%g0,%2,%%y	! SPARC has 0-3 delay insn after a wr\n\t" \
	"sra	%3,31,%%g2	! Don't move this insn\n\t"		\
	"and	%2,%%g2,%%g2	! Don't move this insn\n\t"		\
	"andcc	%%g0,0,%%g1	! Don't move this insn\n\t"		\
	"mulscc	%%g1,%3,%%g1\n\t"					\
	"mulscc	%%g1,%3,%%g1\n\t"					\
	"mulscc	%%g1,%3,%%g1\n\t"					\
	"mulscc	%%g1,%3,%%g1\n\t"					\
	"mulscc	%%g1,%3,%%g1\n\t"					\
	"mulscc	%%g1,%3,%%g1\n\t"					\
	"mulscc	%%g1,%3,%%g1\n\t"					\
	"mulscc	%%g1,%3,%%g1\n\t"					\
	"mulscc	%%g1,%3,%%g1\n\t"					\
	"mulscc	%%g1,%3,%%g1\n\t"					\
	"mulscc	%%g1,%3,%%g1\n\t"					\
	"mulscc	%%g1,%3,%%g1\n\t"					\
	"mulscc	%%g1,%3,%%g1\n\t"					\
	"mulscc	%%g1,%3,%%g1\n\t"					\
	"mulscc	%%g1,%3,%%g1\n\t"					\
	"mulscc	%%g1,%3,%%g1\n\t"					\
	"mulscc	%%g1,%3,%%g1\n\t"					\
	"mulscc	%%g1,%3,%%g1\n\t"					\
	"mulscc	%%g1,%3,%%g1\n\t"					\
	"mulscc	%%g1,%3,%%g1\n\t"					\
	"mulscc	%%g1,%3,%%g1\n\t"					\
	"mulscc	%%g1,%3,%%g1\n\t"					\
	"mulscc	%%g1,%3,%%g1\n\t"					\
	"mulscc	%%g1,%3,%%g1\n\t"					\
	"mulscc	%%g1,%3,%%g1\n\t"					\
	"mulscc	%%g1,%3,%%g1\n\t"					\
	"mulscc	%%g1,%3,%%g1\n\t"					\
	"mulscc	%%g1,%3,%%g1\n\t"					\
	"mulscc	%%g1,%3,%%g1\n\t"					\
	"mulscc	%%g1,%3,%%g1\n\t"					\
	"mulscc	%%g1,%3,%%g1\n\t"					\
	"mulscc	%%g1,%3,%%g1\n\t"					\
	"mulscc	%%g1,0,%%g1\n\t" 					\
	"add	%%g1,%%g2,%0\n\t" 					\
	"rd	%%y,%1\n"						\
	   : "=r" ((USItype)(w1)),					\
	     "=r" ((USItype)(w0))					\
	   : "%rI" ((USItype)(u)),					\
	     "r" ((USItype)(v))						\
	   : "%g1", "%g2", "cc")

/* It's quite necessary to add this much assembler for the sparc.
   The default udiv_qrnnd (in C) is more than 10 times slower!  */
#define udiv_qrnnd(q, r, n1, n0, d) \
  __asm__ ("! Inlined udiv_qrnnd\n\t"					\
	   "mov	32,%%g1\n\t"						\
	   "subcc	%1,%2,%%g0\n\t"					\
	   "1:	bcs	5f\n\t"						\
	   "addxcc %0,%0,%0	! shift n1n0 and a q-bit in lsb\n\t"	\
	   "sub	%1,%2,%1	! this kills msb of n\n\t"		\
	   "addx	%1,%1,%1	! so this can't give carry\n\t"	\
	   "subcc	%%g1,1,%%g1\n\t"				\
	   "2:	bne	1b\n\t"						\
	   "subcc	%1,%2,%%g0\n\t"					\
	   "bcs	3f\n\t"							\
	   "addxcc %0,%0,%0	! shift n1n0 and a q-bit in lsb\n\t"	\
	   "b		3f\n\t"						\
	   "sub	%1,%2,%1	! this kills msb of n\n\t"		\
	   "4:	sub	%1,%2,%1\n\t"					\
	   "5:	addxcc	%1,%1,%1\n\t"					\
	   "bcc	2b\n\t"							\
	   "subcc	%%g1,1,%%g1\n\t"				\
	   "! Got carry from n.  Subtract next step to cancel this carry.\n\t" \
	   "bne	4b\n\t"							\
	   "addcc	%0,%0,%0	! shift n1n0 and a 0-bit in lsb\n\t" \
	   "sub	%1,%2,%1\n\t"						\
	   "3:	xnor	%0,0,%0\n\t"					\
	   "! End of inline udiv_qrnnd\n"				\
	   : "=&r" ((USItype)(q)),					\
	     "=&r" ((USItype)(r))					\
	   : "r" ((USItype)(d)),					\
	     "1" ((USItype)(n1)),					\
	     "0" ((USItype)(n0)) : "%g1", "cc")
#define UDIV_NEEDS_NORMALIZATION 0

#define abort()								\
	return 0

#ifdef __BIG_ENDIAN
#define __BYTE_ORDER __BIG_ENDIAN
#else
#define __BYTE_ORDER __LITTLE_ENDIAN
#endif
