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
 *	arch/mips/dec/prom/call_o32.S
 *
 *	O32 interface for the 64 (or N32) ABI.
 *
 *	Copyright (C) 2002  Maciej W. Rozycki
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License
 *	as published by the Free Software Foundation; either version
 *	2 of the License, or (at your option) any later version.
 */

#include <asm/asm.h>
#include <asm/regdef.h>

/* Maximum number of arguments supported.  Must be even!  */
#define O32_ARGC	32
/* Number of static registers we save.  */
#define O32_STATC	11
/* Frame size for static register  */
#define O32_FRAMESZ	(SZREG * O32_STATC)
/* Frame size on new stack */
#define O32_FRAMESZ_NEW (SZREG + 4 * O32_ARGC)

		.text

/*
 * O32 function call dispatcher, for interfacing 32-bit ROM routines.
 *
 * The standard 64 (N32) calling sequence is supported, with a0
 * holding a function pointer, a1 a new stack pointer, a2-a7 -- its
 * first six arguments and the stack -- remaining ones (up to O32_ARGC,
 * including a2-a7). Static registers, gp and fp are preserved, v0 holds
 * a result. This code relies on the called o32 function for sp and ra
 * restoration and this dispatcher has to be placed in a KSEGx (or KUSEG)
 * address space.  Any pointers passed have to point to addresses within
 * one of these spaces as well.
 */
NESTED(call_o32, O32_FRAMESZ, ra)
		REG_SUBU	sp,O32_FRAMESZ

		REG_S		ra,O32_FRAMESZ-1*SZREG(sp)
		REG_S		fp,O32_FRAMESZ-2*SZREG(sp)
		REG_S		gp,O32_FRAMESZ-3*SZREG(sp)
		REG_S		s7,O32_FRAMESZ-4*SZREG(sp)
		REG_S		s6,O32_FRAMESZ-5*SZREG(sp)
		REG_S		s5,O32_FRAMESZ-6*SZREG(sp)
		REG_S		s4,O32_FRAMESZ-7*SZREG(sp)
		REG_S		s3,O32_FRAMESZ-8*SZREG(sp)
		REG_S		s2,O32_FRAMESZ-9*SZREG(sp)
		REG_S		s1,O32_FRAMESZ-10*SZREG(sp)
		REG_S		s0,O32_FRAMESZ-11*SZREG(sp)

		move		jp,a0
		REG_SUBU	s0,a1,O32_FRAMESZ_NEW
		REG_S		sp,O32_FRAMESZ_NEW-1*SZREG(s0)

		sll		a0,a2,zero
		sll		a1,a3,zero
		sll		a2,a4,zero
		sll		a3,a5,zero
		sw		a6,0x10(s0)
		sw		a7,0x14(s0)

		PTR_LA		t0,O32_FRAMESZ(sp)
		PTR_LA		t1,0x18(s0)
		li		t2,O32_ARGC-6
1:
		lw		t3,(t0)
		REG_ADDU	t0,SZREG
		sw		t3,(t1)
		REG_SUBU	t2,1
		REG_ADDU	t1,4
		bnez		t2,1b

		move		sp,s0

		jalr		jp

		REG_L		sp,O32_FRAMESZ_NEW-1*SZREG(sp)

		REG_L		s0,O32_FRAMESZ-11*SZREG(sp)
		REG_L		s1,O32_FRAMESZ-10*SZREG(sp)
		REG_L		s2,O32_FRAMESZ-9*SZREG(sp)
		REG_L		s3,O32_FRAMESZ-8*SZREG(sp)
		REG_L		s4,O32_FRAMESZ-7*SZREG(sp)
		REG_L		s5,O32_FRAMESZ-6*SZREG(sp)
		REG_L		s6,O32_FRAMESZ-5*SZREG(sp)
		REG_L		s7,O32_FRAMESZ-4*SZREG(sp)
		REG_L		gp,O32_FRAMESZ-3*SZREG(sp)
		REG_L		fp,O32_FRAMESZ-2*SZREG(sp)
		REG_L		ra,O32_FRAMESZ-1*SZREG(sp)

		REG_ADDU	sp,O32_FRAMESZ
		jr		ra
END(call_o32)
