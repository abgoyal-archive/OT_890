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
 *  linux/arch/arm/lib/getuser.S
 *
 *  Copyright (C) 2001 Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  Idea from x86 version, (C) Copyright 1998 Linus Torvalds
 *
 * These functions have a non-standard call interface to make them more
 * efficient, especially as they return an error value in addition to
 * the "real" return value.
 *
 * __get_user_X
 *
 * Inputs:	r0 contains the address
 * Outputs:	r0 is the error code
 *		r2, r3 contains the zero-extended value
 *		lr corrupted
 *
 * No other registers must be altered.  (see <asm/uaccess.h>
 * for specific ASM register usage).
 *
 * Note that ADDR_LIMIT is either 0 or 0xc0000000.
 * Note also that it is intended that __get_user_bad is not global.
 */
#include <linux/linkage.h>
#include <asm/errno.h>

ENTRY(__get_user_1)
1:	ldrbt	r2, [r0]
	mov	r0, #0
	mov	pc, lr
ENDPROC(__get_user_1)

ENTRY(__get_user_2)
2:	ldrbt	r2, [r0], #1
3:	ldrbt	r3, [r0]
#ifndef __ARMEB__
	orr	r2, r2, r3, lsl #8
#else
	orr	r2, r3, r2, lsl #8
#endif
	mov	r0, #0
	mov	pc, lr
ENDPROC(__get_user_2)

ENTRY(__get_user_4)
4:	ldrt	r2, [r0]
	mov	r0, #0
	mov	pc, lr
ENDPROC(__get_user_4)

__get_user_bad:
	mov	r2, #0
	mov	r0, #-EFAULT
	mov	pc, lr
ENDPROC(__get_user_bad)

.section __ex_table, "a"
	.long	1b, __get_user_bad
	.long	2b, __get_user_bad
	.long	3b, __get_user_bad
	.long	4b, __get_user_bad
.previous
