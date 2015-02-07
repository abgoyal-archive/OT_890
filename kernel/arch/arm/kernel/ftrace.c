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
 * Dynamic function tracing support.
 *
 * Copyright (C) 2008 Abhishek Sagar <sagar.abhishek@gmail.com>
 *
 * For licencing details, see COPYING.
 *
 * Defines low-level handling of mcount calls when the kernel
 * is compiled with the -pg flag. When using dynamic ftrace, the
 * mcount call-sites get patched lazily with NOP till they are
 * enabled. All code mutation routines here take effect atomically.
 */

#include <linux/ftrace.h>

#include <asm/cacheflush.h>
#include <asm/ftrace.h>

#define PC_OFFSET      8
#define BL_OPCODE      0xeb000000
#define BL_OFFSET_MASK 0x00ffffff

static unsigned long bl_insn;
static const unsigned long NOP = 0xe1a00000; /* mov r0, r0 */

unsigned char *ftrace_nop_replace(void)
{
	return (char *)&NOP;
}

/* construct a branch (BL) instruction to addr */
unsigned char *ftrace_call_replace(unsigned long pc, unsigned long addr)
{
	long offset;

	offset = (long)addr - (long)(pc + PC_OFFSET);
	if (unlikely(offset < -33554432 || offset > 33554428)) {
		/* Can't generate branches that far (from ARM ARM). Ftrace
		 * doesn't generate branches outside of kernel text.
		 */
		WARN_ON_ONCE(1);
		return NULL;
	}
	offset = (offset >> 2) & BL_OFFSET_MASK;
	bl_insn = BL_OPCODE | offset;
	return (unsigned char *)&bl_insn;
}

int ftrace_modify_code(unsigned long pc, unsigned char *old_code,
		       unsigned char *new_code)
{
	unsigned long err = 0, replaced = 0, old, new;

	old = *(unsigned long *)old_code;
	new = *(unsigned long *)new_code;

	__asm__ __volatile__ (
		"1:  ldr    %1, [%2]  \n"
		"    cmp    %1, %4    \n"
		"2:  streq  %3, [%2]  \n"
		"    cmpne  %1, %3    \n"
		"    movne  %0, #2    \n"
		"3:\n"

		".section .fixup, \"ax\"\n"
		"4:  mov  %0, #1  \n"
		"    b    3b      \n"
		".previous\n"

		".section __ex_table, \"a\"\n"
		"    .long 1b, 4b \n"
		"    .long 2b, 4b \n"
		".previous\n"

		: "=r"(err), "=r"(replaced)
		: "r"(pc), "r"(new), "r"(old), "0"(err), "1"(replaced)
		: "memory");

	if (!err && (replaced == old))
		flush_icache_range(pc, pc + MCOUNT_INSN_SIZE);

	return err;
}

int ftrace_update_ftrace_func(ftrace_func_t func)
{
	int ret;
	unsigned long pc, old;
	unsigned char *new;

	pc = (unsigned long)&ftrace_call;
	memcpy(&old, &ftrace_call, MCOUNT_INSN_SIZE);
	new = ftrace_call_replace(pc, (unsigned long)func);
	ret = ftrace_modify_code(pc, (unsigned char *)&old, new);
	return ret;
}

/* run from ftrace_init with irqs disabled */
int __init ftrace_dyn_arch_init(void *data)
{
	ftrace_mcount_set(data);
	return 0;
}
