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
 *  arch/blackfin/kernel/flat.c
 *
 *  Copyright (C) 2007 Analog Devices, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <linux/module.h>
#include <linux/sched.h>
#include <linux/flat.h>

#define FLAT_BFIN_RELOC_TYPE_16_BIT 0
#define FLAT_BFIN_RELOC_TYPE_16H_BIT 1
#define FLAT_BFIN_RELOC_TYPE_32_BIT 2

unsigned long bfin_get_addr_from_rp(unsigned long *ptr,
		unsigned long relval,
		unsigned long flags,
		unsigned long *persistent)
{
	unsigned short *usptr = (unsigned short *)ptr;
	int type = (relval >> 26) & 7;
	unsigned long val;

	switch (type) {
	case FLAT_BFIN_RELOC_TYPE_16_BIT:
	case FLAT_BFIN_RELOC_TYPE_16H_BIT:
		usptr = (unsigned short *)ptr;
		pr_debug("*usptr = %x", get_unaligned(usptr));
		val = get_unaligned(usptr);
		val += *persistent;
		break;

	case FLAT_BFIN_RELOC_TYPE_32_BIT:
		pr_debug("*ptr = %lx", get_unaligned(ptr));
		val = get_unaligned(ptr);
		break;

	default:
		pr_debug("BINFMT_FLAT: Unknown relocation type %x\n", type);
		return 0;
	}

	/*
	 * Stack-relative relocs contain the offset into the stack, we
	 * have to add the stack's start address here and return 1 from
	 * flat_addr_absolute to prevent the normal address calculations
	 */
	if (relval & (1 << 29))
		return val + current->mm->context.end_brk;

	if ((flags & FLAT_FLAG_GOTPIC) == 0)
		val = htonl(val);
	return val;
}
EXPORT_SYMBOL(bfin_get_addr_from_rp);

/*
 * Insert the address ADDR into the symbol reference at RP;
 * RELVAL is the raw relocation-table entry from which RP is derived
 */
void bfin_put_addr_at_rp(unsigned long *ptr, unsigned long addr,
		unsigned long relval)
{
	unsigned short *usptr = (unsigned short *)ptr;
	int type = (relval >> 26) & 7;

	switch (type) {
	case FLAT_BFIN_RELOC_TYPE_16_BIT:
		put_unaligned(addr, usptr);
		pr_debug("new value %x at %p", get_unaligned(usptr), usptr);
		break;

	case FLAT_BFIN_RELOC_TYPE_16H_BIT:
		put_unaligned(addr >> 16, usptr);
		pr_debug("new value %x", get_unaligned(usptr));
		break;

	case FLAT_BFIN_RELOC_TYPE_32_BIT:
		put_unaligned(addr, ptr);
		pr_debug("new ptr =%lx", get_unaligned(ptr));
		break;
	}
}
EXPORT_SYMBOL(bfin_put_addr_at_rp);
