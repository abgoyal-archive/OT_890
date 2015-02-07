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

#ifndef _M68KNOMMU_DELAY_H
#define _M68KNOMMU_DELAY_H

/*
 * Copyright (C) 1994 Hamish Macdonald
 * Copyright (C) 2004 Greg Ungerer <gerg@snapgear.com>
 */

#include <asm/param.h>

static inline void __delay(unsigned long loops)
{
#if defined(CONFIG_COLDFIRE)
	/* The coldfire runs this loop at significantly different speeds
	 * depending upon long word alignment or not.  We'll pad it to
	 * long word alignment which is the faster version.
	 * The 0x4a8e is of course a 'tstl %fp' instruction.  This is better
	 * than using a NOP (0x4e71) instruction because it executes in one
	 * cycle not three and doesn't allow for an arbitary delay waiting
	 * for bus cycles to finish.  Also fp/a6 isn't likely to cause a
	 * stall waiting for the register to become valid if such is added
	 * to the coldfire at some stage.
	 */
	__asm__ __volatile__ (	".balignw 4, 0x4a8e\n\t"
				"1: subql #1, %0\n\t"
				"jcc 1b"
		: "=d" (loops) : "0" (loops));
#else
	__asm__ __volatile__ (	"1: subql #1, %0\n\t"
				"jcc 1b"
		: "=d" (loops) : "0" (loops));
#endif
}

/*
 *	Ideally we use a 32*32->64 multiply to calculate the number of
 *	loop iterations, but the older standard 68k and ColdFire do not
 *	have this instruction. So for them we have a clsoe approximation
 *	loop using 32*32->32 multiplies only. This calculation based on
 *	the ARM version of delay.
 *
 *	We want to implement:
 *
 *	loops = (usecs * 0x10c6 * HZ * loops_per_jiffy) / 2^32
 */

#define	HZSCALE		(268435456 / (1000000/HZ))

extern unsigned long loops_per_jiffy;

static inline void _udelay(unsigned long usecs)
{
#if defined(CONFIG_M68328) || defined(CONFIG_M68EZ328) || \
    defined(CONFIG_M68VZ328) || defined(CONFIG_M68360) || \
    defined(CONFIG_COLDFIRE)
	__delay((((usecs * HZSCALE) >> 11) * (loops_per_jiffy >> 11)) >> 6);
#else
	unsigned long tmp;

	usecs *= 4295;		/* 2**32 / 1000000 */
	__asm__ ("mulul %2,%0:%1"
		: "=d" (usecs), "=d" (tmp)
		: "d" (usecs), "1" (loops_per_jiffy*HZ));
	__delay(usecs);
#endif
}

/*
 *	Moved the udelay() function into library code, no longer inlined.
 *	I had to change the algorithm because we are overflowing now on
 *	the faster ColdFire parts. The code is a little bigger, so it makes
 *	sense to library it.
 */
extern void udelay(unsigned long usecs);

#endif /* defined(_M68KNOMMU_DELAY_H) */
