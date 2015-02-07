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
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1996, 1997, 1998, 1999, 2000 by Ralf Baechle
 * Copyright (C) 1999, 2000 Silicon Graphics, Inc.
 *
 * Protected memory access.  Used for everything that might take revenge
 * by sending a DBE error like accessing possibly non-existant memory or
 * devices.
 */
#ifndef _ASM_PACCESS_H
#define _ASM_PACCESS_H

#include <linux/errno.h>

#ifdef CONFIG_32BIT
#define __PA_ADDR	".word"
#endif
#ifdef CONFIG_64BIT
#define __PA_ADDR	".dword"
#endif

extern asmlinkage void handle_ibe(void);
extern asmlinkage void handle_dbe(void);

#define put_dbe(x, ptr) __put_dbe((x), (ptr), sizeof(*(ptr)))
#define get_dbe(x, ptr) __get_dbe((x), (ptr), sizeof(*(ptr)))

struct __large_pstruct { unsigned long buf[100]; };
#define __mp(x) (*(struct __large_pstruct *)(x))

#define __get_dbe(x, ptr, size)						\
({									\
	long __gu_err;							\
	__typeof__(*(ptr)) __gu_val;					\
	unsigned long __gu_addr;					\
	__asm__("":"=r" (__gu_val));					\
	__gu_addr = (unsigned long) (ptr);				\
	__asm__("":"=r" (__gu_err));					\
	switch (size) {							\
	case 1: __get_dbe_asm("lb"); break;				\
	case 2: __get_dbe_asm("lh"); break;				\
	case 4: __get_dbe_asm("lw"); break;				\
	case 8:  __get_dbe_asm("ld"); break;				\
	default: __get_dbe_unknown(); break;				\
	}								\
	x = (__typeof__(*(ptr))) __gu_val;				\
	__gu_err;							\
})

#define __get_dbe_asm(insn)						\
{									\
	__asm__ __volatile__(						\
	"1:\t" insn "\t%1,%2\n\t"					\
	"move\t%0,$0\n"							\
	"2:\n\t"							\
	".section\t.fixup,\"ax\"\n"					\
	"3:\tli\t%0,%3\n\t"						\
	"move\t%1,$0\n\t"						\
	"j\t2b\n\t"							\
	".previous\n\t"							\
	".section\t__dbe_table,\"a\"\n\t"				\
	__PA_ADDR "\t1b, 3b\n\t"					\
	".previous"							\
	:"=r" (__gu_err), "=r" (__gu_val)				\
	:"o" (__mp(__gu_addr)), "i" (-EFAULT));				\
}

extern void __get_dbe_unknown(void);

#define __put_dbe(x, ptr, size)						\
({									\
	long __pu_err;							\
	__typeof__(*(ptr)) __pu_val;					\
	long __pu_addr;							\
	__pu_val = (x);							\
	__pu_addr = (long) (ptr);					\
	__asm__("":"=r" (__pu_err));					\
	switch (size) {							\
	case 1: __put_dbe_asm("sb"); break;				\
	case 2: __put_dbe_asm("sh"); break;				\
	case 4: __put_dbe_asm("sw"); break;				\
	case 8: __put_dbe_asm("sd"); break;				\
	default: __put_dbe_unknown(); break;				\
	}								\
	__pu_err;							\
})

#define __put_dbe_asm(insn)						\
{									\
	__asm__ __volatile__(						\
	"1:\t" insn "\t%1,%2\n\t"					\
	"move\t%0,$0\n"							\
	"2:\n\t"							\
	".section\t.fixup,\"ax\"\n"					\
	"3:\tli\t%0,%3\n\t"						\
	"j\t2b\n\t"							\
	".previous\n\t"							\
	".section\t__dbe_table,\"a\"\n\t"				\
	__PA_ADDR "\t1b, 3b\n\t"					\
	".previous"							\
	: "=r" (__pu_err)						\
	: "r" (__pu_val), "o" (__mp(__pu_addr)), "i" (-EFAULT));	\
}

extern void __put_dbe_unknown(void);

extern unsigned long search_dbe_table(unsigned long addr);

#endif /* _ASM_PACCESS_H */
