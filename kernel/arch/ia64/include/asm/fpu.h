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

#ifndef _ASM_IA64_FPU_H
#define _ASM_IA64_FPU_H

/*
 * Copyright (C) 1998, 1999, 2002, 2003 Hewlett-Packard Co
 *	David Mosberger-Tang <davidm@hpl.hp.com>
 */

#include <asm/types.h>

/* floating point status register: */
#define FPSR_TRAP_VD	(1 << 0)	/* invalid op trap disabled */
#define FPSR_TRAP_DD	(1 << 1)	/* denormal trap disabled */
#define FPSR_TRAP_ZD	(1 << 2)	/* zero-divide trap disabled */
#define FPSR_TRAP_OD	(1 << 3)	/* overflow trap disabled */
#define FPSR_TRAP_UD	(1 << 4)	/* underflow trap disabled */
#define FPSR_TRAP_ID	(1 << 5)	/* inexact trap disabled */
#define FPSR_S0(x)	((x) <<  6)
#define FPSR_S1(x)	((x) << 19)
#define FPSR_S2(x)	(__IA64_UL(x) << 32)
#define FPSR_S3(x)	(__IA64_UL(x) << 45)

/* floating-point status field controls: */
#define FPSF_FTZ	(1 << 0)		/* flush-to-zero */
#define FPSF_WRE	(1 << 1)		/* widest-range exponent */
#define FPSF_PC(x)	(((x) & 0x3) << 2)	/* precision control */
#define FPSF_RC(x)	(((x) & 0x3) << 4)	/* rounding control */
#define FPSF_TD		(1 << 6)		/* trap disabled */

/* floating-point status field flags: */
#define FPSF_V		(1 <<  7)		/* invalid operation flag */
#define FPSF_D		(1 <<  8)		/* denormal/unnormal operand flag */
#define FPSF_Z		(1 <<  9)		/* zero divide (IEEE) flag */
#define FPSF_O		(1 << 10)		/* overflow (IEEE) flag */
#define FPSF_U		(1 << 11)		/* underflow (IEEE) flag */
#define FPSF_I		(1 << 12)		/* inexact (IEEE) flag) */

/* floating-point rounding control: */
#define FPRC_NEAREST	0x0
#define FPRC_NEGINF	0x1
#define FPRC_POSINF	0x2
#define FPRC_TRUNC	0x3

#define FPSF_DEFAULT	(FPSF_PC (0x3) | FPSF_RC (FPRC_NEAREST))

/* This default value is the same as HP-UX uses.  Don't change it
   without a very good reason.  */
#define FPSR_DEFAULT	(FPSR_TRAP_VD | FPSR_TRAP_DD | FPSR_TRAP_ZD	\
			 | FPSR_TRAP_OD | FPSR_TRAP_UD | FPSR_TRAP_ID	\
			 | FPSR_S0 (FPSF_DEFAULT)			\
			 | FPSR_S1 (FPSF_DEFAULT | FPSF_TD | FPSF_WRE)	\
			 | FPSR_S2 (FPSF_DEFAULT | FPSF_TD)		\
			 | FPSR_S3 (FPSF_DEFAULT | FPSF_TD))

# ifndef __ASSEMBLY__

struct ia64_fpreg {
	union {
		unsigned long bits[2];
		long double __dummy;	/* force 16-byte alignment */
	} u;
};

# endif /* __ASSEMBLY__ */

#endif /* _ASM_IA64_FPU_H */
