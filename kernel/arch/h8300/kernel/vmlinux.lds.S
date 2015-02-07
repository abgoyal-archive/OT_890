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

#define VMLINUX_SYMBOL(_sym_) _##_sym_
#include <asm-generic/vmlinux.lds.h>

/* target memory map */
#ifdef CONFIG_H8300H_GENERIC
#define ROMTOP  0x000000
#define ROMSIZE 0x400000
#define RAMTOP  0x400000
#define RAMSIZE 0x400000
#endif

#ifdef CONFIG_H8300H_AKI3068NET
#define ROMTOP  0x000000
#define ROMSIZE 0x080000
#define RAMTOP  0x400000
#define RAMSIZE 0x200000
#endif

#ifdef CONFIG_H8300H_H8MAX
#define ROMTOP  0x000000
#define ROMSIZE 0x080000
#define RAMTOP  0x400000
#define RAMSIZE 0x200000
#endif

#ifdef CONFIG_H8300H_SIM
#define ROMTOP  0x000000
#define ROMSIZE 0x400000
#define RAMTOP  0x400000
#define RAMSIZE 0x400000
#endif

#ifdef CONFIG_H8S_SIM
#define ROMTOP  0x000000
#define ROMSIZE 0x400000
#define RAMTOP  0x400000
#define RAMSIZE 0x800000
#endif

#ifdef CONFIG_H8S_EDOSK2674
#define ROMTOP  0x000000
#define ROMSIZE 0x400000
#define RAMTOP  0x400000
#define RAMSIZE 0x800000
#endif

#if defined(CONFIG_H8300H_SIM) || defined(CONFIG_H8S_SIM)
INPUT(romfs.o)
#endif

_jiffies = _jiffies_64 + 4;

ENTRY(__start)

SECTIONS
{
#if defined(CONFIG_ROMKERNEL)
	. = ROMTOP; 
	.vectors :
	{
	__vector = . ;
		*(.vectors*)
	}
#else
	. = RAMTOP; 
	.bootvec :	
	{
		*(.bootvec)
	}
#endif
        .text :
	{
	_text = .;
#if defined(CONFIG_ROMKERNEL)
	*(.int_redirect)
#endif
	__stext = . ;
	TEXT_TEXT
	SCHED_TEXT
	LOCK_TEXT
	__etext = . ;
	. = ALIGN(16);          /* Exception table              */
	___start___ex_table = .;
		*(__ex_table)
	___stop___ex_table = .;
	}

	RODATA
#if defined(CONFIG_ROMKERNEL)
	SECURITY_INIT
#endif
	ROEND = .; 
#if defined(CONFIG_ROMKERNEL)
	. = RAMTOP;
	.data :	AT(ROEND)
#else
	.data :	
#endif
	{
	__sdata = . ;
	___data_start = . ;

	. = ALIGN(0x2000) ;
		*(.data.init_task)
	. = ALIGN(0x4) ;
		DATA_DATA
	. = ALIGN(0x4) ;
		*(.data.*)	

	. = ALIGN(0x4) ;
	___init_begin = .;
	__sinittext = .; 
		INIT_TEXT
	__einittext = .; 
		INIT_DATA
	. = ALIGN(0x4) ;
	___setup_start = .;
		*(.init.setup)
	. = ALIGN(0x4) ;
	___setup_end = .;
	___initcall_start = .;
		INITCALLS
	___initcall_end = .;
	___con_initcall_start = .;
		*(.con_initcall.init)
	___con_initcall_end = .;
		EXIT_TEXT
		EXIT_DATA
#if defined(CONFIG_BLK_DEV_INITRD)
		. = ALIGN(4);
	___initramfs_start = .;
  		*(.init.ramfs)
  	___initramfs_end = .;
#endif
	. = ALIGN(0x4) ;
	___init_end = .;
	__edata = . ;
	}
#if defined(CONFIG_RAMKERNEL)
	SECURITY_INIT
#endif
	__begin_data = LOADADDR(.data);
        .bss : 
        {
	. = ALIGN(0x4) ;
	__sbss = . ;
		*(.bss*)
	. = ALIGN(0x4) ;
		*(COMMON)
	. = ALIGN(0x4) ;
	__ebss = . ;
	__end = . ;
	__ramstart = .;
	}
	/DISCARD/ : {
		*(.exitcall.exit)
	}
        .romfs :	
	{
		*(.romfs*)
	}
	. = RAMTOP+RAMSIZE;
        .dummy :
        {
	COMMAND_START = . - 0x200 ;
	__ramend = . ;
	}
}
