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
 *  linux/arch/arm/mm/proc-arm9tdmi.S: utility functions for ARM9TDMI
 *
 *  Copyright (C) 2003-2006 Hyok S. Choi <hyok.choi@samsung.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */
#include <linux/linkage.h>
#include <linux/init.h>
#include <asm/assembler.h>
#include <asm/asm-offsets.h>
#include <asm/hwcap.h>
#include <asm/pgtable-hwdef.h>
#include <asm/pgtable.h>
#include <asm/ptrace.h>

	.text
/*
 * cpu_arm9tdmi_proc_init()
 * cpu_arm9tdmi_do_idle()
 * cpu_arm9tdmi_dcache_clean_area()
 * cpu_arm9tdmi_switch_mm()
 *
 * These are not required.
 */
ENTRY(cpu_arm9tdmi_proc_init)
ENTRY(cpu_arm9tdmi_do_idle)
ENTRY(cpu_arm9tdmi_dcache_clean_area)
ENTRY(cpu_arm9tdmi_switch_mm)
		mov	pc, lr

/*
 * cpu_arm9tdmi_proc_fin()
 */
ENTRY(cpu_arm9tdmi_proc_fin)
		mov	r0, #PSR_F_BIT | PSR_I_BIT | SVC_MODE
		msr	cpsr_c, r0
		mov	pc, lr

/*
 * Function: cpu_arm9tdmi_reset(loc)
 * Params  : loc(r0)	address to jump to
 * Purpose : Sets up everything for a reset and jump to the location for soft reset.
 */
ENTRY(cpu_arm9tdmi_reset)
		mov	pc, r0

		__INIT

		.type	__arm9tdmi_setup, #function
__arm9tdmi_setup:
		mov	pc, lr
		.size	__arm9tdmi_setup, . - __arm9tdmi_setup

		__INITDATA

/*
 * Purpose : Function pointers used to access above functions - all calls
 *	     come through these
 */
		.type	arm9tdmi_processor_functions, #object
ENTRY(arm9tdmi_processor_functions)
		.word	nommu_early_abort
		.word	pabort_noifar
		.word	cpu_arm9tdmi_proc_init
		.word	cpu_arm9tdmi_proc_fin
		.word	cpu_arm9tdmi_reset
		.word	cpu_arm9tdmi_do_idle
		.word	cpu_arm9tdmi_dcache_clean_area
		.word	cpu_arm9tdmi_switch_mm
		.word	0		@ cpu_*_set_pte
		.size	arm9tdmi_processor_functions, . - arm9tdmi_processor_functions

		.section ".rodata"

		.type	cpu_arch_name, #object
cpu_arch_name:
		.asciz	"armv4t"
		.size	cpu_arch_name, . - cpu_arch_name

		.type	cpu_elf_name, #object
cpu_elf_name:
		.asciz	"v4"
		.size	cpu_elf_name, . - cpu_elf_name

		.type	cpu_arm9tdmi_name, #object
cpu_arm9tdmi_name:
		.asciz	"ARM9TDMI"
		.size	cpu_arm9tdmi_name, . - cpu_arm9tdmi_name

		.type	cpu_p2001_name, #object
cpu_p2001_name:
		.asciz	"P2001"
		.size	cpu_p2001_name, . - cpu_p2001_name

		.align

		.section ".proc.info.init", #alloc, #execinstr

		.type	__arm9tdmi_proc_info, #object
__arm9tdmi_proc_info:
		.long	0x41009900
		.long	0xfff8ff00
		.long	0
		.long	0
		b	__arm9tdmi_setup
		.long	cpu_arch_name
		.long	cpu_elf_name
		.long	HWCAP_SWP | HWCAP_THUMB | HWCAP_26BIT
		.long	cpu_arm9tdmi_name
		.long	arm9tdmi_processor_functions
		.long	0
		.long	0
		.long	v4_cache_fns
		.size	__arm9tdmi_proc_info, . - __arm9dmi_proc_info

		.type	__p2001_proc_info, #object
__p2001_proc_info:
		.long	0x41029000
		.long	0xffffffff
		.long	0
		.long	0
		b	__arm9tdmi_setup
		.long	cpu_arch_name
		.long	cpu_elf_name
		.long	HWCAP_SWP | HWCAP_THUMB | HWCAP_26BIT
		.long	cpu_p2001_name
		.long	arm9tdmi_processor_functions
		.long	0
		.long	0
		.long	v4_cache_fns
		.size	__p2001_proc_info, . - __p2001_proc_info
