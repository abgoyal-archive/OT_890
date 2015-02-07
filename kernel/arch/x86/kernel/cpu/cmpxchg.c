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
 * cmpxchg*() fallbacks for CPU not supporting these instructions
 */

#include <linux/kernel.h>
#include <linux/smp.h>
#include <linux/module.h>

#ifndef CONFIG_X86_CMPXCHG
unsigned long cmpxchg_386_u8(volatile void *ptr, u8 old, u8 new)
{
	u8 prev;
	unsigned long flags;

	/* Poor man's cmpxchg for 386. Unsuitable for SMP */
	local_irq_save(flags);
	prev = *(u8 *)ptr;
	if (prev == old)
		*(u8 *)ptr = new;
	local_irq_restore(flags);
	return prev;
}
EXPORT_SYMBOL(cmpxchg_386_u8);

unsigned long cmpxchg_386_u16(volatile void *ptr, u16 old, u16 new)
{
	u16 prev;
	unsigned long flags;

	/* Poor man's cmpxchg for 386. Unsuitable for SMP */
	local_irq_save(flags);
	prev = *(u16 *)ptr;
	if (prev == old)
		*(u16 *)ptr = new;
	local_irq_restore(flags);
	return prev;
}
EXPORT_SYMBOL(cmpxchg_386_u16);

unsigned long cmpxchg_386_u32(volatile void *ptr, u32 old, u32 new)
{
	u32 prev;
	unsigned long flags;

	/* Poor man's cmpxchg for 386. Unsuitable for SMP */
	local_irq_save(flags);
	prev = *(u32 *)ptr;
	if (prev == old)
		*(u32 *)ptr = new;
	local_irq_restore(flags);
	return prev;
}
EXPORT_SYMBOL(cmpxchg_386_u32);
#endif

#ifndef CONFIG_X86_CMPXCHG64
unsigned long long cmpxchg_486_u64(volatile void *ptr, u64 old, u64 new)
{
	u64 prev;
	unsigned long flags;

	/* Poor man's cmpxchg8b for 386 and 486. Unsuitable for SMP */
	local_irq_save(flags);
	prev = *(u64 *)ptr;
	if (prev == old)
		*(u64 *)ptr = new;
	local_irq_restore(flags);
	return prev;
}
EXPORT_SYMBOL(cmpxchg_486_u64);
#endif

