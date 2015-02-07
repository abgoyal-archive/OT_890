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

#ifndef __ASM_SH_BITOPS_GRB_H
#define __ASM_SH_BITOPS_GRB_H

static inline void set_bit(int nr, volatile void * addr)
{
	int	mask;
	volatile unsigned int *a = addr;
	unsigned long tmp;

	a += nr >> 5;
	mask = 1 << (nr & 0x1f);

        __asm__ __volatile__ (
                "   .align 2              \n\t"
                "   mova    1f,   r0      \n\t" /* r0 = end point */
                "   mov    r15,   r1      \n\t" /* r1 = saved sp */
                "   mov    #-6,   r15     \n\t" /* LOGIN: r15 = size */
                "   mov.l  @%1,   %0      \n\t" /* load  old value */
                "   or      %2,   %0      \n\t" /* or */
                "   mov.l   %0,   @%1     \n\t" /* store new value */
                "1: mov     r1,   r15     \n\t" /* LOGOUT */
                : "=&r" (tmp),
                  "+r"  (a)
                : "r"   (mask)
                : "memory" , "r0", "r1");
}

static inline void clear_bit(int nr, volatile void * addr)
{
	int	mask;
	volatile unsigned int *a = addr;
        unsigned long tmp;

	a += nr >> 5;
        mask = ~(1 << (nr & 0x1f));
        __asm__ __volatile__ (
                "   .align 2              \n\t"
                "   mova    1f,   r0      \n\t" /* r0 = end point */
                "   mov    r15,   r1      \n\t" /* r1 = saved sp */
                "   mov    #-6,   r15     \n\t" /* LOGIN: r15 = size */
                "   mov.l  @%1,   %0      \n\t" /* load  old value */
                "   and     %2,   %0      \n\t" /* and */
                "   mov.l   %0,   @%1     \n\t" /* store new value */
                "1: mov     r1,   r15     \n\t" /* LOGOUT */
                : "=&r" (tmp),
                  "+r"  (a)
                : "r"   (mask)
                : "memory" , "r0", "r1");
}

static inline void change_bit(int nr, volatile void * addr)
{
        int     mask;
        volatile unsigned int *a = addr;
        unsigned long tmp;

        a += nr >> 5;
        mask = 1 << (nr & 0x1f);
        __asm__ __volatile__ (
                "   .align 2              \n\t"
                "   mova    1f,   r0      \n\t" /* r0 = end point */
                "   mov    r15,   r1      \n\t" /* r1 = saved sp */
                "   mov    #-6,   r15     \n\t" /* LOGIN: r15 = size */
                "   mov.l  @%1,   %0      \n\t" /* load  old value */
                "   xor     %2,   %0      \n\t" /* xor */
                "   mov.l   %0,   @%1     \n\t" /* store new value */
                "1: mov     r1,   r15     \n\t" /* LOGOUT */
                : "=&r" (tmp),
                  "+r"  (a)
                : "r"   (mask)
                : "memory" , "r0", "r1");
}

static inline int test_and_set_bit(int nr, volatile void * addr)
{
        int     mask, retval;
	volatile unsigned int *a = addr;
        unsigned long tmp;

	a += nr >> 5;
	mask = 1 << (nr & 0x1f);

        __asm__ __volatile__ (
                "   .align 2              \n\t"
                "   mova    1f,   r0      \n\t" /* r0 = end point */
                "   mov    r15,   r1      \n\t" /* r1 = saved sp */
                "   mov   #-14,   r15     \n\t" /* LOGIN: r15 = size */
                "   mov.l  @%2,   %0      \n\t" /* load old value */
                "   mov     %0,   %1      \n\t"
                "   tst     %1,   %3      \n\t" /* T = ((*a & mask) == 0) */
                "   mov    #-1,   %1      \n\t" /* retvat = -1 */
                "   negc    %1,   %1      \n\t" /* retval = (mask & *a) != 0 */
                "   or      %3,   %0      \n\t"
                "   mov.l   %0,  @%2      \n\t" /* store new value */
                "1: mov     r1,  r15      \n\t" /* LOGOUT */
                : "=&r" (tmp),
                  "=&r" (retval),
                  "+r"  (a)
                : "r"   (mask)
                : "memory" , "r0", "r1" ,"t");

        return retval;
}

static inline int test_and_clear_bit(int nr, volatile void * addr)
{
        int     mask, retval,not_mask;
        volatile unsigned int *a = addr;
        unsigned long tmp;

        a += nr >> 5;
        mask = 1 << (nr & 0x1f);

	not_mask = ~mask;

        __asm__ __volatile__ (
                "   .align 2              \n\t"
		"   mova    1f,   r0      \n\t" /* r0 = end point */
                "   mov    r15,   r1      \n\t" /* r1 = saved sp */
		"   mov   #-14,   r15     \n\t" /* LOGIN */
		"   mov.l  @%2,   %0      \n\t" /* load old value */
                "   mov     %0,   %1      \n\t" /* %1 = *a */
                "   tst     %1,   %3      \n\t" /* T = ((*a & mask) == 0) */
		"   mov    #-1,   %1      \n\t" /* retvat = -1 */
                "   negc    %1,   %1      \n\t" /* retval = (mask & *a) != 0 */
                "   and     %4,   %0      \n\t"
                "   mov.l   %0,  @%2      \n\t" /* store new value */
		"1: mov     r1,   r15     \n\t" /* LOGOUT */
		: "=&r" (tmp),
		  "=&r" (retval),
		  "+r"  (a)
		: "r"   (mask),
		  "r"   (not_mask)
		: "memory" , "r0", "r1", "t");

        return retval;
}

static inline int test_and_change_bit(int nr, volatile void * addr)
{
        int     mask, retval;
        volatile unsigned int *a = addr;
        unsigned long tmp;

        a += nr >> 5;
        mask = 1 << (nr & 0x1f);

        __asm__ __volatile__ (
                "   .align 2              \n\t"
                "   mova    1f,   r0      \n\t" /* r0 = end point */
                "   mov    r15,   r1      \n\t" /* r1 = saved sp */
                "   mov   #-14,   r15     \n\t" /* LOGIN */
                "   mov.l  @%2,   %0      \n\t" /* load old value */
                "   mov     %0,   %1      \n\t" /* %1 = *a */
                "   tst     %1,   %3      \n\t" /* T = ((*a & mask) == 0) */
                "   mov    #-1,   %1      \n\t" /* retvat = -1 */
                "   negc    %1,   %1      \n\t" /* retval = (mask & *a) != 0 */
                "   xor     %3,   %0      \n\t"
                "   mov.l   %0,  @%2      \n\t" /* store new value */
                "1: mov     r1,   r15     \n\t" /* LOGOUT */
                : "=&r" (tmp),
                  "=&r" (retval),
                  "+r"  (a)
                : "r"   (mask)
                : "memory" , "r0", "r1", "t");

        return retval;
}

#include <asm-generic/bitops/non-atomic.h>

#endif /* __ASM_SH_BITOPS_GRB_H */
