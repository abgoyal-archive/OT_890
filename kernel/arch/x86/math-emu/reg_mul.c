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
 |  reg_mul.c                                                                |
 |                                                                           |
 | Multiply one FPU_REG by another, put the result in a destination FPU_REG. |
 |                                                                           |
 | Copyright (C) 1992,1993,1997                                              |
 |                  W. Metzenthen, 22 Parker St, Ormond, Vic 3163, Australia |
 |                  E-mail   billm@suburbia.net                              |
 |                                                                           |
 | Returns the tag of the result if no exceptions or errors occurred.        |
 |                                                                           |
 +---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------+
 | The destination may be any FPU_REG, including one of the source FPU_REGs. |
 +---------------------------------------------------------------------------*/

#include "fpu_emu.h"
#include "exception.h"
#include "reg_constant.h"
#include "fpu_system.h"

/*
  Multiply two registers to give a register result.
  The sources are st(deststnr) and (b,tagb,signb).
  The destination is st(deststnr).
  */
/* This routine must be called with non-empty source registers */
int FPU_mul(FPU_REG const *b, u_char tagb, int deststnr, int control_w)
{
	FPU_REG *a = &st(deststnr);
	FPU_REG *dest = a;
	u_char taga = FPU_gettagi(deststnr);
	u_char saved_sign = getsign(dest);
	u_char sign = (getsign(a) ^ getsign(b));
	int tag;

	if (!(taga | tagb)) {
		/* Both regs Valid, this should be the most common case. */

		tag =
		    FPU_u_mul(a, b, dest, control_w, sign,
			      exponent(a) + exponent(b));
		if (tag < 0) {
			setsign(dest, saved_sign);
			return tag;
		}
		FPU_settagi(deststnr, tag);
		return tag;
	}

	if (taga == TAG_Special)
		taga = FPU_Special(a);
	if (tagb == TAG_Special)
		tagb = FPU_Special(b);

	if (((taga == TAG_Valid) && (tagb == TW_Denormal))
	    || ((taga == TW_Denormal) && (tagb == TAG_Valid))
	    || ((taga == TW_Denormal) && (tagb == TW_Denormal))) {
		FPU_REG x, y;
		if (denormal_operand() < 0)
			return FPU_Exception;

		FPU_to_exp16(a, &x);
		FPU_to_exp16(b, &y);
		tag = FPU_u_mul(&x, &y, dest, control_w, sign,
				exponent16(&x) + exponent16(&y));
		if (tag < 0) {
			setsign(dest, saved_sign);
			return tag;
		}
		FPU_settagi(deststnr, tag);
		return tag;
	} else if ((taga <= TW_Denormal) && (tagb <= TW_Denormal)) {
		if (((tagb == TW_Denormal) || (taga == TW_Denormal))
		    && (denormal_operand() < 0))
			return FPU_Exception;

		/* Must have either both arguments == zero, or
		   one valid and the other zero.
		   The result is therefore zero. */
		FPU_copy_to_regi(&CONST_Z, TAG_Zero, deststnr);
		/* The 80486 book says that the answer is +0, but a real
		   80486 behaves this way.
		   IEEE-754 apparently says it should be this way. */
		setsign(dest, sign);
		return TAG_Zero;
	}
	/* Must have infinities, NaNs, etc */
	else if ((taga == TW_NaN) || (tagb == TW_NaN)) {
		return real_2op_NaN(b, tagb, deststnr, &st(0));
	} else if (((taga == TW_Infinity) && (tagb == TAG_Zero))
		   || ((tagb == TW_Infinity) && (taga == TAG_Zero))) {
		return arith_invalid(deststnr);	/* Zero*Infinity is invalid */
	} else if (((taga == TW_Denormal) || (tagb == TW_Denormal))
		   && (denormal_operand() < 0)) {
		return FPU_Exception;
	} else if (taga == TW_Infinity) {
		FPU_copy_to_regi(a, TAG_Special, deststnr);
		setsign(dest, sign);
		return TAG_Special;
	} else if (tagb == TW_Infinity) {
		FPU_copy_to_regi(b, TAG_Special, deststnr);
		setsign(dest, sign);
		return TAG_Special;
	}
#ifdef PARANOID
	else {
		EXCEPTION(EX_INTERNAL | 0x102);
		return FPU_Exception;
	}
#endif /* PARANOID */

	return 0;
}
