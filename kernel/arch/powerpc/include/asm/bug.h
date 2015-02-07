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

#ifndef _ASM_POWERPC_BUG_H
#define _ASM_POWERPC_BUG_H
#ifdef __KERNEL__

#include <asm/asm-compat.h>

/*
 * Define an illegal instr to trap on the bug.
 * We don't use 0 because that marks the end of a function
 * in the ELF ABI.  That's "Boo Boo" in case you wonder...
 */
#define BUG_OPCODE .long 0x00b00b00  /* For asm */
#define BUG_ILLEGAL_INSTR "0x00b00b00" /* For BUG macro */

#ifdef CONFIG_BUG

#ifdef __ASSEMBLY__
#include <asm/asm-offsets.h>
#ifdef CONFIG_DEBUG_BUGVERBOSE
.macro EMIT_BUG_ENTRY addr,file,line,flags
	 .section __bug_table,"a"
5001:	 PPC_LONG \addr, 5002f
	 .short \line, \flags
	 .org 5001b+BUG_ENTRY_SIZE
	 .previous
	 .section .rodata,"a"
5002:	 .asciz "\file"
	 .previous
.endm
#else
.macro EMIT_BUG_ENTRY addr,file,line,flags
	 .section __bug_table,"a"
5001:	 PPC_LONG \addr
	 .short \flags
	 .org 5001b+BUG_ENTRY_SIZE
	 .previous
.endm
#endif /* verbose */

#else /* !__ASSEMBLY__ */
/* _EMIT_BUG_ENTRY expects args %0,%1,%2,%3 to be FILE, LINE, flags and
   sizeof(struct bug_entry), respectively */
#ifdef CONFIG_DEBUG_BUGVERBOSE
#define _EMIT_BUG_ENTRY				\
	".section __bug_table,\"a\"\n"		\
	"2:\t" PPC_LONG "1b, %0\n"		\
	"\t.short %1, %2\n"			\
	".org 2b+%3\n"				\
	".previous\n"
#else
#define _EMIT_BUG_ENTRY				\
	".section __bug_table,\"a\"\n"		\
	"2:\t" PPC_LONG "1b\n"			\
	"\t.short %2\n"				\
	".org 2b+%3\n"				\
	".previous\n"
#endif

/*
 * BUG_ON() and WARN_ON() do their best to cooperate with compile-time
 * optimisations. However depending on the complexity of the condition
 * some compiler versions may not produce optimal results.
 */

#define BUG() do {						\
	__asm__ __volatile__(					\
		"1:	twi 31,0,0\n"				\
		_EMIT_BUG_ENTRY					\
		: : "i" (__FILE__), "i" (__LINE__),		\
		    "i" (0), "i"  (sizeof(struct bug_entry)));	\
	for(;;) ;						\
} while (0)

#define BUG_ON(x) do {						\
	if (__builtin_constant_p(x)) {				\
		if (x)						\
			BUG();					\
	} else {						\
		__asm__ __volatile__(				\
		"1:	"PPC_TLNEI"	%4,0\n"			\
		_EMIT_BUG_ENTRY					\
		: : "i" (__FILE__), "i" (__LINE__), "i" (0),	\
		  "i" (sizeof(struct bug_entry)),		\
		  "r" ((__force long)(x)));			\
	}							\
} while (0)

#define __WARN() do {						\
	__asm__ __volatile__(					\
		"1:	twi 31,0,0\n"				\
		_EMIT_BUG_ENTRY					\
		: : "i" (__FILE__), "i" (__LINE__),		\
		  "i" (BUGFLAG_WARNING),			\
		  "i" (sizeof(struct bug_entry)));		\
} while (0)

#define WARN_ON(x) ({						\
	int __ret_warn_on = !!(x);				\
	if (__builtin_constant_p(__ret_warn_on)) {		\
		if (__ret_warn_on)				\
			__WARN();				\
	} else {						\
		__asm__ __volatile__(				\
		"1:	"PPC_TLNEI"	%4,0\n"			\
		_EMIT_BUG_ENTRY					\
		: : "i" (__FILE__), "i" (__LINE__),		\
		  "i" (BUGFLAG_WARNING),			\
		  "i" (sizeof(struct bug_entry)),		\
		  "r" (__ret_warn_on));				\
	}							\
	unlikely(__ret_warn_on);				\
})

#define HAVE_ARCH_BUG
#define HAVE_ARCH_BUG_ON
#define HAVE_ARCH_WARN_ON
#endif /* __ASSEMBLY __ */
#else
#ifdef __ASSEMBLY__
.macro EMIT_BUG_ENTRY addr,file,line,flags
.endm
#else /* !__ASSEMBLY__ */
#define _EMIT_BUG_ENTRY
#endif
#endif /* CONFIG_BUG */

#include <asm-generic/bug.h>

#endif /* __KERNEL__ */
#endif /* _ASM_POWERPC_BUG_H */
