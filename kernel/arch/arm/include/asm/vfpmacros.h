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
 * arch/arm/include/asm/vfpmacros.h
 *
 * Assembler-only file containing VFP macros and register definitions.
 */
#include "vfp.h"

@ Macros to allow building with old toolkits (with no VFP support)
	.macro	VFPFMRX, rd, sysreg, cond
	MRC\cond	p10, 7, \rd, \sysreg, cr0, 0	@ FMRX	\rd, \sysreg
	.endm

	.macro	VFPFMXR, sysreg, rd, cond
	MCR\cond	p10, 7, \rd, \sysreg, cr0, 0	@ FMXR	\sysreg, \rd
	.endm

	@ read all the working registers back into the VFP
	.macro	VFPFLDMIA, base, tmp
#if __LINUX_ARM_ARCH__ < 6
	LDC	p11, cr0, [\base],#33*4		    @ FLDMIAX \base!, {d0-d15}
#else
	LDC	p11, cr0, [\base],#32*4		    @ FLDMIAD \base!, {d0-d15}
#endif
#ifdef CONFIG_VFPv3
	VFPFMRX	\tmp, MVFR0			    @ Media and VFP Feature Register 0
	and	\tmp, \tmp, #MVFR0_A_SIMD_MASK	    @ A_SIMD field
	cmp	\tmp, #2			    @ 32 x 64bit registers?
	ldceql	p11, cr0, [\base],#32*4		    @ FLDMIAD \base!, {d16-d31}
	addne	\base, \base, #32*4		    @ step over unused register space
#endif
	.endm

	@ write all the working registers out of the VFP
	.macro	VFPFSTMIA, base, tmp
#if __LINUX_ARM_ARCH__ < 6
	STC	p11, cr0, [\base],#33*4		    @ FSTMIAX \base!, {d0-d15}
#else
	STC	p11, cr0, [\base],#32*4		    @ FSTMIAD \base!, {d0-d15}
#endif
#ifdef CONFIG_VFPv3
	VFPFMRX	\tmp, MVFR0			    @ Media and VFP Feature Register 0
	and	\tmp, \tmp, #MVFR0_A_SIMD_MASK	    @ A_SIMD field
	cmp	\tmp, #2			    @ 32 x 64bit registers?
	stceql	p11, cr0, [\base],#32*4		    @ FSTMIAD \base!, {d16-d31}
	addne	\base, \base, #32*4		    @ step over unused register space
#endif
	.endm
