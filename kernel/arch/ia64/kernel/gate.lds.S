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
 * Linker script for gate DSO.  The gate pages are an ELF shared object
 * prelinked to its virtual address, with only one read-only segment and
 * one execute-only segment (both fit in one page).  This script controls
 * its layout.
 */


#include <asm/system.h>

SECTIONS
{
	. = GATE_ADDR + SIZEOF_HEADERS;

	.hash			: { *(.hash) }		:readable
	.gnu.hash		: { *(.gnu.hash) }
	.dynsym			: { *(.dynsym) }
	.dynstr			: { *(.dynstr) }
	.gnu.version		: { *(.gnu.version) }
	.gnu.version_d		: { *(.gnu.version_d) }
	.gnu.version_r		: { *(.gnu.version_r) }

	.note			: { *(.note*) }		:readable	:note

	.dynamic		: { *(.dynamic) }	:readable	:dynamic

	/*
	 * This linker script is used both with -r and with -shared.  For
	 * the layouts to match, we need to skip more than enough space for
	 * the dynamic symbol table et al.  If this amount is insufficient,
	 * ld -shared will barf.  Just increase it here.
	 */
	. = GATE_ADDR + 0x600;

	.data.patch		: {
		__start_gate_mckinley_e9_patchlist = .;
		*(.data.patch.mckinley_e9)
		__end_gate_mckinley_e9_patchlist = .;

		__start_gate_vtop_patchlist = .;
		*(.data.patch.vtop)
		__end_gate_vtop_patchlist = .;

		__start_gate_fsyscall_patchlist = .;
		*(.data.patch.fsyscall_table)
		__end_gate_fsyscall_patchlist = .;

		__start_gate_brl_fsys_bubble_down_patchlist = .;
		*(.data.patch.brl_fsys_bubble_down)
		__end_gate_brl_fsys_bubble_down_patchlist = .;
	}						:readable

	.IA_64.unwind_info	: { *(.IA_64.unwind_info*) }
	.IA_64.unwind		: { *(.IA_64.unwind*) }	:readable	:unwind
#ifdef HAVE_BUGGY_SEGREL
	.text (GATE_ADDR + PAGE_SIZE) : { *(.text) *(.text.*) }	:readable
#else
	. = ALIGN(PERCPU_PAGE_SIZE) + (. & (PERCPU_PAGE_SIZE - 1));
	.text			: { *(.text) *(.text.*) }	:epc
#endif

	/DISCARD/		: {
		*(.got.plt) *(.got)
		*(.data .data.* .gnu.linkonce.d.*)
		*(.dynbss)
		*(.bss .bss.* .gnu.linkonce.b.*)
		*(__ex_table)
		*(__mca_table)
	}
}

/*
 * ld does not recognize this name token; use the constant.
 */
#define	PT_IA_64_UNWIND	0x70000001

/*
 * We must supply the ELF program headers explicitly to get just one
 * PT_LOAD segment, and set the flags explicitly to make segments read-only.
 */
PHDRS
{
	readable	PT_LOAD	FILEHDR	PHDRS	FLAGS(4);	/* PF_R */
#ifndef HAVE_BUGGY_SEGREL
	epc		PT_LOAD	FILEHDR PHDRS	FLAGS(1);	/* PF_X */
#endif
	dynamic		PT_DYNAMIC		FLAGS(4);	/* PF_R */
	note		PT_NOTE			FLAGS(4);	/* PF_R */
	unwind		PT_IA_64_UNWIND;
}

/*
 * This controls what symbols we export from the DSO.
 */
VERSION
{
	LINUX_2.5 {
	global:
		__kernel_syscall_via_break;
		__kernel_syscall_via_epc;
		__kernel_sigtramp;

	local: *;
	};
}

/* The ELF entry point can be used to set the AT_SYSINFO value.  */
ENTRY(__kernel_syscall_via_epc)
