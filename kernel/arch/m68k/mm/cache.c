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
 *  linux/arch/m68k/mm/cache.c
 *
 *  Instruction cache handling
 *
 *  Copyright (C) 1995  Hamish Macdonald
 */

#include <linux/module.h>
#include <asm/pgalloc.h>
#include <asm/traps.h>


static unsigned long virt_to_phys_slow(unsigned long vaddr)
{
	if (CPU_IS_060) {
		unsigned long paddr;

		/* The PLPAR instruction causes an access error if the translation
		 * is not possible. To catch this we use the same exception mechanism
		 * as for user space accesses in <asm/uaccess.h>. */
		asm volatile (".chip 68060\n"
			      "1: plpar (%0)\n"
			      ".chip 68k\n"
			      "2:\n"
			      ".section .fixup,\"ax\"\n"
			      "   .even\n"
			      "3: sub.l %0,%0\n"
			      "   jra 2b\n"
			      ".previous\n"
			      ".section __ex_table,\"a\"\n"
			      "   .align 4\n"
			      "   .long 1b,3b\n"
			      ".previous"
			      : "=a" (paddr)
			      : "0" (vaddr));
		return paddr;
	} else if (CPU_IS_040) {
		unsigned long mmusr;

		asm volatile (".chip 68040\n\t"
			      "ptestr (%1)\n\t"
			      "movec %%mmusr, %0\n\t"
			      ".chip 68k"
			      : "=r" (mmusr)
			      : "a" (vaddr));

		if (mmusr & MMU_R_040)
			return (mmusr & PAGE_MASK) | (vaddr & ~PAGE_MASK);
	} else {
		unsigned short mmusr;
		unsigned long *descaddr;

		asm volatile ("ptestr %3,%2@,#7,%0\n\t"
			      "pmove %%psr,%1@"
			      : "=a&" (descaddr)
			      : "a" (&mmusr), "a" (vaddr), "d" (get_fs().seg));
		if (mmusr & (MMU_I|MMU_B|MMU_L))
			return 0;
		descaddr = phys_to_virt((unsigned long)descaddr);
		switch (mmusr & MMU_NUM) {
		case 1:
			return (*descaddr & 0xfe000000) | (vaddr & 0x01ffffff);
		case 2:
			return (*descaddr & 0xfffc0000) | (vaddr & 0x0003ffff);
		case 3:
			return (*descaddr & PAGE_MASK) | (vaddr & ~PAGE_MASK);
		}
	}
	return 0;
}

/* Push n pages at kernel virtual address and clear the icache */
/* RZ: use cpush %bc instead of cpush %dc, cinv %ic */
void flush_icache_range(unsigned long address, unsigned long endaddr)
{

	if (CPU_IS_040_OR_060) {
		address &= PAGE_MASK;

		do {
			asm volatile ("nop\n\t"
				      ".chip 68040\n\t"
				      "cpushp %%bc,(%0)\n\t"
				      ".chip 68k"
				      : : "a" (virt_to_phys_slow(address)));
			address += PAGE_SIZE;
		} while (address < endaddr);
	} else {
		unsigned long tmp;
		asm volatile ("movec %%cacr,%0\n\t"
			      "orw %1,%0\n\t"
			      "movec %0,%%cacr"
			      : "=&d" (tmp)
			      : "di" (FLUSH_I));
	}
}
EXPORT_SYMBOL(flush_icache_range);

void flush_icache_user_range(struct vm_area_struct *vma, struct page *page,
			     unsigned long addr, int len)
{
	if (CPU_IS_040_OR_060) {
		asm volatile ("nop\n\t"
			      ".chip 68040\n\t"
			      "cpushp %%bc,(%0)\n\t"
			      ".chip 68k"
			      : : "a" (page_to_phys(page)));
	} else {
		unsigned long tmp;
		asm volatile ("movec %%cacr,%0\n\t"
			      "orw %1,%0\n\t"
			      "movec %0,%%cacr"
			      : "=&d" (tmp)
			      : "di" (FLUSH_I));
	}
}

