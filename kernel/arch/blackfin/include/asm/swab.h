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

#ifndef _BLACKFIN_SWAB_H
#define _BLACKFIN_SWAB_H

#include <asm/types.h>
#include <linux/compiler.h>

#if defined(__GNUC__) && !defined(__STRICT_ANSI__) || defined(__KERNEL__)
#  define __SWAB_64_THRU_32__
#endif

#ifdef __GNUC__

static __inline__ __attribute_const__ __u32 __arch_swahb32(__u32 xx)
{
	__u32 tmp;
	__asm__("%1 = %0 >> 8 (V);\n\t"
		"%0 = %0 << 8 (V);\n\t"
		"%0 = %0 | %1;\n\t"
		: "+d"(xx), "=&d"(tmp));
	return xx;
}
#define __arch_swahb32 __arch_swahb32

static __inline__ __attribute_const__ __u32 __arch_swahw32(__u32 xx)
{
	__u32 rv;
	__asm__("%0 = PACK(%1.L, %1.H);\n\t": "=d"(rv): "d"(xx));
	return rv;
}
#define __arch_swahw32 __arch_swahw32

static __inline__ __attribute_const__ __u32 __arch_swab32(__u32 xx)
{
	return __arch_swahb32(__arch_swahw32(xx));
}
#define __arch_swab32 __arch_swab32

static __inline__ __attribute_const__ __u16 __arch_swab16(__u16 xx)
{
	__u32 xw = xx;
	__asm__("%0 <<= 8;\n	%0.L = %0.L + %0.H (NS);\n": "+d"(xw));
	return (__u16)xw;
}
#define __arch_swab16 __arch_swab16

#endif /* __GNUC__ */

#endif				/* _BLACKFIN_SWAB_H */
