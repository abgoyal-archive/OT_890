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

#ifndef __ASM_SH_CMPXCHG_LLSC_H
#define __ASM_SH_CMPXCHG_LLSC_H

static inline unsigned long xchg_u32(volatile u32 *m, unsigned long val)
{
	unsigned long retval;
	unsigned long tmp;

	__asm__ __volatile__ (
		"1:					\n\t"
		"movli.l	@%1, %0	! xchg_u32	\n\t"
		"mov		%0, %2			\n\t"
		"mov		%4, %0			\n\t"
		"movco.l	%0, @%1			\n\t"
		"bf		1b			\n\t"
		"synco					\n\t"
		: "=&z"(tmp), "=r" (m), "=&r" (retval)
		: "1" (m), "r" (val)
		: "t", "memory"
	);

	return retval;
}

static inline unsigned long xchg_u8(volatile u8 *m, unsigned long val)
{
	unsigned long retval;
	unsigned long tmp;

	__asm__ __volatile__ (
		"1:					\n\t"
		"movli.l	@%1, %0	! xchg_u8	\n\t"
		"mov		%0, %2			\n\t"
		"mov		%4, %0			\n\t"
		"movco.l	%0, @%1			\n\t"
		"bf		1b			\n\t"
		"synco					\n\t"
		: "=&z"(tmp), "=r" (m), "=&r" (retval)
		: "1" (m), "r" (val & 0xff)
		: "t", "memory"
	);

	return retval;
}

static inline unsigned long
__cmpxchg_u32(volatile int *m, unsigned long old, unsigned long new)
{
	unsigned long retval;
	unsigned long tmp;

	__asm__ __volatile__ (
		"1:						\n\t"
		"movli.l	@%1, %0	! __cmpxchg_u32		\n\t"
		"mov		%0, %2				\n\t"
		"cmp/eq		%2, %4				\n\t"
		"bf		2f				\n\t"
		"mov		%5, %0				\n\t"
		"2:						\n\t"
		"movco.l	%0, @%1				\n\t"
		"bf		1b				\n\t"
		"synco						\n\t"
		: "=&z" (tmp), "=r" (m), "=&r" (retval)
		: "1" (m), "r" (old), "r" (new)
		: "t", "memory"
	);

	return retval;
}

#endif /* __ASM_SH_CMPXCHG_LLSC_H */
