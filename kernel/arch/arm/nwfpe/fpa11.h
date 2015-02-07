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
    NetWinder Floating Point Emulator
    (c) Rebel.com, 1998-1999
    
    Direct questions, comments to Scott Bambrough <scottb@netwinder.org>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifndef __FPA11_H__
#define __FPA11_H__

#define GET_FPA11() ((FPA11 *)(&current_thread_info()->fpstate))

/*
 * The processes registers are always at the very top of the 8K
 * stack+task struct.  Use the same method as 'current' uses to
 * reach them.
 */
#define GET_USERREG() ((struct pt_regs *)(THREAD_START_SP + (unsigned long)current_thread_info()) - 1)

#include <linux/thread_info.h>

/* includes */
#include "fpsr.h"		/* FP control and status register definitions */
#include "milieu.h"

struct roundingData {
    int8 mode;
    int8 precision;
    signed char exception;
};

#include "softfloat.h"

#define		typeNone		0x00
#define		typeSingle		0x01
#define		typeDouble		0x02
#define		typeExtended		0x03

/*
 * This must be no more and no less than 12 bytes.
 */
typedef union tagFPREG {
	float32 fSingle;
	float64 fDouble;
#ifdef CONFIG_FPE_NWFPE_XP
	floatx80 fExtended;
#else
	u32 padding[3];
#endif
} __attribute__ ((packed,aligned(4))) FPREG;

/*
 * FPA11 device model.
 *
 * This structure is exported to user space.  Do not re-order.
 * Only add new stuff to the end, and do not change the size of
 * any element.  Elements of this structure are used by user
 * space, and must match struct user_fp in <asm/user.h>.
 * We include the byte offsets below for documentation purposes.
 *
 * The size of this structure and FPREG are checked by fpmodule.c
 * on initialisation.  If the rules have been broken, NWFPE will
 * not initialise.
 */
typedef struct tagFPA11 {
/*   0 */ FPREG fpreg[8];	/* 8 floating point registers */
/*  96 */ FPSR fpsr;		/* floating point status register */
/* 100 */ FPCR fpcr;		/* floating point control register */
/* 104 */ unsigned char fType[8];	/* type of floating point value held in
					   floating point registers.  One of
					   none, single, double or extended. */
/* 112 */ int initflag;		/* this is special.  The kernel guarantees
				   to set it to 0 when a thread is launched,
				   so we can use it to detect whether this
				   instance of the emulator needs to be
				   initialised. */
} __attribute__ ((packed,aligned(4))) FPA11;

extern int8 SetRoundingMode(const unsigned int);
extern int8 SetRoundingPrecision(const unsigned int);
extern void nwfpe_init_fpa(union fp_state *fp);

extern unsigned int EmulateAll(unsigned int opcode);

extern unsigned int EmulateCPDT(const unsigned int opcode);
extern unsigned int EmulateCPDO(const unsigned int opcode);
extern unsigned int EmulateCPRT(const unsigned int opcode);

/* fpa11_cpdt.c */
extern unsigned int PerformLDF(const unsigned int opcode);
extern unsigned int PerformSTF(const unsigned int opcode);
extern unsigned int PerformLFM(const unsigned int opcode);
extern unsigned int PerformSFM(const unsigned int opcode);

/* single_cpdo.c */

extern unsigned int SingleCPDO(struct roundingData *roundData,
			       const unsigned int opcode, FPREG * rFd);
/* double_cpdo.c */
extern unsigned int DoubleCPDO(struct roundingData *roundData,
			       const unsigned int opcode, FPREG * rFd);

#endif
