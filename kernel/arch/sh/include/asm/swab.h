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

#ifndef __ASM_SH_SWAB_H
#define __ASM_SH_SWAB_H

/*
 * Copyright (C) 1999  Niibe Yutaka
 * Copyright (C) 2000, 2001  Paolo Alberelli
 */
#include <linux/compiler.h>
#include <linux/types.h>

#define __SWAB_64_THRU_32__

static inline __attribute_const__ __u32 __arch_swab32(__u32 x)
{
	__asm__(
#ifdef __SH5__
		"byterev	%0, %0\n\t"
		"shari		%0, 32, %0"
#else
		"swap.b		%0, %0\n\t"
		"swap.w		%0, %0\n\t"
		"swap.b		%0, %0"
#endif
		: "=r" (x)
		: "0" (x));

	return x;
}
#define __arch_swab32 __arch_swab32

static inline __attribute_const__ __u16 __arch_swab16(__u16 x)
{
	__asm__(
#ifdef __SH5__
		"byterev	%0, %0\n\t"
		"shari		%0, 32, %0"
#else
		"swap.b		%0, %0"
#endif
		: "=r" (x)
		:  "0" (x));

	return x;
}
#define __arch_swab16 __arch_swab16

static inline __u64 __arch_swab64(__u64 val)
{
	union {
		struct { __u32 a,b; } s;
		__u64 u;
	} v, w;
	v.u = val;
	w.s.b = __arch_swab32(v.s.a);
	w.s.a = __arch_swab32(v.s.b);
	return w.u;
}
#define __arch_swab64 __arch_swab64

#endif /* __ASM_SH_SWAB_H */
