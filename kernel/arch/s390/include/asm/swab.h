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

#ifndef _S390_SWAB_H
#define _S390_SWAB_H

/*
 *  include/asm-s390/swab.h
 *
 *  S390 version
 *    Copyright (C) 1999 IBM Deutschland Entwicklung GmbH, IBM Corporation
 *    Author(s): Martin Schwidefsky (schwidefsky@de.ibm.com)
 */

#include <linux/types.h>

#ifndef __s390x__
# define __SWAB_64_THRU_32__
#endif

#ifdef __s390x__
static inline __u64 __arch_swab64p(const __u64 *x)
{
	__u64 result;

	asm volatile("lrvg %0,%1" : "=d" (result) : "m" (*x));
	return result;
}
#define __arch_swab64p __arch_swab64p

static inline __u64 __arch_swab64(__u64 x)
{
	__u64 result;

	asm volatile("lrvgr %0,%1" : "=d" (result) : "d" (x));
	return result;
}
#define __arch_swab64 __arch_swab64

static inline void __arch_swab64s(__u64 *x)
{
	*x = __arch_swab64p(x);
}
#define __arch_swab64s __arch_swab64s
#endif /* __s390x__ */

static inline __u32 __arch_swab32p(const __u32 *x)
{
	__u32 result;
	
	asm volatile(
#ifndef __s390x__
		"	icm	%0,8,3(%1)\n"
		"	icm	%0,4,2(%1)\n"
		"	icm	%0,2,1(%1)\n"
		"	ic	%0,0(%1)"
		: "=&d" (result) : "a" (x), "m" (*x) : "cc");
#else /* __s390x__ */
		"	lrv	%0,%1"
		: "=d" (result) : "m" (*x));
#endif /* __s390x__ */
	return result;
}
#define __arch_swab32p __arch_swab32p

#ifdef __s390x__
static inline __u32 __arch_swab32(__u32 x)
{
	__u32 result;
	
	asm volatile("lrvr  %0,%1" : "=d" (result) : "d" (x));
	return result;
}
#define __arch_swab32 __arch_swab32
#endif /* __s390x__ */

static inline __u16 __arch_swab16p(const __u16 *x)
{
	__u16 result;
	
	asm volatile(
#ifndef __s390x__
		"	icm	%0,2,1(%1)\n"
		"	ic	%0,0(%1)\n"
		: "=&d" (result) : "a" (x), "m" (*x) : "cc");
#else /* __s390x__ */
		"	lrvh	%0,%1"
		: "=d" (result) : "m" (*x));
#endif /* __s390x__ */
	return result;
}
#define __arch_swab16p __arch_swab16p

#endif /* _S390_SWAB_H */
