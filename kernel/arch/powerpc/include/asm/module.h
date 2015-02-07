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

#ifndef _ASM_POWERPC_MODULE_H
#define _ASM_POWERPC_MODULE_H
#ifdef __KERNEL__

/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#include <linux/list.h>
#include <asm/bug.h>


#ifndef __powerpc64__
/*
 * Thanks to Paul M for explaining this.
 *
 * PPC can only do rel jumps += 32MB, and often the kernel and other
 * modules are furthur away than this.  So, we jump to a table of
 * trampolines attached to the module (the Procedure Linkage Table)
 * whenever that happens.
 */

struct ppc_plt_entry {
	/* 16 byte jump instruction sequence (4 instructions) */
	unsigned int jump[4];
};
#endif	/* __powerpc64__ */


struct mod_arch_specific {
#ifdef __powerpc64__
	unsigned int stubs_section;	/* Index of stubs section in module */
	unsigned int toc_section;	/* What section is the TOC? */
#ifdef CONFIG_DYNAMIC_FTRACE
	unsigned long toc;
	unsigned long tramp;
#endif

#else /* powerpc64 */
	/* Indices of PLT sections within module. */
	unsigned int core_plt_section;
	unsigned int init_plt_section;
#ifdef CONFIG_DYNAMIC_FTRACE
	unsigned long tramp;
#endif
#endif /* powerpc64 */

	/* List of BUG addresses, source line numbers and filenames */
	struct list_head bug_list;
	struct bug_entry *bug_table;
	unsigned int num_bugs;
};

/*
 * Select ELF headers.
 * Make empty section for module_frob_arch_sections to expand.
 */

#ifdef __powerpc64__
#    define Elf_Shdr	Elf64_Shdr
#    define Elf_Sym	Elf64_Sym
#    define Elf_Ehdr	Elf64_Ehdr
#    ifdef MODULE
	asm(".section .stubs,\"ax\",@nobits; .align 3; .previous");
#    endif
#else
#    define Elf_Shdr	Elf32_Shdr
#    define Elf_Sym	Elf32_Sym
#    define Elf_Ehdr	Elf32_Ehdr
#    ifdef MODULE
	asm(".section .plt,\"ax\",@nobits; .align 3; .previous");
	asm(".section .init.plt,\"ax\",@nobits; .align 3; .previous");
#    endif	/* MODULE */
#endif

#ifdef CONFIG_DYNAMIC_FTRACE
#    ifdef MODULE
	asm(".section .ftrace.tramp,\"ax\",@nobits; .align 3; .previous");
#    endif	/* MODULE */
#endif


struct exception_table_entry;
void sort_ex_table(struct exception_table_entry *start,
		   struct exception_table_entry *finish);

#endif /* __KERNEL__ */
#endif	/* _ASM_POWERPC_MODULE_H */
