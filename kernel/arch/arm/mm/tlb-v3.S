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
 *  linux/arch/arm/mm/tlbv3.S
 *
 *  Copyright (C) 1997-2002 Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  ARM architecture version 3 TLB handling functions.
 *
 * Processors: ARM610, ARM710.
 */
#include <linux/linkage.h>
#include <linux/init.h>
#include <asm/asm-offsets.h>
#include <asm/tlbflush.h>
#include "proc-macros.S"

	.align	5
/*
 *	v3_flush_user_tlb_range(start, end, mm)
 *
 *	Invalidate a range of TLB entries in the specified address space.
 *
 *	- start - range start address
 *	- end   - range end address
 *	- mm    - mm_struct describing address space
 */
	.align	5
ENTRY(v3_flush_user_tlb_range)
	vma_vm_mm r2, r2
	act_mm	r3				@ get current->active_mm
	teq	r2, r3				@ == mm ?
	movne	pc, lr				@ no, we dont do anything
ENTRY(v3_flush_kern_tlb_range)
	bic	r0, r0, #0x0ff
	bic	r0, r0, #0xf00
1:	mcr	p15, 0, r0, c6, c0, 0		@ invalidate TLB entry
	add	r0, r0, #PAGE_SZ
	cmp	r0, r1
	blo	1b
	mov	pc, lr

	__INITDATA

	.type	v3_tlb_fns, #object
ENTRY(v3_tlb_fns)
	.long	v3_flush_user_tlb_range
	.long	v3_flush_kern_tlb_range
	.long	v3_tlb_flags
	.size	v3_tlb_fns, . - v3_tlb_fns
