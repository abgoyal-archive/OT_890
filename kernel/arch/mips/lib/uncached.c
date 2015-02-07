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
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2005 Thiemo Seufer
 * Copyright (C) 2005  MIPS Technologies, Inc.  All rights reserved.
 *	Author: Maciej W. Rozycki <macro@mips.com>
 */

#include <linux/init.h>

#include <asm/addrspace.h>
#include <asm/bug.h>
#include <asm/cacheflush.h>

#ifndef CKSEG2
#define CKSEG2 CKSSEG
#endif
#ifndef TO_PHYS_MASK
#define TO_PHYS_MASK -1
#endif

/*
 * FUNC is executed in one of the uncached segments, depending on its
 * original address as follows:
 *
 * 1. If the original address is in CKSEG0 or CKSEG1, then the uncached
 *    segment used is CKSEG1.
 * 2. If the original address is in XKPHYS, then the uncached segment
 *    used is XKPHYS(2).
 * 3. Otherwise it's a bug.
 *
 * The same remapping is done with the stack pointer.  Stack handling
 * works because we don't handle stack arguments or more complex return
 * values, so we can avoid sharing the same stack area between a cached
 * and the uncached mode.
 */
unsigned long __cpuinit run_uncached(void *func)
{
	register long sp __asm__("$sp");
	register long ret __asm__("$2");
	long lfunc = (long)func, ufunc;
	long usp;

	if (sp >= (long)CKSEG0 && sp < (long)CKSEG2)
		usp = CKSEG1ADDR(sp);
#ifdef CONFIG_64BIT
	else if ((long long)sp >= (long long)PHYS_TO_XKPHYS(0, 0) &&
		 (long long)sp < (long long)PHYS_TO_XKPHYS(8, 0))
		usp = PHYS_TO_XKPHYS(K_CALG_UNCACHED,
				     XKPHYS_TO_PHYS((long long)sp));
#endif
	else {
		BUG();
		usp = sp;
	}
	if (lfunc >= (long)CKSEG0 && lfunc < (long)CKSEG2)
		ufunc = CKSEG1ADDR(lfunc);
#ifdef CONFIG_64BIT
	else if ((long long)lfunc >= (long long)PHYS_TO_XKPHYS(0, 0) &&
		 (long long)lfunc < (long long)PHYS_TO_XKPHYS(8, 0))
		ufunc = PHYS_TO_XKPHYS(K_CALG_UNCACHED,
				       XKPHYS_TO_PHYS((long long)lfunc));
#endif
	else {
		BUG();
		ufunc = lfunc;
	}

	__asm__ __volatile__ (
		"	move	$16, $sp\n"
		"	move	$sp, %1\n"
		"	jalr	%2\n"
		"	move	$sp, $16"
		: "=r" (ret)
		: "r" (usp), "r" (ufunc)
		: "$16", "$31");

	return ret;
}
