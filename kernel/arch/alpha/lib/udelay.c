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
 * Copyright (C) 1993, 2000 Linus Torvalds
 *
 * Delay routines, using a pre-computed "loops_per_jiffy" value.
 */

#include <linux/module.h>
#include <linux/sched.h> /* for udelay's use of smp_processor_id */
#include <asm/param.h>
#include <asm/smp.h>
#include <linux/delay.h>

/*
 * Use only for very small delays (< 1 msec). 
 *
 * The active part of our cycle counter is only 32-bits wide, and
 * we're treating the difference between two marks as signed.  On
 * a 1GHz box, that's about 2 seconds.
 */

void
__delay(int loops)
{
	int tmp;
	__asm__ __volatile__(
		"	rpcc %0\n"
		"	addl %1,%0,%1\n"
		"1:	rpcc %0\n"
		"	subl %1,%0,%0\n"
		"	bgt %0,1b"
		: "=&r" (tmp), "=r" (loops) : "1"(loops));
}

#ifdef CONFIG_SMP
#define LPJ	 cpu_data[smp_processor_id()].loops_per_jiffy
#else
#define LPJ	 loops_per_jiffy
#endif

void
udelay(unsigned long usecs)
{
	usecs *= (((unsigned long)HZ << 32) / 1000000) * LPJ;
	__delay((long)usecs >> 32);
}
EXPORT_SYMBOL(udelay);

void
ndelay(unsigned long nsecs)
{
	nsecs *= (((unsigned long)HZ << 32) / 1000000000) * LPJ;
	__delay((long)nsecs >> 32);
}
EXPORT_SYMBOL(ndelay);
