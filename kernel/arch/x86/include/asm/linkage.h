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

#ifndef _ASM_X86_LINKAGE_H
#define _ASM_X86_LINKAGE_H

#undef notrace
#define notrace __attribute__((no_instrument_function))

#ifdef CONFIG_X86_64
#define __ALIGN .p2align 4,,15
#define __ALIGN_STR ".p2align 4,,15"
#endif

#ifdef CONFIG_X86_32
#define asmlinkage CPP_ASMLINKAGE __attribute__((regparm(0)))
/*
 * For 32-bit UML - mark functions implemented in assembly that use
 * regparm input parameters:
 */
#define asmregparm __attribute__((regparm(3)))

/*
 * Make sure the compiler doesn't do anything stupid with the
 * arguments on the stack - they are owned by the *caller*, not
 * the callee. This just fools gcc into not spilling into them,
 * and keeps it from doing tailcall recursion and/or using the
 * stack slots for temporaries, since they are live and "used"
 * all the way to the end of the function.
 *
 * NOTE! On x86-64, all the arguments are in registers, so this
 * only matters on a 32-bit kernel.
 */
#define asmlinkage_protect(n, ret, args...) \
	__asmlinkage_protect##n(ret, ##args)
#define __asmlinkage_protect_n(ret, args...) \
	__asm__ __volatile__ ("" : "=r" (ret) : "0" (ret), ##args)
#define __asmlinkage_protect0(ret) \
	__asmlinkage_protect_n(ret)
#define __asmlinkage_protect1(ret, arg1) \
	__asmlinkage_protect_n(ret, "g" (arg1))
#define __asmlinkage_protect2(ret, arg1, arg2) \
	__asmlinkage_protect_n(ret, "g" (arg1), "g" (arg2))
#define __asmlinkage_protect3(ret, arg1, arg2, arg3) \
	__asmlinkage_protect_n(ret, "g" (arg1), "g" (arg2), "g" (arg3))
#define __asmlinkage_protect4(ret, arg1, arg2, arg3, arg4) \
	__asmlinkage_protect_n(ret, "g" (arg1), "g" (arg2), "g" (arg3), \
			      "g" (arg4))
#define __asmlinkage_protect5(ret, arg1, arg2, arg3, arg4, arg5) \
	__asmlinkage_protect_n(ret, "g" (arg1), "g" (arg2), "g" (arg3), \
			      "g" (arg4), "g" (arg5))
#define __asmlinkage_protect6(ret, arg1, arg2, arg3, arg4, arg5, arg6) \
	__asmlinkage_protect_n(ret, "g" (arg1), "g" (arg2), "g" (arg3), \
			      "g" (arg4), "g" (arg5), "g" (arg6))

#endif

#ifdef CONFIG_X86_ALIGNMENT_16
#define __ALIGN .align 16,0x90
#define __ALIGN_STR ".align 16,0x90"
#endif

/*
 * to check ENTRY_X86/END_X86 and
 * KPROBE_ENTRY_X86/KPROBE_END_X86
 * unbalanced-missed-mixed appearance
 */
#define __set_entry_x86		.set ENTRY_X86_IN, 0
#define __unset_entry_x86	.set ENTRY_X86_IN, 1
#define __set_kprobe_x86	.set KPROBE_X86_IN, 0
#define __unset_kprobe_x86	.set KPROBE_X86_IN, 1

#define __macro_err_x86 .error "ENTRY_X86/KPROBE_X86 unbalanced,missed,mixed"

#define __check_entry_x86	\
	.ifdef ENTRY_X86_IN;	\
	.ifeq ENTRY_X86_IN;	\
	__macro_err_x86;	\
	.abort;			\
	.endif;			\
	.endif

#define __check_kprobe_x86	\
	.ifdef KPROBE_X86_IN;	\
	.ifeq KPROBE_X86_IN;	\
	__macro_err_x86;	\
	.abort;			\
	.endif;			\
	.endif

#define __check_entry_kprobe_x86	\
	__check_entry_x86;		\
	__check_kprobe_x86

#define ENTRY_KPROBE_FINAL_X86 __check_entry_kprobe_x86

#define ENTRY_X86(name)			\
	__check_entry_kprobe_x86;	\
	__set_entry_x86;		\
	.globl name;			\
	__ALIGN;			\
	name:

#define END_X86(name)			\
	__unset_entry_x86;		\
	__check_entry_kprobe_x86;	\
	.size name, .-name

#define KPROBE_ENTRY_X86(name)		\
	__check_entry_kprobe_x86;	\
	__set_kprobe_x86;		\
	.pushsection .kprobes.text, "ax"; \
	.globl name;			\
	__ALIGN;			\
	name:

#define KPROBE_END_X86(name)		\
	__unset_kprobe_x86;		\
	__check_entry_kprobe_x86;	\
	.size name, .-name;		\
	.popsection

#endif /* _ASM_X86_LINKAGE_H */

