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
 * File:         arch/blackfin/kernel/vmlinux.lds.S
 * Based on:     none - original work
 * Author:
 *
 * Created:      Tue Sep 21 2004
 * Description:  Master linker script for blackfin architecture
 *
 * Modified:
 *               Copyright 2004-2007 Analog Devices Inc.
 *
 * Bugs:         Enter bugs at http://blackfin.uclinux.org/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see the file COPYING, or write
 * to the Free Software Foundation, Inc.,
 * 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#define VMLINUX_SYMBOL(_sym_) _##_sym_

#include <asm-generic/vmlinux.lds.h>
#include <asm/mem_map.h>
#include <asm/page.h>
#include <asm/thread_info.h>

OUTPUT_FORMAT("elf32-bfin")
ENTRY(__start)
_jiffies = _jiffies_64;

SECTIONS
{
	. = CONFIG_BOOT_LOAD;
	/* Neither the text, ro_data or bss section need to be aligned
	 * So pack them back to back
	 */
	.text :
	{
		__text = .;
		_text = .;
		__stext = .;
		TEXT_TEXT
		SCHED_TEXT
		LOCK_TEXT
		KPROBES_TEXT
		*(.text.*)
		*(.fixup)

#if !L1_CODE_LENGTH
		*(.l1.text)
#endif

		. = ALIGN(16);
		___start___ex_table = .;
		*(__ex_table)
		___stop___ex_table = .;

		__etext = .;
	}

	NOTES

	/* Just in case the first read only is a 32-bit access */
	RO_DATA(4)

	.bss :
	{
		. = ALIGN(4);
		___bss_start = .;
		*(.bss .bss.*)
		*(COMMON)
#if !L1_DATA_A_LENGTH
		*(.l1.bss)
#endif
#if !L1_DATA_B_LENGTH
		*(.l1.bss.B)
#endif
		. = ALIGN(4);
		___bss_stop = .;
	}

	.data :
	{
		__sdata = .;
		/* This gets done first, so the glob doesn't suck it in */
		. = ALIGN(32);
		*(.data.cacheline_aligned)

#if !L1_DATA_A_LENGTH
		. = ALIGN(32);
		*(.data_l1.cacheline_aligned)
		*(.l1.data)
#endif
#if !L1_DATA_B_LENGTH
		*(.l1.data.B)
#endif
#if !L2_LENGTH
		. = ALIGN(32);
		*(.data_l2.cacheline_aligned)
		*(.l2.data)
#endif

		DATA_DATA
		CONSTRUCTORS

		/* make sure the init_task is aligned to the
		 * kernel thread size so we can locate the kernel
		 * stack properly and quickly.
		 */
		. = ALIGN(THREAD_SIZE);
		*(.init_task.data)

		__edata = .;
	}

	/* The init section should be last, so when we free it, it goes into
	 * the general memory pool, and (hopefully) will decrease fragmentation
	 * a tiny bit. The init section has a _requirement_ that it be
	 * PAGE_SIZE aligned
	 */
	. = ALIGN(PAGE_SIZE);
	___init_begin = .;

	.init.text :
	{
		. = ALIGN(PAGE_SIZE);
		__sinittext = .;
		INIT_TEXT
		__einittext = .;
	}
	.init.data :
	{
		. = ALIGN(16);
		INIT_DATA
	}
	.init.setup :
	{
		. = ALIGN(16);
		___setup_start = .;
		*(.init.setup)
		___setup_end = .;
	}
	.initcall.init :
	{
		___initcall_start = .;
		INITCALLS
		___initcall_end = .;
	}
	.con_initcall.init :
	{
		___con_initcall_start = .;
		*(.con_initcall.init)
		___con_initcall_end = .;
	}
	PERCPU(4)
	SECURITY_INIT
	.init.ramfs :
	{
		. = ALIGN(4);
		___initramfs_start = .;
		*(.init.ramfs)
		. = ALIGN(4);
		___initramfs_end = .;
	}

	__l1_lma_start = .;

	.text_l1 L1_CODE_START : AT(LOADADDR(.init.ramfs) + SIZEOF(.init.ramfs))
	{
		. = ALIGN(4);
		__stext_l1 = .;
		*(.l1.text)
		. = ALIGN(4);
		__etext_l1 = .;
	}

	.data_l1 L1_DATA_A_START : AT(LOADADDR(.text_l1) + SIZEOF(.text_l1))
	{
		. = ALIGN(4);
		__sdata_l1 = .;
		*(.l1.data)
		__edata_l1 = .;

		. = ALIGN(32);
		*(.data_l1.cacheline_aligned)

		. = ALIGN(4);
		__sbss_l1 = .;
		*(.l1.bss)
		. = ALIGN(4);
		__ebss_l1 = .;
	}

	.data_b_l1 L1_DATA_B_START : AT(LOADADDR(.data_l1) + SIZEOF(.data_l1))
	{
		. = ALIGN(4);
		__sdata_b_l1 = .;
		*(.l1.data.B)
		__edata_b_l1 = .;

		. = ALIGN(4);
		__sbss_b_l1 = .;
		*(.l1.bss.B)
		. = ALIGN(4);
		__ebss_b_l1 = .;
	}

	__l2_lma_start = LOADADDR(.data_b_l1) + SIZEOF(.data_b_l1);

	.text_data_l2 L2_START : AT(LOADADDR(.data_b_l1) + SIZEOF(.data_b_l1))
	{
		. = ALIGN(4);
		__stext_l2 = .;
		*(.l2.text)
		. = ALIGN(4);
		__etext_l2 = .;

		. = ALIGN(4);
		__sdata_l2 = .;
		*(.l2.data)
		__edata_l2 = .;

		. = ALIGN(32);
		*(.data_l2.cacheline_aligned)

		. = ALIGN(4);
		__sbss_l2 = .;
		*(.l2.bss)
		. = ALIGN(4);
		__ebss_l2 = .;
	}

	/* Force trailing alignment of our init section so that when we
	 * free our init memory, we don't leave behind a partial page.
	 */
	. = LOADADDR(.text_data_l2) + SIZEOF(.text_data_l2);
	. = ALIGN(PAGE_SIZE);
	___init_end = .;

	__end =.;

	STABS_DEBUG

	DWARF_DEBUG

	/DISCARD/ :
	{
		EXIT_TEXT
		EXIT_DATA
		*(.exitcall.exit)
	}
}
