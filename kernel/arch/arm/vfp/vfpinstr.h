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
 *  linux/arch/arm/vfp/vfpinstr.h
 *
 *  Copyright (C) 2004 ARM Limited.
 *  Written by Deep Blue Solutions Limited.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * VFP instruction masks.
 */
#define INST_CPRTDO(inst)	(((inst) & 0x0f000000) == 0x0e000000)
#define INST_CPRT(inst)		((inst) & (1 << 4))
#define INST_CPRT_L(inst)	((inst) & (1 << 20))
#define INST_CPRT_Rd(inst)	(((inst) & (15 << 12)) >> 12)
#define INST_CPRT_OP(inst)	(((inst) >> 21) & 7)
#define INST_CPNUM(inst)	((inst) & 0xf00)
#define CPNUM(cp)		((cp) << 8)

#define FOP_MASK	(0x00b00040)
#define FOP_FMAC	(0x00000000)
#define FOP_FNMAC	(0x00000040)
#define FOP_FMSC	(0x00100000)
#define FOP_FNMSC	(0x00100040)
#define FOP_FMUL	(0x00200000)
#define FOP_FNMUL	(0x00200040)
#define FOP_FADD	(0x00300000)
#define FOP_FSUB	(0x00300040)
#define FOP_FDIV	(0x00800000)
#define FOP_EXT		(0x00b00040)

#define FOP_TO_IDX(inst)	((inst & 0x00b00000) >> 20 | (inst & (1 << 6)) >> 4)

#define FEXT_MASK	(0x000f0080)
#define FEXT_FCPY	(0x00000000)
#define FEXT_FABS	(0x00000080)
#define FEXT_FNEG	(0x00010000)
#define FEXT_FSQRT	(0x00010080)
#define FEXT_FCMP	(0x00040000)
#define FEXT_FCMPE	(0x00040080)
#define FEXT_FCMPZ	(0x00050000)
#define FEXT_FCMPEZ	(0x00050080)
#define FEXT_FCVT	(0x00070080)
#define FEXT_FUITO	(0x00080000)
#define FEXT_FSITO	(0x00080080)
#define FEXT_FTOUI	(0x000c0000)
#define FEXT_FTOUIZ	(0x000c0080)
#define FEXT_FTOSI	(0x000d0000)
#define FEXT_FTOSIZ	(0x000d0080)

#define FEXT_TO_IDX(inst)	((inst & 0x000f0000) >> 15 | (inst & (1 << 7)) >> 7)

#define vfp_get_sd(inst)	((inst & 0x0000f000) >> 11 | (inst & (1 << 22)) >> 22)
#define vfp_get_dd(inst)	((inst & 0x0000f000) >> 12 | (inst & (1 << 22)) >> 18)
#define vfp_get_sm(inst)	((inst & 0x0000000f) << 1 | (inst & (1 << 5)) >> 5)
#define vfp_get_dm(inst)	((inst & 0x0000000f) | (inst & (1 << 5)) >> 1)
#define vfp_get_sn(inst)	((inst & 0x000f0000) >> 15 | (inst & (1 << 7)) >> 7)
#define vfp_get_dn(inst)	((inst & 0x000f0000) >> 16 | (inst & (1 << 7)) >> 3)

#define vfp_single(inst)	(((inst) & 0x0000f00) == 0xa00)

#define FPSCR_N	(1 << 31)
#define FPSCR_Z	(1 << 30)
#define FPSCR_C (1 << 29)
#define FPSCR_V	(1 << 28)

/*
 * Since we aren't building with -mfpu=vfp, we need to code
 * these instructions using their MRC/MCR equivalents.
 */
#define vfpreg(_vfp_) #_vfp_

#define fmrx(_vfp_) ({			\
	u32 __v;			\
	asm("mrc p10, 7, %0, " vfpreg(_vfp_) ", cr0, 0 @ fmrx	%0, " #_vfp_	\
	    : "=r" (__v) : : "cc");	\
	__v;				\
 })

#define fmxr(_vfp_,_var_)		\
	asm("mcr p10, 7, %0, " vfpreg(_vfp_) ", cr0, 0 @ fmxr	" #_vfp_ ", %0"	\
	   : : "r" (_var_) : "cc")

u32 vfp_single_cpdo(u32 inst, u32 fpscr);
u32 vfp_single_cprt(u32 inst, u32 fpscr, struct pt_regs *regs);

u32 vfp_double_cpdo(u32 inst, u32 fpscr);
