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
 * Copyright (c) 1996, 1998, 1999, 2004 by Ralf Baechle
 * Copyright (c) 1999 Silicon Graphics, Inc.
 */
#include <asm/asm.h>
#include <asm/asm-offsets.h>
#include <asm/regdef.h>

#define EX(insn,reg,addr,handler)			\
9:	insn	reg, addr;				\
	.section __ex_table,"a";			\
	PTR	9b, handler;				\
	.previous

/*
 * Return the size of a string including the ending NUL character upto a
 * maximum of a1 or 0 in case of error.
 *
 * Note: for performance reasons we deliberately accept that a user may
 *       make strlen_user and strnlen_user access the first few KSEG0
 *       bytes.  There's nothing secret there.  On 64-bit accessing beyond
 *       the maximum is a tad hairier ...
 */
LEAF(__strnlen_user_asm)
	LONG_L		v0, TI_ADDR_LIMIT($28)	# pointer ok?
	and		v0, a0
	bnez		v0, .Lfault

FEXPORT(__strnlen_user_nocheck_asm)
	move		v0, a0
	PTR_ADDU	a1, a0			# stop pointer
1:	beq		v0, a1, 1f		# limit reached?
	EX(lb, t0, (v0), .Lfault)
	PTR_ADDU	v0, 1
	bnez		t0, 1b
1:	PTR_SUBU	v0, a0
	jr		ra
	END(__strnlen_user_asm)

.Lfault:
	move		v0, zero
	jr		ra
