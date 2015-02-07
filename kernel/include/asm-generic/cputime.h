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

#ifndef _ASM_GENERIC_CPUTIME_H
#define _ASM_GENERIC_CPUTIME_H

#include <linux/time.h>
#include <linux/jiffies.h>

typedef unsigned long cputime_t;

#define cputime_zero			(0UL)
#define cputime_max			((~0UL >> 1) - 1)
#define cputime_add(__a, __b)		((__a) +  (__b))
#define cputime_sub(__a, __b)		((__a) -  (__b))
#define cputime_div(__a, __n)		((__a) /  (__n))
#define cputime_halve(__a)		((__a) >> 1)
#define cputime_eq(__a, __b)		((__a) == (__b))
#define cputime_gt(__a, __b)		((__a) >  (__b))
#define cputime_ge(__a, __b)		((__a) >= (__b))
#define cputime_lt(__a, __b)		((__a) <  (__b))
#define cputime_le(__a, __b)		((__a) <= (__b))
#define cputime_to_jiffies(__ct)	(__ct)
#define cputime_to_scaled(__ct)		(__ct)
#define jiffies_to_cputime(__hz)	(__hz)

typedef u64 cputime64_t;

#define cputime64_zero (0ULL)
#define cputime64_add(__a, __b)		((__a) + (__b))
#define cputime64_sub(__a, __b)		((__a) - (__b))
#define cputime64_to_jiffies64(__ct)	(__ct)
#define jiffies64_to_cputime64(__jif)	(__jif)
#define cputime_to_cputime64(__ct)	((u64) __ct)


/*
 * Convert cputime to milliseconds and back.
 */
#define cputime_to_msecs(__ct)		jiffies_to_msecs(__ct)
#define msecs_to_cputime(__msecs)	msecs_to_jiffies(__msecs)

/*
 * Convert cputime to seconds and back.
 */
#define cputime_to_secs(jif)		((jif) / HZ)
#define secs_to_cputime(sec)		((sec) * HZ)

/*
 * Convert cputime to timespec and back.
 */
#define timespec_to_cputime(__val)	timespec_to_jiffies(__val)
#define cputime_to_timespec(__ct,__val)	jiffies_to_timespec(__ct,__val)

/*
 * Convert cputime to timeval and back.
 */
#define timeval_to_cputime(__val)	timeval_to_jiffies(__val)
#define cputime_to_timeval(__ct,__val)	jiffies_to_timeval(__ct,__val)

/*
 * Convert cputime to clock and back.
 */
#define cputime_to_clock_t(__ct)	jiffies_to_clock_t(__ct)
#define clock_t_to_cputime(__x)		clock_t_to_jiffies(__x)

/*
 * Convert cputime64 to clock.
 */
#define cputime64_to_clock_t(__ct)	jiffies_64_to_clock_t(__ct)

#endif
