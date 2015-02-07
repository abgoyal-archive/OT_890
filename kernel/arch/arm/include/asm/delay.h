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
 * Copyright (C) 1995-2004 Russell King
 *
 * Delay routines, using a pre-computed "loops_per_second" value.
 */
#ifndef __ASM_ARM_DELAY_H
#define __ASM_ARM_DELAY_H

#include <asm/param.h>	/* HZ */

extern void __delay(int loops);

/*
 * This function intentionally does not exist; if you see references to
 * it, it means that you're calling udelay() with an out of range value.
 *
 * With currently imposed limits, this means that we support a max delay
 * of 2000us. Further limits: HZ<=1000 and bogomips<=3355
 */
extern void __bad_udelay(void);

/*
 * division by multiplication: you don't have to worry about
 * loss of precision.
 *
 * Use only for very small delays ( < 1 msec).  Should probably use a
 * lookup table, really, as the multiplications take much too long with
 * short delays.  This is a "reasonable" implementation, though (and the
 * first constant multiplications gets optimized away if the delay is
 * a constant)
 */
extern void __udelay(unsigned long usecs);
extern void __const_udelay(unsigned long);

#define MAX_UDELAY_MS 2

#define udelay(n)							\
	(__builtin_constant_p(n) ?					\
	  ((n) > (MAX_UDELAY_MS * 1000) ? __bad_udelay() :		\
			__const_udelay((n) * ((2199023U*HZ)>>11))) :	\
	  __udelay(n))

#endif /* defined(_ARM_DELAY_H) */

