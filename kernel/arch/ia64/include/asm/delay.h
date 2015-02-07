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

#ifndef _ASM_IA64_DELAY_H
#define _ASM_IA64_DELAY_H

/*
 * Delay routines using a pre-computed "cycles/usec" value.
 *
 * Copyright (C) 1998, 1999 Hewlett-Packard Co
 *	David Mosberger-Tang <davidm@hpl.hp.com>
 * Copyright (C) 1999 VA Linux Systems
 * Copyright (C) 1999 Walt Drummond <drummond@valinux.com>
 * Copyright (C) 1999 Asit Mallick <asit.k.mallick@intel.com>
 * Copyright (C) 1999 Don Dugger <don.dugger@intel.com>
 */

#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/compiler.h>

#include <asm/intrinsics.h>
#include <asm/processor.h>

static __inline__ void
ia64_set_itm (unsigned long val)
{
	ia64_setreg(_IA64_REG_CR_ITM, val);
	ia64_srlz_d();
}

static __inline__ unsigned long
ia64_get_itm (void)
{
	unsigned long result;

	result = ia64_getreg(_IA64_REG_CR_ITM);
	ia64_srlz_d();
	return result;
}

static __inline__ void
ia64_set_itv (unsigned long val)
{
	ia64_setreg(_IA64_REG_CR_ITV, val);
	ia64_srlz_d();
}

static __inline__ unsigned long
ia64_get_itv (void)
{
	return ia64_getreg(_IA64_REG_CR_ITV);
}

static __inline__ void
ia64_set_itc (unsigned long val)
{
	ia64_setreg(_IA64_REG_AR_ITC, val);
	ia64_srlz_d();
}

static __inline__ unsigned long
ia64_get_itc (void)
{
	unsigned long result;

	result = ia64_getreg(_IA64_REG_AR_ITC);
	ia64_barrier();
#ifdef CONFIG_ITANIUM
	while (unlikely((__s32) result == -1)) {
		result = ia64_getreg(_IA64_REG_AR_ITC);
		ia64_barrier();
	}
#endif
	return result;
}

extern void ia64_delay_loop (unsigned long loops);

static __inline__ void
__delay (unsigned long loops)
{
	if (unlikely(loops < 1))
		return;

	ia64_delay_loop (loops - 1);
}

extern void udelay (unsigned long usecs);

#endif /* _ASM_IA64_DELAY_H */
