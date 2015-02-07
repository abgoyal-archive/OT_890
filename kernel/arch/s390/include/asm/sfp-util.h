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

#define add_ssaaaa(sh, sl, ah, al, bh, bl) ({		\
	unsigned int __sh = (ah);			\
	unsigned int __sl = (al);			\
	asm volatile(					\
		"	alr	%1,%3\n"		\
		"	brc	12,0f\n"		\
		"	ahi	%0,1\n"			\
		"0:	alr  %0,%2"			\
		: "+&d" (__sh), "+d" (__sl)		\
		: "d" (bh), "d" (bl) : "cc");		\
	(sh) = __sh;					\
	(sl) = __sl;					\
})

#define sub_ddmmss(sh, sl, ah, al, bh, bl) ({		\
	unsigned int __sh = (ah);			\
	unsigned int __sl = (al);			\
	asm volatile(					\
		"	slr	%1,%3\n"		\
		"	brc	3,0f\n"			\
		"	ahi	%0,-1\n"		\
		"0:	slr	%0,%2"			\
		: "+&d" (__sh), "+d" (__sl)		\
		: "d" (bh), "d" (bl) : "cc");		\
	(sh) = __sh;					\
	(sl) = __sl;					\
})

/* a umul b = a mul b + (a>=2<<31) ? b<<32:0 + (b>=2<<31) ? a<<32:0 */
#define umul_ppmm(wh, wl, u, v) ({			\
	unsigned int __wh = u;				\
	unsigned int __wl = v;				\
	asm volatile(					\
		"	ltr	1,%0\n"			\
		"	mr	0,%1\n"			\
		"	jnm	0f\n"				\
		"	alr	0,%1\n"			\
		"0:	ltr	%1,%1\n"			\
		"	jnm	1f\n"				\
		"	alr	0,%0\n"			\
		"1:	lr	%0,0\n"			\
		"	lr	%1,1\n"			\
		: "+d" (__wh), "+d" (__wl)		\
		: : "0", "1", "cc");			\
	wh = __wh;					\
	wl = __wl;					\
})

#ifdef __s390x__
#define udiv_qrnnd(q, r, n1, n0, d)			\
  do { unsigned long __n;				\
       unsigned int __r, __d;				\
    __n = ((unsigned long)(n1) << 32) + n0;		\
    __d = (d);						\
    (q) = __n / __d;					\
    (r) = __n % __d;					\
  } while (0)
#else
#define udiv_qrnnd(q, r, n1, n0, d)			\
  do { unsigned int __r;				\
    (q) = __udiv_qrnnd (&__r, (n1), (n0), (d));		\
    (r) = __r;						\
  } while (0)
extern unsigned long __udiv_qrnnd (unsigned int *, unsigned int,
				   unsigned int , unsigned int);
#endif

#define UDIV_NEEDS_NORMALIZATION 0

#define abort() return 0

#define __BYTE_ORDER __BIG_ENDIAN
