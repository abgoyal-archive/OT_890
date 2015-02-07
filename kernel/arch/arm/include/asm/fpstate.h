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
 *  arch/arm/include/asm/fpstate.h
 *
 *  Copyright (C) 1995 Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __ASM_ARM_FPSTATE_H
#define __ASM_ARM_FPSTATE_H


#ifndef __ASSEMBLY__

/*
 * VFP storage area has:
 *  - FPEXC, FPSCR, FPINST and FPINST2.
 *  - 16 or 32 double precision data registers
 *  - an implementation-dependant word of state for FLDMX/FSTMX (pre-ARMv6)
 * 
 *  FPEXC will always be non-zero once the VFP has been used in this process.
 */

struct vfp_hard_struct {
#ifdef CONFIG_VFPv3
	__u64 fpregs[32];
#else
	__u64 fpregs[16];
#endif
#if __LINUX_ARM_ARCH__ < 6
	__u32 fpmx_state;
#endif
	__u32 fpexc;
	__u32 fpscr;
	/*
	 * VFP implementation specific state
	 */
	__u32 fpinst;
	__u32 fpinst2;

#ifdef CONFIG_SMP
	__u32 cpu;
#endif
};

union vfp_state {
	struct vfp_hard_struct	hard;
};

extern void vfp_flush_thread(union vfp_state *);
extern void vfp_release_thread(union vfp_state *);

#define FP_HARD_SIZE 35

struct fp_hard_struct {
	unsigned int save[FP_HARD_SIZE];		/* as yet undefined */
};

#define FP_SOFT_SIZE 35

struct fp_soft_struct {
	unsigned int save[FP_SOFT_SIZE];		/* undefined information */
};

#define IWMMXT_SIZE	0x98

struct iwmmxt_struct {
	unsigned int save[IWMMXT_SIZE / sizeof(unsigned int)];
};

union fp_state {
	struct fp_hard_struct	hard;
	struct fp_soft_struct	soft;
#ifdef CONFIG_IWMMXT
	struct iwmmxt_struct	iwmmxt;
#endif
};

#define FP_SIZE (sizeof(union fp_state) / sizeof(int))

struct crunch_state {
	unsigned int	mvdx[16][2];
	unsigned int	mvax[4][3];
	unsigned int	dspsc[2];
};

#define CRUNCH_SIZE	sizeof(struct crunch_state)

#endif

#endif
