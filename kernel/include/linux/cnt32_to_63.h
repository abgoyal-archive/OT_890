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
 *  Extend a 32-bit counter to 63 bits
 *
 *  Author:	Nicolas Pitre
 *  Created:	December 3, 2006
 *  Copyright:	MontaVista Software, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 */

#ifndef __LINUX_CNT32_TO_63_H__
#define __LINUX_CNT32_TO_63_H__

#include <linux/compiler.h>
#include <linux/types.h>
#include <asm/byteorder.h>
#include <asm/system.h>

/* this is used only to give gcc a clue about good code generation */
union cnt32_to_63 {
	struct {
#if defined(__LITTLE_ENDIAN)
		u32 lo, hi;
#elif defined(__BIG_ENDIAN)
		u32 hi, lo;
#endif
	};
	u64 val;
};


/**
 * cnt32_to_63 - Expand a 32-bit counter to a 63-bit counter
 * @cnt_lo: The low part of the counter
 *
 * Many hardware clock counters are only 32 bits wide and therefore have
 * a relatively short period making wrap-arounds rather frequent.  This
 * is a problem when implementing sched_clock() for example, where a 64-bit
 * non-wrapping monotonic value is expected to be returned.
 *
 * To overcome that limitation, let's extend a 32-bit counter to 63 bits
 * in a completely lock free fashion. Bits 0 to 31 of the clock are provided
 * by the hardware while bits 32 to 62 are stored in memory.  The top bit in
 * memory is used to synchronize with the hardware clock half-period.  When
 * the top bit of both counters (hardware and in memory) differ then the
 * memory is updated with a new value, incrementing it when the hardware
 * counter wraps around.
 *
 * Because a word store in memory is atomic then the incremented value will
 * always be in synch with the top bit indicating to any potential concurrent
 * reader if the value in memory is up to date or not with regards to the
 * needed increment.  And any race in updating the value in memory is harmless
 * as the same value would simply be stored more than once.
 *
 * The restrictions for the algorithm to work properly are:
 *
 * 1) this code must be called at least once per each half period of the
 *    32-bit counter;
 *
 * 2) this code must not be preempted for a duration longer than the
 *    32-bit counter half period minus the longest period between two
 *    calls to this code.
 *
 * Those requirements ensure proper update to the state bit in memory.
 * This is usually not a problem in practice, but if it is then a kernel
 * timer should be scheduled to manage for this code to be executed often
 * enough.
 *
 * Note that the top bit (bit 63) in the returned value should be considered
 * as garbage.  It is not cleared here because callers are likely to use a
 * multiplier on the returned value which can get rid of the top bit
 * implicitly by making the multiplier even, therefore saving on a runtime
 * clear-bit instruction. Otherwise caller must remember to clear the top
 * bit explicitly.
 */
#define cnt32_to_63(cnt_lo) \
({ \
	static u32 __m_cnt_hi; \
	union cnt32_to_63 __x; \
	__x.hi = __m_cnt_hi; \
 	smp_rmb(); \
	__x.lo = (cnt_lo); \
	if (unlikely((s32)(__x.hi ^ __x.lo) < 0)) \
		__m_cnt_hi = __x.hi = (__x.hi ^ 0x80000000) + (__x.hi >> 31); \
	__x.val; \
})

#endif
