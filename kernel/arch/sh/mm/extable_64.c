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
 * arch/sh/mm/extable_64.c
 *
 * Copyright (C) 2003 Richard Curnow
 * Copyright (C) 2003, 2004  Paul Mundt
 *
 * Cloned from the 2.5 SH version..
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */
#include <linux/rwsem.h>
#include <linux/module.h>
#include <asm/uaccess.h>

extern unsigned long copy_user_memcpy, copy_user_memcpy_end;
extern void __copy_user_fixup(void);

static const struct exception_table_entry __copy_user_fixup_ex = {
	.fixup = (unsigned long)&__copy_user_fixup,
};

/*
 * Some functions that may trap due to a bad user-mode address have too
 * many loads and stores in them to make it at all practical to label
 * each one and put them all in the main exception table.
 *
 * In particular, the fast memcpy routine is like this.  It's fix-up is
 * just to fall back to a slow byte-at-a-time copy, which is handled the
 * conventional way.  So it's functionally OK to just handle any trap
 * occurring in the fast memcpy with that fixup.
 */
static const struct exception_table_entry *check_exception_ranges(unsigned long addr)
{
	if ((addr >= (unsigned long)&copy_user_memcpy) &&
	    (addr <= (unsigned long)&copy_user_memcpy_end))
		return &__copy_user_fixup_ex;

	return NULL;
}

/* Simple binary search */
const struct exception_table_entry *
search_extable(const struct exception_table_entry *first,
		 const struct exception_table_entry *last,
		 unsigned long value)
{
	const struct exception_table_entry *mid;

	mid = check_exception_ranges(value);
	if (mid)
		return mid;

        while (first <= last) {
		long diff;

		mid = (last - first) / 2 + first;
		diff = mid->insn - value;
                if (diff == 0)
                        return mid;
                else if (diff < 0)
                        first = mid+1;
                else
                        last = mid-1;
        }

        return NULL;
}

int fixup_exception(struct pt_regs *regs)
{
	const struct exception_table_entry *fixup;

	fixup = search_exception_tables(regs->pc);
	if (fixup) {
		regs->pc = fixup->fixup;
		return 1;
	}

	return 0;
}
