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

/*---------------------------------------------------------------------------+
 |  exception.h                                                              |
 |                                                                           |
 | Copyright (C) 1992    W. Metzenthen, 22 Parker St, Ormond, Vic 3163,      |
 |                       Australia.  E-mail   billm@vaxc.cc.monash.edu.au    |
 |                                                                           |
 +---------------------------------------------------------------------------*/

#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_

#ifdef __ASSEMBLY__
#define	Const_(x)	$##x
#else
#define	Const_(x)	x
#endif

#ifndef SW_C1
#include "fpu_emu.h"
#endif /* SW_C1 */

#define FPU_BUSY        Const_(0x8000)	/* FPU busy bit (8087 compatibility) */
#define EX_ErrorSummary Const_(0x0080)	/* Error summary status */
/* Special exceptions: */
#define	EX_INTERNAL	Const_(0x8000)	/* Internal error in wm-FPU-emu */
#define EX_StackOver	Const_(0x0041|SW_C1)	/* stack overflow */
#define EX_StackUnder	Const_(0x0041)	/* stack underflow */
/* Exception flags: */
#define EX_Precision	Const_(0x0020)	/* loss of precision */
#define EX_Underflow	Const_(0x0010)	/* underflow */
#define EX_Overflow	Const_(0x0008)	/* overflow */
#define EX_ZeroDiv	Const_(0x0004)	/* divide by zero */
#define EX_Denormal	Const_(0x0002)	/* denormalized operand */
#define EX_Invalid	Const_(0x0001)	/* invalid operation */

#define PRECISION_LOST_UP    Const_((EX_Precision | SW_C1))
#define PRECISION_LOST_DOWN  Const_(EX_Precision)

#ifndef __ASSEMBLY__

#ifdef DEBUG
#define	EXCEPTION(x)	{ printk("exception in %s at line %d\n", \
	__FILE__, __LINE__); FPU_exception(x); }
#else
#define	EXCEPTION(x)	FPU_exception(x)
#endif

#endif /* __ASSEMBLY__ */

#endif /* _EXCEPTION_H_ */
