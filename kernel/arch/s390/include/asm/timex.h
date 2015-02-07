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
 *  include/asm-s390/timex.h
 *
 *  S390 version
 *    Copyright (C) 1999 IBM Deutschland Entwicklung GmbH, IBM Corporation
 *
 *  Derived from "include/asm-i386/timex.h"
 *    Copyright (C) 1992, Linus Torvalds
 */

#ifndef _ASM_S390_TIMEX_H
#define _ASM_S390_TIMEX_H

/* Inline functions for clock register access. */
static inline int set_clock(__u64 time)
{
	int cc;

	asm volatile(
		"   sck   0(%2)\n"
		"   ipm   %0\n"
		"   srl   %0,28\n"
		: "=d" (cc) : "m" (time), "a" (&time) : "cc");
	return cc;
}

static inline int store_clock(__u64 *time)
{
	int cc;

	asm volatile(
		"   stck  0(%2)\n"
		"   ipm   %0\n"
		"   srl   %0,28\n"
		: "=d" (cc), "=m" (*time) : "a" (time) : "cc");
	return cc;
}

static inline void set_clock_comparator(__u64 time)
{
	asm volatile("sckc 0(%1)" : : "m" (time), "a" (&time));
}

static inline void store_clock_comparator(__u64 *time)
{
	asm volatile("stckc 0(%1)" : "=m" (*time) : "a" (time));
}

#define CLOCK_TICK_RATE	1193180 /* Underlying HZ */

typedef unsigned long long cycles_t;

static inline unsigned long long get_clock (void)
{
	unsigned long long clk;

#if __GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ > 2)
	asm volatile("stck %0" : "=Q" (clk) : : "cc");
#else /* __GNUC__ */
	asm volatile("stck 0(%1)" : "=m" (clk) : "a" (&clk) : "cc");
#endif /* __GNUC__ */
	return clk;
}

static inline unsigned long long get_clock_xt(void)
{
	unsigned char clk[16];

#if __GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ > 2)
	asm volatile("stcke %0" : "=Q" (clk) : : "cc");
#else /* __GNUC__ */
	asm volatile("stcke 0(%1)" : "=m" (clk)
				   : "a" (clk) : "cc");
#endif /* __GNUC__ */

	return *((unsigned long long *)&clk[1]);
}

static inline cycles_t get_cycles(void)
{
	return (cycles_t) get_clock() >> 2;
}

int get_sync_clock(unsigned long long *clock);
void init_cpu_timer(void);
unsigned long long monotonic_clock(void);

#endif
