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
 *  linux/arch/arm/lib/csumpartialcopyuser.S
 *
 *  Copyright (C) 1995-1998 Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * 27/03/03 Ian Molton Clean up CONFIG_CPU
 *
 */
#include <linux/linkage.h>
#include <asm/assembler.h>
#include <asm/errno.h>
#include <asm/asm-offsets.h>

		.text

		.macro	save_regs
		stmfd	sp!, {r1, r2, r4 - r8, lr}
		.endm

		.macro	load_regs
		ldmfd	sp!, {r1, r2, r4 - r8, pc}
		.endm

		.macro	load1b,	reg1
9999:		ldrbt	\reg1, [r0], $1
		.section __ex_table, "a"
		.align	3
		.long	9999b, 6001f
		.previous
		.endm

		.macro	load2b, reg1, reg2
9999:		ldrbt	\reg1, [r0], $1
9998:		ldrbt	\reg2, [r0], $1
		.section __ex_table, "a"
		.long	9999b, 6001f
		.long	9998b, 6001f
		.previous
		.endm

		.macro	load1l, reg1
9999:		ldrt	\reg1, [r0], $4
		.section __ex_table, "a"
		.align	3
		.long	9999b, 6001f
		.previous
		.endm

		.macro	load2l, reg1, reg2
9999:		ldrt	\reg1, [r0], $4
9998:		ldrt	\reg2, [r0], $4
		.section __ex_table, "a"
		.long	9999b, 6001f
		.long	9998b, 6001f
		.previous
		.endm

		.macro	load4l, reg1, reg2, reg3, reg4
9999:		ldrt	\reg1, [r0], $4
9998:		ldrt	\reg2, [r0], $4
9997:		ldrt	\reg3, [r0], $4
9996:		ldrt	\reg4, [r0], $4
		.section __ex_table, "a"
		.long	9999b, 6001f
		.long	9998b, 6001f
		.long	9997b, 6001f
		.long	9996b, 6001f
		.previous
		.endm

/*
 * unsigned int
 * csum_partial_copy_from_user(const char *src, char *dst, int len, int sum, int *err_ptr)
 *  r0 = src, r1 = dst, r2 = len, r3 = sum, [sp] = *err_ptr
 *  Returns : r0 = checksum, [[sp, #0], #0] = 0 or -EFAULT
 */

#define FN_ENTRY	ENTRY(csum_partial_copy_from_user)
#define FN_EXIT		ENDPROC(csum_partial_copy_from_user)

#include "csumpartialcopygeneric.S"

/*
 * FIXME: minor buglet here
 * We don't return the checksum for the data present in the buffer.  To do
 * so properly, we would have to add in whatever registers were loaded before
 * the fault, which, with the current asm above is not predictable.
 */
		.section .fixup,"ax"
		.align	4
6001:		mov	r4, #-EFAULT
		ldr	r5, [fp, #4]		@ *err_ptr
		str	r4, [r5]
		ldmia	sp, {r1, r2}		@ retrieve dst, len
		add	r2, r2, r1
		mov	r0, #0			@ zero the buffer
6002:		teq	r2, r1
		strneb	r0, [r1], #1
		bne	6002b
		load_regs
		.previous
