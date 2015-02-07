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
 * Spin and read/write lock operations.
 *
 * Copyright (C) 2001-2004 Paul Mackerras <paulus@au.ibm.com>, IBM
 * Copyright (C) 2001 Anton Blanchard <anton@au.ibm.com>, IBM
 * Copyright (C) 2002 Dave Engebretsen <engebret@us.ibm.com>, IBM
 *   Rework to support virtual processors
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#include <linux/kernel.h>
#include <linux/spinlock.h>
#include <linux/module.h>
#include <linux/stringify.h>
#include <linux/smp.h>

/* waiting for a spinlock... */
#if defined(CONFIG_PPC_SPLPAR) || defined(CONFIG_PPC_ISERIES)
#include <asm/hvcall.h>
#include <asm/iseries/hv_call.h>
#include <asm/smp.h>
#include <asm/firmware.h>

void __spin_yield(raw_spinlock_t *lock)
{
	unsigned int lock_value, holder_cpu, yield_count;

	lock_value = lock->slock;
	if (lock_value == 0)
		return;
	holder_cpu = lock_value & 0xffff;
	BUG_ON(holder_cpu >= NR_CPUS);
	yield_count = lppaca[holder_cpu].yield_count;
	if ((yield_count & 1) == 0)
		return;		/* virtual cpu is currently running */
	rmb();
	if (lock->slock != lock_value)
		return;		/* something has changed */
	if (firmware_has_feature(FW_FEATURE_ISERIES))
		HvCall2(HvCallBaseYieldProcessor, HvCall_YieldToProc,
			((u64)holder_cpu << 32) | yield_count);
#ifdef CONFIG_PPC_SPLPAR
	else
		plpar_hcall_norets(H_CONFER,
			get_hard_smp_processor_id(holder_cpu), yield_count);
#endif
}

/*
 * Waiting for a read lock or a write lock on a rwlock...
 * This turns out to be the same for read and write locks, since
 * we only know the holder if it is write-locked.
 */
void __rw_yield(raw_rwlock_t *rw)
{
	int lock_value;
	unsigned int holder_cpu, yield_count;

	lock_value = rw->lock;
	if (lock_value >= 0)
		return;		/* no write lock at present */
	holder_cpu = lock_value & 0xffff;
	BUG_ON(holder_cpu >= NR_CPUS);
	yield_count = lppaca[holder_cpu].yield_count;
	if ((yield_count & 1) == 0)
		return;		/* virtual cpu is currently running */
	rmb();
	if (rw->lock != lock_value)
		return;		/* something has changed */
	if (firmware_has_feature(FW_FEATURE_ISERIES))
		HvCall2(HvCallBaseYieldProcessor, HvCall_YieldToProc,
			((u64)holder_cpu << 32) | yield_count);
#ifdef CONFIG_PPC_SPLPAR
	else
		plpar_hcall_norets(H_CONFER,
			get_hard_smp_processor_id(holder_cpu), yield_count);
#endif
}
#endif

void __raw_spin_unlock_wait(raw_spinlock_t *lock)
{
	while (lock->slock) {
		HMT_low();
		if (SHARED_PROCESSOR)
			__spin_yield(lock);
	}
	HMT_medium();
}

EXPORT_SYMBOL(__raw_spin_unlock_wait);
