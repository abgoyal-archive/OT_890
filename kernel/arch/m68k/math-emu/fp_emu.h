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
 * fp_emu.h
 *
 * Copyright Roman Zippel, 1997.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, and the entire permission notice in its entirety,
 *    including the disclaimer of warranties.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior
 *    written permission.
 *
 * ALTERNATIVELY, this product may be distributed under the terms of
 * the GNU General Public License, in which case the provisions of the GPL are
 * required INSTEAD OF the above restrictions.  (This clause is
 * necessary due to a potential bad interaction between the GPL and
 * the restrictions contained in a BSD-style copyright.)
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _FP_EMU_H
#define _FP_EMU_H

#ifdef __ASSEMBLY__
#include <asm/asm-offsets.h>
#endif
#include <asm/math-emu.h>

#ifndef __ASSEMBLY__

#define IS_INF(a) ((a)->exp == 0x7fff)
#define IS_ZERO(a) ((a)->mant.m64 == 0)


#define fp_set_sr(bit) ({					\
	FPDATA->fpsr |= 1 << (bit);				\
})

#define fp_set_quotient(quotient) ({				\
	FPDATA->fpsr &= 0xff00ffff;				\
	FPDATA->fpsr |= ((quotient) & 0xff) << 16;		\
})

/* linkage for several useful functions */

/* Normalize the extended struct, return 0 for a NaN */
#define fp_normalize_ext(fpreg) ({				\
	register struct fp_ext *reg asm ("a0") = fpreg;		\
	register int res asm ("d0");				\
								\
	asm volatile ("jsr fp_conv_ext2ext"			\
			: "=d" (res) : "a" (reg)		\
			: "a1", "d1", "d2", "memory");		\
	res;							\
})

#define fp_copy_ext(dest, src) ({				\
	*dest = *src;						\
})

#define fp_monadic_check(dest, src) ({				\
	fp_copy_ext(dest, src);					\
	if (!fp_normalize_ext(dest))				\
		return dest;					\
})

#define fp_dyadic_check(dest, src) ({				\
	if (!fp_normalize_ext(dest))				\
		return dest;					\
	if (!fp_normalize_ext(src)) {				\
		fp_copy_ext(dest, src);				\
		return dest;					\
	}							\
})

extern const struct fp_ext fp_QNaN;
extern const struct fp_ext fp_Inf;

#define fp_set_nan(dest) ({					\
	fp_set_sr(FPSR_EXC_OPERR);				\
	*dest = fp_QNaN;					\
})

/* TODO check rounding mode? */
#define fp_set_ovrflw(dest) ({					\
	fp_set_sr(FPSR_EXC_OVFL);				\
	dest->exp = 0x7fff;					\
	dest->mant.m64 = 0;					\
})

#define fp_conv_ext2long(src) ({				\
	register struct fp_ext *__src asm ("a0") = src;		\
	register int __res asm ("d0");				\
								\
	asm volatile ("jsr fp_conv_ext2long"			\
			: "=d" (__res) : "a" (__src)		\
			: "a1", "d1", "d2", "memory");		\
	__res;							\
})

#define fp_conv_long2ext(dest, src) ({				\
	register struct fp_ext *__dest asm ("a0") = dest;	\
	register int __src asm ("d0") = src;			\
								\
	asm volatile ("jsr fp_conv_ext2long"			\
			: : "d" (__src), "a" (__dest)		\
			: "a1", "d1", "d2", "memory");		\
})

#else /* __ASSEMBLY__ */

/*
 * set, reset or clear a bit in the fp status register
 */
.macro	fp_set_sr	bit
	bset	#(\bit&7),(FPD_FPSR+3-(\bit/8),FPDATA)
.endm

.macro	fp_clr_sr	bit
	bclr	#(\bit&7),(FPD_FPSR+3-(\bit/8),FPDATA)
.endm

.macro	fp_tst_sr	bit
	btst	#(\bit&7),(FPD_FPSR+3-(\bit/8),FPDATA)
.endm

#endif /* __ASSEMBLY__ */

#endif /* _FP_EMU_H */
