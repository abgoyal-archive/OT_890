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
 * linux/arch/sparc/mm/extable.c
 */

#include <linux/module.h>
#include <asm/uaccess.h>

void sort_extable(struct exception_table_entry *start,
		  struct exception_table_entry *finish)
{
}

/* Caller knows they are in a range if ret->fixup == 0 */
const struct exception_table_entry *
search_extable(const struct exception_table_entry *start,
	       const struct exception_table_entry *last,
	       unsigned long value)
{
	const struct exception_table_entry *walk;

	/* Single insn entries are encoded as:
	 *	word 1:	insn address
	 *	word 2:	fixup code address
	 *
	 * Range entries are encoded as:
	 *	word 1: first insn address
	 *	word 2: 0
	 *	word 3: last insn address + 4 bytes
	 *	word 4: fixup code address
	 *
	 * See asm/uaccess.h for more details.
	 */

	/* 1. Try to find an exact match. */
	for (walk = start; walk <= last; walk++) {
		if (walk->fixup == 0) {
			/* A range entry, skip both parts. */
			walk++;
			continue;
		}

		if (walk->insn == value)
			return walk;
	}

	/* 2. Try to find a range match. */
	for (walk = start; walk <= (last - 1); walk++) {
		if (walk->fixup)
			continue;

		if (walk[0].insn <= value && walk[1].insn > value)
			return walk;

		walk++;
	}

        return NULL;
}

/* Special extable search, which handles ranges.  Returns fixup */
unsigned long search_extables_range(unsigned long addr, unsigned long *g2)
{
	const struct exception_table_entry *entry;

	entry = search_exception_tables(addr);
	if (!entry)
		return 0;

	/* Inside range?  Fix g2 and return correct fixup */
	if (!entry->fixup) {
		*g2 = (addr - entry->insn) / 4;
		return (entry + 1)->fixup;
	}

	return entry->fixup;
}
