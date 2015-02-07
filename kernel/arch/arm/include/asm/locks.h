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
 *  arch/arm/include/asm/locks.h
 *
 *  Copyright (C) 2000 Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  Interrupt safe locking assembler. 
 */
#ifndef __ASM_PROC_LOCKS_H
#define __ASM_PROC_LOCKS_H

#if __LINUX_ARM_ARCH__ >= 6

#define __down_op(ptr,fail)			\
	({					\
	__asm__ __volatile__(			\
	"@ down_op\n"				\
"1:	ldrex	lr, [%0]\n"			\
"	sub	lr, lr, %1\n"			\
"	strex	ip, lr, [%0]\n"			\
"	teq	ip, #0\n"			\
"	bne	1b\n"				\
"	teq	lr, #0\n"			\
"	movmi	ip, %0\n"			\
"	blmi	" #fail				\
	:					\
	: "r" (ptr), "I" (1)			\
	: "ip", "lr", "cc");			\
	smp_mb();				\
	})

#define __down_op_ret(ptr,fail)			\
	({					\
		unsigned int ret;		\
	__asm__ __volatile__(			\
	"@ down_op_ret\n"			\
"1:	ldrex	lr, [%1]\n"			\
"	sub	lr, lr, %2\n"			\
"	strex	ip, lr, [%1]\n"			\
"	teq	ip, #0\n"			\
"	bne	1b\n"				\
"	teq	lr, #0\n"			\
"	movmi	ip, %1\n"			\
"	movpl	ip, #0\n"			\
"	blmi	" #fail "\n"			\
"	mov	%0, ip"				\
	: "=&r" (ret)				\
	: "r" (ptr), "I" (1)			\
	: "ip", "lr", "cc");			\
	smp_mb();				\
	ret;					\
	})

#define __up_op(ptr,wake)			\
	({					\
	smp_mb();				\
	__asm__ __volatile__(			\
	"@ up_op\n"				\
"1:	ldrex	lr, [%0]\n"			\
"	add	lr, lr, %1\n"			\
"	strex	ip, lr, [%0]\n"			\
"	teq	ip, #0\n"			\
"	bne	1b\n"				\
"	cmp	lr, #0\n"			\
"	movle	ip, %0\n"			\
"	blle	" #wake				\
	:					\
	: "r" (ptr), "I" (1)			\
	: "ip", "lr", "cc");			\
	})

/*
 * The value 0x01000000 supports up to 128 processors and
 * lots of processes.  BIAS must be chosen such that sub'ing
 * BIAS once per CPU will result in the long remaining
 * negative.
 */
#define RW_LOCK_BIAS      0x01000000
#define RW_LOCK_BIAS_STR "0x01000000"

#define __down_op_write(ptr,fail)		\
	({					\
	__asm__ __volatile__(			\
	"@ down_op_write\n"			\
"1:	ldrex	lr, [%0]\n"			\
"	sub	lr, lr, %1\n"			\
"	strex	ip, lr, [%0]\n"			\
"	teq	ip, #0\n"			\
"	bne	1b\n"				\
"	teq	lr, #0\n"			\
"	movne	ip, %0\n"			\
"	blne	" #fail				\
	:					\
	: "r" (ptr), "I" (RW_LOCK_BIAS)		\
	: "ip", "lr", "cc");			\
	smp_mb();				\
	})

#define __up_op_write(ptr,wake)			\
	({					\
	smp_mb();				\
	__asm__ __volatile__(			\
	"@ up_op_write\n"			\
"1:	ldrex	lr, [%0]\n"			\
"	adds	lr, lr, %1\n"			\
"	strex	ip, lr, [%0]\n"			\
"	teq	ip, #0\n"			\
"	bne	1b\n"				\
"	movcs	ip, %0\n"			\
"	blcs	" #wake				\
	:					\
	: "r" (ptr), "I" (RW_LOCK_BIAS)		\
	: "ip", "lr", "cc");			\
	})

#define __down_op_read(ptr,fail)		\
	__down_op(ptr, fail)

#define __up_op_read(ptr,wake)			\
	({					\
	smp_mb();				\
	__asm__ __volatile__(			\
	"@ up_op_read\n"			\
"1:	ldrex	lr, [%0]\n"			\
"	add	lr, lr, %1\n"			\
"	strex	ip, lr, [%0]\n"			\
"	teq	ip, #0\n"			\
"	bne	1b\n"				\
"	teq	lr, #0\n"			\
"	moveq	ip, %0\n"			\
"	bleq	" #wake				\
	:					\
	: "r" (ptr), "I" (1)			\
	: "ip", "lr", "cc");			\
	})

#else

#define __down_op(ptr,fail)			\
	({					\
	__asm__ __volatile__(			\
	"@ down_op\n"				\
"	mrs	ip, cpsr\n"			\
"	orr	lr, ip, #128\n"			\
"	msr	cpsr_c, lr\n"			\
"	ldr	lr, [%0]\n"			\
"	subs	lr, lr, %1\n"			\
"	str	lr, [%0]\n"			\
"	msr	cpsr_c, ip\n"			\
"	movmi	ip, %0\n"			\
"	blmi	" #fail				\
	:					\
	: "r" (ptr), "I" (1)			\
	: "ip", "lr", "cc");			\
	smp_mb();				\
	})

#define __down_op_ret(ptr,fail)			\
	({					\
		unsigned int ret;		\
	__asm__ __volatile__(			\
	"@ down_op_ret\n"			\
"	mrs	ip, cpsr\n"			\
"	orr	lr, ip, #128\n"			\
"	msr	cpsr_c, lr\n"			\
"	ldr	lr, [%1]\n"			\
"	subs	lr, lr, %2\n"			\
"	str	lr, [%1]\n"			\
"	msr	cpsr_c, ip\n"			\
"	movmi	ip, %1\n"			\
"	movpl	ip, #0\n"			\
"	blmi	" #fail "\n"			\
"	mov	%0, ip"				\
	: "=&r" (ret)				\
	: "r" (ptr), "I" (1)			\
	: "ip", "lr", "cc");			\
	smp_mb();				\
	ret;					\
	})

#define __up_op(ptr,wake)			\
	({					\
	smp_mb();				\
	__asm__ __volatile__(			\
	"@ up_op\n"				\
"	mrs	ip, cpsr\n"			\
"	orr	lr, ip, #128\n"			\
"	msr	cpsr_c, lr\n"			\
"	ldr	lr, [%0]\n"			\
"	adds	lr, lr, %1\n"			\
"	str	lr, [%0]\n"			\
"	msr	cpsr_c, ip\n"			\
"	movle	ip, %0\n"			\
"	blle	" #wake				\
	:					\
	: "r" (ptr), "I" (1)			\
	: "ip", "lr", "cc");			\
	})

/*
 * The value 0x01000000 supports up to 128 processors and
 * lots of processes.  BIAS must be chosen such that sub'ing
 * BIAS once per CPU will result in the long remaining
 * negative.
 */
#define RW_LOCK_BIAS      0x01000000
#define RW_LOCK_BIAS_STR "0x01000000"

#define __down_op_write(ptr,fail)		\
	({					\
	__asm__ __volatile__(			\
	"@ down_op_write\n"			\
"	mrs	ip, cpsr\n"			\
"	orr	lr, ip, #128\n"			\
"	msr	cpsr_c, lr\n"			\
"	ldr	lr, [%0]\n"			\
"	subs	lr, lr, %1\n"			\
"	str	lr, [%0]\n"			\
"	msr	cpsr_c, ip\n"			\
"	movne	ip, %0\n"			\
"	blne	" #fail				\
	:					\
	: "r" (ptr), "I" (RW_LOCK_BIAS)		\
	: "ip", "lr", "cc");			\
	smp_mb();				\
	})

#define __up_op_write(ptr,wake)			\
	({					\
	__asm__ __volatile__(			\
	"@ up_op_write\n"			\
"	mrs	ip, cpsr\n"			\
"	orr	lr, ip, #128\n"			\
"	msr	cpsr_c, lr\n"			\
"	ldr	lr, [%0]\n"			\
"	adds	lr, lr, %1\n"			\
"	str	lr, [%0]\n"			\
"	msr	cpsr_c, ip\n"			\
"	movcs	ip, %0\n"			\
"	blcs	" #wake				\
	:					\
	: "r" (ptr), "I" (RW_LOCK_BIAS)		\
	: "ip", "lr", "cc");			\
	smp_mb();				\
	})

#define __down_op_read(ptr,fail)		\
	__down_op(ptr, fail)

#define __up_op_read(ptr,wake)			\
	({					\
	smp_mb();				\
	__asm__ __volatile__(			\
	"@ up_op_read\n"			\
"	mrs	ip, cpsr\n"			\
"	orr	lr, ip, #128\n"			\
"	msr	cpsr_c, lr\n"			\
"	ldr	lr, [%0]\n"			\
"	adds	lr, lr, %1\n"			\
"	str	lr, [%0]\n"			\
"	msr	cpsr_c, ip\n"			\
"	moveq	ip, %0\n"			\
"	bleq	" #wake				\
	:					\
	: "r" (ptr), "I" (1)			\
	: "ip", "lr", "cc");			\
	})

#endif

#endif
