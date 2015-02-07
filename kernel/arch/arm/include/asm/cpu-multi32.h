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
 *  arch/arm/include/asm/cpu-multi32.h
 *
 *  Copyright (C) 2000 Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <asm/page.h>

struct mm_struct;

/*
 * Don't change this structure - ASM code
 * relies on it.
 */
extern struct processor {
	/* MISC
	 * get data abort address/flags
	 */
	void (*_data_abort)(unsigned long pc);
	/*
	 * Retrieve prefetch fault address
	 */
	unsigned long (*_prefetch_abort)(unsigned long lr);
	/*
	 * Set up any processor specifics
	 */
	void (*_proc_init)(void);
	/*
	 * Disable any processor specifics
	 */
	void (*_proc_fin)(void);
	/*
	 * Special stuff for a reset
	 */
	void (*reset)(unsigned long addr) __attribute__((noreturn));
	/*
	 * Idle the processor
	 */
	int (*_do_idle)(void);
	/*
	 * Processor architecture specific
	 */
	/*
	 * clean a virtual address range from the
	 * D-cache without flushing the cache.
	 */
	void (*dcache_clean_area)(void *addr, int size);

	/*
	 * Set the page table
	 */
	void (*switch_mm)(unsigned long pgd_phys, struct mm_struct *mm);
	/*
	 * Set a possibly extended PTE.  Non-extended PTEs should
	 * ignore 'ext'.
	 */
	void (*set_pte_ext)(pte_t *ptep, pte_t pte, unsigned int ext);
} processor;

#define cpu_proc_init()			processor._proc_init()
#define cpu_proc_fin()			processor._proc_fin()
#define cpu_reset(addr)			processor.reset(addr)
#define cpu_do_idle()			processor._do_idle()
#define cpu_dcache_clean_area(addr,sz)	processor.dcache_clean_area(addr,sz)
#define cpu_set_pte_ext(ptep,pte,ext)	processor.set_pte_ext(ptep,pte,ext)
#define cpu_do_switch_mm(pgd,mm)	processor.switch_mm(pgd,mm)
