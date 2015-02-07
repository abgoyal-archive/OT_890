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
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (c) 1996, 1999 by Ralf Baechle
 */
#include <linux/errno.h>
#include <asm/asm.h>
#include <asm/asm-offsets.h>
#include <asm/regdef.h>

#define EX(insn,reg,addr,handler)			\
9:	insn	reg, addr;				\
	.section __ex_table,"a";			\
	PTR	9b, handler;				\
	.previous

/*
 * Returns: -EFAULT if exception before terminator, N if the entire
 * buffer filled, else strlen.
 */

/*
 * Ugly special case have to check: we might get passed a user space
 * pointer which wraps into the kernel space.  We don't deal with that.  If
 * it happens at most some bytes of the exceptions handlers will be copied.
 */

LEAF(__strncpy_from_user_asm)
	LONG_L		v0, TI_ADDR_LIMIT($28)	# pointer ok?
	and		v0, a1
	bnez		v0, .Lfault

FEXPORT(__strncpy_from_user_nocheck_asm)
	move		v0, zero
	move		v1, a1
	.set		noreorder
1:	EX(lbu, t0, (v1), .Lfault)
	PTR_ADDIU	v1, 1
	R10KCBARRIER(0(ra))
	beqz		t0, 2f
	 sb		t0, (a0)
	PTR_ADDIU	v0, 1
	.set		reorder
	PTR_ADDIU	a0, 1
	bne		v0, a2, 1b
2:	PTR_ADDU	t0, a1, v0
	xor		t0, a1
	bltz		t0, .Lfault
	jr		ra			# return n
	END(__strncpy_from_user_asm)

.Lfault:	li		v0, -EFAULT
	jr		ra

	.section	__ex_table,"a"
	PTR		1b, .Lfault
	.previous
