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

/* MN10300 MMU context allocation and management
 *
 * Copyright (C) 2007 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version
 * 2 of the Licence, or (at your option) any later version.
 */
#include <linux/sched.h>
#include <linux/mm.h>
#include <asm/mmu_context.h>
#include <asm/tlbflush.h>

/*
 * list of the MMU contexts last allocated on each CPU
 */
unsigned long mmu_context_cache[NR_CPUS] = {
	[0 ... NR_CPUS - 1] = MMU_CONTEXT_FIRST_VERSION * 2 - 1,
};

/*
 * flush the specified TLB entry
 */
void flush_tlb_page(struct vm_area_struct *vma, unsigned long addr)
{
	unsigned long pteu, cnx, flags;

	addr &= PAGE_MASK;

	/* make sure the context doesn't migrate and defend against
	 * interference from vmalloc'd regions */
	local_irq_save(flags);

	cnx = mm_context(vma->vm_mm);

	if (cnx != MMU_NO_CONTEXT) {
		pteu = addr | (cnx & 0x000000ffUL);
		IPTEU = pteu;
		DPTEU = pteu;
		if (IPTEL & xPTEL_V)
			IPTEL = 0;
		if (DPTEL & xPTEL_V)
			DPTEL = 0;
	}

	local_irq_restore(flags);
}

/*
 * preemptively set a TLB entry
 */
void update_mmu_cache(struct vm_area_struct *vma, unsigned long addr, pte_t pte)
{
	unsigned long pteu, ptel, cnx, flags;

	addr &= PAGE_MASK;
	ptel = pte_val(pte) & ~(xPTEL_UNUSED1 | xPTEL_UNUSED2);

	/* make sure the context doesn't migrate and defend against
	 * interference from vmalloc'd regions */
	local_irq_save(flags);

	cnx = mm_context(vma->vm_mm);

	if (cnx != MMU_NO_CONTEXT) {
		pteu = addr | (cnx & 0x000000ffUL);
		if (!(pte_val(pte) & _PAGE_NX)) {
			IPTEU = pteu;
			if (IPTEL & xPTEL_V)
				IPTEL = ptel;
		}
		DPTEU = pteu;
		if (DPTEL & xPTEL_V)
			DPTEL = ptel;
	}

	local_irq_restore(flags);
}
