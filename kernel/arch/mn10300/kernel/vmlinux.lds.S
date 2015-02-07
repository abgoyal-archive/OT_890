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

/* MN10300 Main kernel linker script
 *
 * Copyright (C) 2007 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version
 * 2 of the Licence, or (at your option) any later version.
 */
#define __VMLINUX_LDS__
#include <asm-generic/vmlinux.lds.h>
#include <asm/thread_info.h>
#include <asm/page.h>

OUTPUT_FORMAT("elf32-am33lin", "elf32-am33lin", "elf32-am33lin")
OUTPUT_ARCH(mn10300)
ENTRY(_start)
jiffies = jiffies_64;
#ifndef CONFIG_MN10300_CURRENT_IN_E2
current = __current;
#endif
SECTIONS
{
  . = CONFIG_KERNEL_TEXT_ADDRESS;
  /* read-only */
  _stext = .;
  _text = .;			/* Text and read-only data */
  .text : {
	*(
	.text.head
	.text
	)
	TEXT_TEXT
	SCHED_TEXT
	LOCK_TEXT
	KPROBES_TEXT
	*(.fixup)
	*(.gnu.warning)
	} = 0xcb

  _etext = .;			/* End of text section */

  . = ALIGN(16);		/* Exception table */
  __start___ex_table = .;
  __ex_table : { *(__ex_table) }
  __stop___ex_table = .;

  BUG_TABLE

  RODATA

  /* writeable */
  .data : {			/* Data */
	DATA_DATA
	CONSTRUCTORS
	}

  . = ALIGN(PAGE_SIZE);
  __nosave_begin = .;
  .data_nosave : { *(.data.nosave) }
  . = ALIGN(PAGE_SIZE);
  __nosave_end = .;

  . = ALIGN(PAGE_SIZE);
  .data.page_aligned : { *(.data.idt) }

  . = ALIGN(32);
  .data.cacheline_aligned : { *(.data.cacheline_aligned) }

  /* rarely changed data like cpu maps */
  . = ALIGN(32);
  .data.read_mostly : AT(ADDR(.data.read_mostly)) {
	*(.data.read_mostly)
	_edata = .;		/* End of data section */
  }

  . = ALIGN(THREAD_SIZE);	/* init_task */
  .data.init_task : { *(.data.init_task) }

  /* might get freed after init */
  . = ALIGN(PAGE_SIZE);
  .smp_locks : AT(ADDR(.smp_locks) - LOAD_OFFSET) {
  	__smp_locks = .;
	*(.smp_locks)
	__smp_locks_end = .;
  }

  /* will be freed after init */
  . = ALIGN(PAGE_SIZE);		/* Init code and data */
  __init_begin = .;
  .init.text : {
	_sinittext = .;
	*(.init.text)
	_einittext = .;
  }
  .init.data : { *(.init.data) }
  . = ALIGN(16);
  __setup_start = .;
  .setup.init : { KEEP(*(.init.setup)) }
  __setup_end = .;

  __initcall_start = .;
  .initcall.init : {
	INITCALLS
  }
  __initcall_end = .;
  __con_initcall_start = .;
  .con_initcall.init : { *(.con_initcall.init) }
  __con_initcall_end = .;

  SECURITY_INIT
  . = ALIGN(4);
  __alt_instructions = .;
  .altinstructions : { *(.altinstructions) }
  __alt_instructions_end = .;
 .altinstr_replacement : { *(.altinstr_replacement) }
  /* .exit.text is discard at runtime, not link time, to deal with references
     from .altinstructions and .eh_frame */
  .exit.text : { *(.exit.text) }
  .exit.data : { *(.exit.data) }

#ifdef CONFIG_BLK_DEV_INITRD
  . = ALIGN(PAGE_SIZE);
  __initramfs_start = .;
  .init.ramfs : { *(.init.ramfs) }
  __initramfs_end = .;
#endif

  PERCPU(32)
  . = ALIGN(PAGE_SIZE);
  __init_end = .;
  /* freed after init ends here */

  __bss_start = .;		/* BSS */
  .bss : {
	*(.bss.page_aligned)
	*(.bss)
  }
  . = ALIGN(4);
  __bss_stop = .;

  _end = . ;

  /* This is where the kernel creates the early boot page tables */
  . = ALIGN(PAGE_SIZE);
  pg0 = .;

  /* Sections to be discarded */
  /DISCARD/ : {
	*(.exitcall.exit)
	}

  STABS_DEBUG

  DWARF_DEBUG
}
