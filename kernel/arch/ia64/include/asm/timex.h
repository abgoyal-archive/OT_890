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

#ifndef _ASM_IA64_TIMEX_H
#define _ASM_IA64_TIMEX_H

/*
 * Copyright (C) 1998-2001, 2003 Hewlett-Packard Co
 *	David Mosberger-Tang <davidm@hpl.hp.com>
 */
/*
 * 2001/01/18 davidm	Removed CLOCK_TICK_RATE.  It makes no sense on IA-64.
 *			Also removed cacheflush_time as it's entirely unused.
 */

#include <asm/intrinsics.h>
#include <asm/processor.h>

typedef unsigned long cycles_t;

extern void (*ia64_udelay)(unsigned long usecs);

/*
 * For performance reasons, we don't want to define CLOCK_TICK_TRATE as
 * local_cpu_data->itc_rate.  Fortunately, we don't have to, either: according to George
 * Anzinger, 1/CLOCK_TICK_RATE is taken as the resolution of the timer clock.  The time
 * calculation assumes that you will use enough of these so that your tick size <= 1/HZ.
 * If the calculation shows that your CLOCK_TICK_RATE can not supply exactly 1/HZ ticks,
 * the actual value is calculated and used to update the wall clock each jiffie.  Setting
 * the CLOCK_TICK_RATE to x*HZ insures that the calculation will find no errors.  Hence we
 * pick a multiple of HZ which gives us a (totally virtual) CLOCK_TICK_RATE of about
 * 100MHz.
 */
#define CLOCK_TICK_RATE		(HZ * 100000UL)

static inline cycles_t
get_cycles (void)
{
	cycles_t ret;

	ret = ia64_getreg(_IA64_REG_AR_ITC);
	return ret;
}

extern void ia64_cpu_local_tick (void);

#endif /* _ASM_IA64_TIMEX_H */
