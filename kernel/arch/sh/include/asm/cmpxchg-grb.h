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

#ifndef __ASM_SH_CMPXCHG_GRB_H
#define __ASM_SH_CMPXCHG_GRB_H

static inline unsigned long xchg_u32(volatile u32 *m, unsigned long val)
{
	unsigned long retval;

	__asm__ __volatile__ (
		"   .align 2              \n\t"
		"   mova    1f,   r0      \n\t" /* r0 = end point */
		"   nop                   \n\t"
		"   mov    r15,   r1      \n\t" /* r1 = saved sp */
		"   mov    #-4,   r15     \n\t" /* LOGIN */
		"   mov.l  @%1,   %0      \n\t" /* load  old value */
		"   mov.l   %2,   @%1     \n\t" /* store new value */
		"1: mov     r1,   r15     \n\t" /* LOGOUT */
		: "=&r" (retval),
		  "+r"  (m)
		: "r"   (val)
		: "memory", "r0", "r1");

	return retval;
}

static inline unsigned long xchg_u8(volatile u8 *m, unsigned long val)
{
	unsigned long retval;

	__asm__ __volatile__ (
		"   .align  2             \n\t"
		"   mova    1f,   r0      \n\t" /* r0 = end point */
		"   mov    r15,   r1      \n\t" /* r1 = saved sp */
		"   mov    #-6,   r15     \n\t" /* LOGIN */
		"   mov.b  @%1,   %0      \n\t" /* load  old value */
		"   extu.b  %0,   %0      \n\t" /* extend as unsigned */
		"   mov.b   %2,   @%1     \n\t" /* store new value */
		"1: mov     r1,   r15     \n\t" /* LOGOUT */
		: "=&r" (retval),
		  "+r"  (m)
		: "r"   (val)
		: "memory" , "r0", "r1");

	return retval;
}

static inline unsigned long __cmpxchg_u32(volatile int *m, unsigned long old,
					  unsigned long new)
{
	unsigned long retval;

	__asm__ __volatile__ (
		"   .align  2             \n\t"
		"   mova    1f,   r0      \n\t" /* r0 = end point */
		"   nop                   \n\t"
		"   mov    r15,   r1      \n\t" /* r1 = saved sp */
		"   mov    #-8,   r15     \n\t" /* LOGIN */
		"   mov.l  @%1,   %0      \n\t" /* load  old value */
		"   cmp/eq  %0,   %2      \n\t"
		"   bf            1f      \n\t" /* if not equal */
		"   mov.l   %2,   @%1     \n\t" /* store new value */
		"1: mov     r1,   r15     \n\t" /* LOGOUT */
		: "=&r" (retval),
		  "+r"  (m)
		: "r"   (new)
		: "memory" , "r0", "r1", "t");

	return retval;
}

#endif /* __ASM_SH_CMPXCHG_GRB_H */
