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
 *  linux/arch/arm/lib/delay.S
 *
 *  Copyright (C) 1995, 1996 Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/linkage.h>
#include <asm/assembler.h>
#include <asm/param.h>
		.text

.LC0:		.word	loops_per_jiffy
.LC1:		.word	(2199023*HZ)>>11

/*
 * r0  <= 2000
 * lpj <= 0x01ffffff (max. 3355 bogomips)
 * HZ  <= 1000
 */

ENTRY(__udelay)
		ldr	r2, .LC1
		mul	r0, r2, r0
ENTRY(__const_udelay)				@ 0 <= r0 <= 0x7fffff06
		ldr	r2, .LC0
		ldr	r2, [r2]		@ max = 0x01ffffff
		mov	r0, r0, lsr #14		@ max = 0x0001ffff
		mov	r2, r2, lsr #10		@ max = 0x00007fff
		mul	r0, r2, r0		@ max = 2^32-1
		movs	r0, r0, lsr #6
		moveq	pc, lr

/*
 * loops = r0 * HZ * loops_per_jiffy / 1000000
 *
 * Oh, if only we had a cycle counter...
 */

@ Delay routine
ENTRY(__delay)
		subs	r0, r0, #1
#if 0
		movls	pc, lr
		subs	r0, r0, #1
		movls	pc, lr
		subs	r0, r0, #1
		movls	pc, lr
		subs	r0, r0, #1
		movls	pc, lr
		subs	r0, r0, #1
		movls	pc, lr
		subs	r0, r0, #1
		movls	pc, lr
		subs	r0, r0, #1
		movls	pc, lr
		subs	r0, r0, #1
#endif
		bhi	__delay
		mov	pc, lr
ENDPROC(__udelay)
ENDPROC(__const_udelay)
ENDPROC(__delay)
