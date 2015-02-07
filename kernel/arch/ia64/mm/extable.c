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
 * Kernel exception handling table support.  Derived from arch/alpha/mm/extable.c.
 *
 * Copyright (C) 1998, 1999, 2001-2002, 2004 Hewlett-Packard Co
 *	David Mosberger-Tang <davidm@hpl.hp.com>
 */

#include <linux/sort.h>

#include <asm/uaccess.h>
#include <asm/module.h>

static int cmp_ex(const void *a, const void *b)
{
	const struct exception_table_entry *l = a, *r = b;
	u64 lip = (u64) &l->addr + l->addr;
	u64 rip = (u64) &r->addr + r->addr;

	/* avoid overflow */
	if (lip > rip)
		return 1;
	if (lip < rip)
		return -1;
	return 0;
}

static void swap_ex(void *a, void *b, int size)
{
	struct exception_table_entry *l = a, *r = b, tmp;
	u64 delta = (u64) r - (u64) l;

	tmp = *l;
	l->addr = r->addr + delta;
	l->cont = r->cont + delta;
	r->addr = tmp.addr - delta;
	r->cont = tmp.cont - delta;
}

/*
 * Sort the exception table. It's usually already sorted, but there
 * may be unordered entries due to multiple text sections (such as the
 * .init text section). Note that the exception-table-entries contain
 * location-relative addresses, which requires a bit of care during
 * sorting to avoid overflows in the offset members (e.g., it would
 * not be safe to make a temporary copy of an exception-table entry on
 * the stack, because the stack may be more than 2GB away from the
 * exception-table).
 */
void sort_extable (struct exception_table_entry *start,
		   struct exception_table_entry *finish)
{
	sort(start, finish - start, sizeof(struct exception_table_entry),
	     cmp_ex, swap_ex);
}

const struct exception_table_entry *
search_extable (const struct exception_table_entry *first,
		const struct exception_table_entry *last,
		unsigned long ip)
{
	const struct exception_table_entry *mid;
	unsigned long mid_ip;
	long diff;

        while (first <= last) {
		mid = &first[(last - first)/2];
		mid_ip = (u64) &mid->addr + mid->addr;
		diff = mid_ip - ip;
                if (diff == 0)
                        return mid;
                else if (diff < 0)
                        first = mid + 1;
                else
                        last = mid - 1;
        }
        return NULL;
}

void
ia64_handle_exception (struct pt_regs *regs, const struct exception_table_entry *e)
{
	long fix = (u64) &e->cont + e->cont;

	regs->r8 = -EFAULT;
	if (fix & 4)
		regs->r9 = 0;
	regs->cr_iip = fix & ~0xf;
	ia64_psr(regs)->ri = fix & 0x3;		/* set continuation slot number */
}
