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

/* tlbflush.h: TLB flushing functions
 *
 * Copyright (C) 2004 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#ifndef _ASM_TLBFLUSH_H
#define _ASM_TLBFLUSH_H

#include <linux/mm.h>
#include <asm/processor.h>

#ifdef CONFIG_MMU

#ifndef __ASSEMBLY__
extern void asmlinkage __flush_tlb_all(void);
extern void asmlinkage __flush_tlb_mm(unsigned long contextid);
extern void asmlinkage __flush_tlb_page(unsigned long contextid, unsigned long start);
extern void asmlinkage __flush_tlb_range(unsigned long contextid,
					 unsigned long start, unsigned long end);
#endif /* !__ASSEMBLY__ */

#define flush_tlb_all()				\
do {						\
	preempt_disable();			\
	__flush_tlb_all();			\
	preempt_enable();			\
} while(0)

#define flush_tlb_mm(mm)			\
do {						\
	preempt_disable();			\
	__flush_tlb_mm((mm)->context.id);	\
	preempt_enable();			\
} while(0)

#define flush_tlb_range(vma,start,end)					\
do {									\
	preempt_disable();						\
	__flush_tlb_range((vma)->vm_mm->context.id, start, end);	\
	preempt_enable();						\
} while(0)

#define flush_tlb_page(vma,addr)				\
do {								\
	preempt_disable();					\
	__flush_tlb_page((vma)->vm_mm->context.id, addr);	\
	preempt_enable();					\
} while(0)


#define __flush_tlb_global()			flush_tlb_all()
#define flush_tlb()				flush_tlb_all()
#define flush_tlb_kernel_range(start, end)	flush_tlb_all()

#else

#define flush_tlb()				BUG()
#define flush_tlb_all()				BUG()
#define flush_tlb_mm(mm)			BUG()
#define flush_tlb_page(vma,addr)		BUG()
#define flush_tlb_range(mm,start,end)		BUG()
#define flush_tlb_kernel_range(start, end)	BUG()

#endif


#endif /* _ASM_TLBFLUSH_H */
