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

/* MN10300 TLB flushing functions
 *
 * Copyright (C) 2007 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version
 * 2 of the Licence, or (at your option) any later version.
 */
#ifndef _ASM_TLBFLUSH_H
#define _ASM_TLBFLUSH_H

#include <asm/processor.h>

#define __flush_tlb()						\
do {								\
	int w;							\
	__asm__ __volatile__					\
		("	mov %1,%0		\n"		\
		 "	or %2,%0		\n"		\
		 "	mov %0,%1		\n"		\
		 : "=d"(w)					\
		 : "m"(MMUCTR), "i"(MMUCTR_IIV|MMUCTR_DIV)	\
		 : "memory"					\
		 );						\
} while (0)

#define __flush_tlb_all() __flush_tlb()
#define __flush_tlb_one(addr) __flush_tlb()


/*
 * TLB flushing:
 *
 *  - flush_tlb() flushes the current mm struct TLBs
 *  - flush_tlb_all() flushes all processes TLBs
 *  - flush_tlb_mm(mm) flushes the specified mm context TLB's
 *  - flush_tlb_page(vma, vmaddr) flushes one page
 *  - flush_tlb_range(mm, start, end) flushes a range of pages
 *  - flush_tlb_pgtables(mm, start, end) flushes a range of page tables
 */
#define flush_tlb_all()				\
do {						\
	preempt_disable();			\
	__flush_tlb_all();			\
	preempt_enable();			\
} while (0)

#define flush_tlb_mm(mm)			\
do {						\
	preempt_disable();			\
	__flush_tlb_all();			\
	preempt_enable();			\
} while (0)

#define flush_tlb_range(vma, start, end)			\
do {								\
	unsigned long __s __attribute__((unused)) = (start);	\
	unsigned long __e __attribute__((unused)) = (end);	\
	preempt_disable();					\
	__flush_tlb_all();					\
	preempt_enable();					\
} while (0)


#define __flush_tlb_global()			flush_tlb_all()
#define flush_tlb()				flush_tlb_all()
#define flush_tlb_kernel_range(start, end)			\
do {								\
	unsigned long __s __attribute__((unused)) = (start);	\
	unsigned long __e __attribute__((unused)) = (end);	\
	flush_tlb_all();					\
} while (0)

extern void flush_tlb_page(struct vm_area_struct *vma, unsigned long addr);

#define flush_tlb_pgtables(mm, start, end)	do {} while (0)

#endif /* _ASM_TLBFLUSH_H */
