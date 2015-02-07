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

#ifndef _ASM_S390_BUG_H
#define _ASM_S390_BUG_H

#include <linux/kernel.h>

#ifdef CONFIG_BUG

#ifdef CONFIG_64BIT
#define S390_LONG ".quad"
#else
#define S390_LONG ".long"
#endif

#ifdef CONFIG_DEBUG_BUGVERBOSE

#define __EMIT_BUG(x) do {					\
	asm volatile(						\
		"0:	j	0b+2\n"				\
		"1:\n"						\
		".section .rodata.str,\"aMS\",@progbits,1\n"	\
		"2:	.asciz	\""__FILE__"\"\n"		\
		".previous\n"					\
		".section __bug_table,\"a\"\n"			\
		"3:\t"	S390_LONG "\t1b,2b\n"			\
		"	.short	%0,%1\n"			\
		"	.org	3b+%2\n"			\
		".previous\n"					\
		: : "i" (__LINE__),				\
		    "i" (x),					\
		    "i" (sizeof(struct bug_entry)));		\
} while (0)

#else /* CONFIG_DEBUG_BUGVERBOSE */

#define __EMIT_BUG(x) do {				\
	asm volatile(					\
		"0:	j	0b+2\n"			\
		"1:\n"					\
		".section __bug_table,\"a\"\n"		\
		"2:\t"	S390_LONG "\t1b\n"		\
		"	.short	%0\n"			\
		"	.org	2b+%1\n"		\
		".previous\n"				\
		: : "i" (x),				\
		    "i" (sizeof(struct bug_entry)));	\
} while (0)

#endif /* CONFIG_DEBUG_BUGVERBOSE */

#define BUG() do {					\
	__EMIT_BUG(0);					\
	for (;;);					\
} while (0)

#define WARN_ON(x) ({					\
	int __ret_warn_on = !!(x);			\
	if (__builtin_constant_p(__ret_warn_on)) {	\
		if (__ret_warn_on)			\
			__EMIT_BUG(BUGFLAG_WARNING);	\
	} else {					\
		if (unlikely(__ret_warn_on))		\
			__EMIT_BUG(BUGFLAG_WARNING);	\
	}						\
	unlikely(__ret_warn_on);			\
})

#define HAVE_ARCH_BUG
#define HAVE_ARCH_WARN_ON
#endif /* CONFIG_BUG */

#include <asm-generic/bug.h>

#endif /* _ASM_S390_BUG_H */
