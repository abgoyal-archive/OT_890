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
 *  linux/arch/cris/mm/fault.c
 *
 *  Low level bus fault handler
 *
 *
 *  Copyright (C) 2000-2007  Axis Communications AB
 *
 *  Authors:  Bjorn Wesen
 *
 */

#include <linux/mm.h>
#include <asm/uaccess.h>
#include <asm/pgtable.h>
#include <arch/svinto.h>
#include <asm/mmu_context.h>

/* debug of low-level TLB reload */
#undef DEBUG

#ifdef DEBUG
#define D(x) x
#else
#define D(x)
#endif

extern const struct exception_table_entry
	*search_exception_tables(unsigned long addr);

asmlinkage void do_page_fault(unsigned long address, struct pt_regs *regs,
                              int protection, int writeaccess);

/* fast TLB-fill fault handler
 * this is called from entry.S with interrupts disabled
 */

void
handle_mmu_bus_fault(struct pt_regs *regs)
{
	int cause;
	int select;
#ifdef DEBUG
	int index;
	int page_id;
	int acc, inv;
#endif
	pgd_t* pgd = (pgd_t*)per_cpu(current_pgd, smp_processor_id());
	pmd_t *pmd;
	pte_t pte;
	int miss, we, writeac;
	unsigned long address;
	unsigned long flags;

	cause = *R_MMU_CAUSE;

	address = cause & PAGE_MASK; /* get faulting address */
	select = *R_TLB_SELECT;

#ifdef DEBUG
	page_id = IO_EXTRACT(R_MMU_CAUSE,  page_id,   cause);
	acc     = IO_EXTRACT(R_MMU_CAUSE,  acc_excp,  cause);
	inv     = IO_EXTRACT(R_MMU_CAUSE,  inv_excp,  cause);
	index   = IO_EXTRACT(R_TLB_SELECT, index,     select);
#endif
	miss    = IO_EXTRACT(R_MMU_CAUSE,  miss_excp, cause);
	we      = IO_EXTRACT(R_MMU_CAUSE,  we_excp,   cause);
	writeac = IO_EXTRACT(R_MMU_CAUSE,  wr_rd,     cause);

	D(printk("bus_fault from IRP 0x%lx: addr 0x%lx, miss %d, inv %d, we %d, acc %d, dx %d pid %d\n",
		 regs->irp, address, miss, inv, we, acc, index, page_id));

	/* leave it to the MM system fault handler */
	if (miss)
		do_page_fault(address, regs, 0, writeac);
        else
		do_page_fault(address, regs, 1, we);

        /* Reload TLB with new entry to avoid an extra miss exception.
	 * do_page_fault may have flushed the TLB so we have to restore
	 * the MMU registers.
	 */
	local_save_flags(flags);
	local_irq_disable();
	pmd = (pmd_t *)(pgd + pgd_index(address));
	if (pmd_none(*pmd))
		goto exit;
	pte = *pte_offset_kernel(pmd, address);
	if (!pte_present(pte))
		goto exit;
	*R_TLB_SELECT = select;
	*R_TLB_HI = cause;
	*R_TLB_LO = pte_val(pte);
exit:
	local_irq_restore(flags);
}
