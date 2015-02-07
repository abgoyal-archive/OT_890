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

/* user_fixup.c: Fix up user copy faults.
 *
 * Copyright (C) 2004 David S. Miller <davem@redhat.com>
 */

#include <linux/compiler.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/errno.h>
#include <linux/module.h>

#include <asm/uaccess.h>

/* Calculating the exact fault address when using
 * block loads and stores can be very complicated.
 *
 * Instead of trying to be clever and handling all
 * of the cases, just fix things up simply here.
 */

static unsigned long compute_size(unsigned long start, unsigned long size, unsigned long *offset)
{
	unsigned long fault_addr = current_thread_info()->fault_address;
	unsigned long end = start + size;

	if (fault_addr < start || fault_addr >= end) {
		*offset = 0;
	} else {
		*offset = fault_addr - start;
		size = end - fault_addr;
	}
	return size;
}

unsigned long copy_from_user_fixup(void *to, const void __user *from, unsigned long size)
{
	unsigned long offset;

	size = compute_size((unsigned long) from, size, &offset);
	if (likely(size))
		memset(to + offset, 0, size);

	return size;
}
EXPORT_SYMBOL(copy_from_user_fixup);

unsigned long copy_to_user_fixup(void __user *to, const void *from, unsigned long size)
{
	unsigned long offset;

	return compute_size((unsigned long) to, size, &offset);
}
EXPORT_SYMBOL(copy_to_user_fixup);

unsigned long copy_in_user_fixup(void __user *to, void __user *from, unsigned long size)
{
	unsigned long fault_addr = current_thread_info()->fault_address;
	unsigned long start = (unsigned long) to;
	unsigned long end = start + size;

	if (fault_addr >= start && fault_addr < end)
		return end - fault_addr;

	start = (unsigned long) from;
	end = start + size;
	if (fault_addr >= start && fault_addr < end)
		return end - fault_addr;

	return size;
}
EXPORT_SYMBOL(copy_in_user_fixup);
