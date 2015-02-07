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
 * include/asm-xtensa/swab.h
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2001 - 2005 Tensilica Inc.
 */

#ifndef _XTENSA_SWAB_H
#define _XTENSA_SWAB_H

#include <asm/types.h>
#include <linux/compiler.h>

#define __SWAB_64_THRU_32__

static inline __attribute_const__ __u32 __arch_swab32(__u32 x)
{
    __u32 res;
    /* instruction sequence from Xtensa ISA release 2/2000 */
    __asm__("ssai     8           \n\t"
	    "srli     %0, %1, 16  \n\t"
	    "src      %0, %0, %1  \n\t"
	    "src      %0, %0, %0  \n\t"
	    "src      %0, %1, %0  \n"
	    : "=&a" (res)
	    : "a" (x)
	    );
    return res;
}
#define __arch_swab32 __arch_swab32

static inline __attribute_const__ __u16 __arch_swab16(__u16 x)
{
    /* Given that 'short' values are signed (i.e., can be negative),
     * we cannot assume that the upper 16-bits of the register are
     * zero.  We are careful to mask values after shifting.
     */

    /* There exists an anomaly between xt-gcc and xt-xcc.  xt-gcc
     * inserts an extui instruction after putting this function inline
     * to ensure that it uses only the least-significant 16 bits of
     * the result.  xt-xcc doesn't use an extui, but assumes the
     * __asm__ macro follows convention that the upper 16 bits of an
     * 'unsigned short' result are still zero.  This macro doesn't
     * follow convention; indeed, it leaves garbage in the upport 16
     * bits of the register.

     * Declaring the temporary variables 'res' and 'tmp' to be 32-bit
     * types while the return type of the function is a 16-bit type
     * forces both compilers to insert exactly one extui instruction
     * (or equivalent) to mask off the upper 16 bits. */

    __u32 res;
    __u32 tmp;

    __asm__("extui    %1, %2, 8, 8\n\t"
	    "slli     %0, %2, 8   \n\t"
	    "or       %0, %0, %1  \n"
	    : "=&a" (res), "=&a" (tmp)
	    : "a" (x)
	    );

    return res;
}
#define __arch_swab16 __arch_swab16

#endif /* _XTENSA_SWAB_H */
