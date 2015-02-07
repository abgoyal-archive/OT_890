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
 * Copyright © 2008 Ingo Molnar
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
 */

#include <asm/iomap.h>
#include <asm/pat.h>
#include <linux/module.h>

int is_io_mapping_possible(resource_size_t base, unsigned long size)
{
#ifndef CONFIG_X86_PAE
	/* There is no way to map greater than 1 << 32 address without PAE */
	if (base + size > 0x100000000ULL)
		return 0;
#endif
	return 1;
}
EXPORT_SYMBOL_GPL(is_io_mapping_possible);

/* Map 'pfn' using fixed map 'type' and protections 'prot'
 */
void *
iomap_atomic_prot_pfn(unsigned long pfn, enum km_type type, pgprot_t prot)
{
	enum fixed_addresses idx;
	unsigned long vaddr;

	pagefault_disable();

	/*
	 * For non-PAT systems, promote PAGE_KERNEL_WC to PAGE_KERNEL_UC_MINUS.
	 * PAGE_KERNEL_WC maps to PWT, which translates to uncached if the
	 * MTRR is UC or WC.  UC_MINUS gets the real intention, of the
	 * user, which is "WC if the MTRR is WC, UC if you can't do that."
	 */
	if (!pat_enabled && pgprot_val(prot) == pgprot_val(PAGE_KERNEL_WC))
		prot = PAGE_KERNEL_UC_MINUS;

	idx = type + KM_TYPE_NR*smp_processor_id();
	vaddr = __fix_to_virt(FIX_KMAP_BEGIN + idx);
	set_pte(kmap_pte-idx, pfn_pte(pfn, prot));
	arch_flush_lazy_mmu_mode();

	return (void*) vaddr;
}
EXPORT_SYMBOL_GPL(iomap_atomic_prot_pfn);

void
iounmap_atomic(void *kvaddr, enum km_type type)
{
	unsigned long vaddr = (unsigned long) kvaddr & PAGE_MASK;
	enum fixed_addresses idx = type + KM_TYPE_NR*smp_processor_id();

	/*
	 * Force other mappings to Oops if they'll try to access this pte
	 * without first remap it.  Keeping stale mappings around is a bad idea
	 * also, in case the page changes cacheability attributes or becomes
	 * a protected page in a hypervisor.
	 */
	if (vaddr == __fix_to_virt(FIX_KMAP_BEGIN+idx))
		kpte_clear_flush(kmap_pte-idx, vaddr);

	arch_flush_lazy_mmu_mode();
	pagefault_enable();
}
EXPORT_SYMBOL_GPL(iounmap_atomic);
