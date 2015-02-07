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
 |  fpu_etc.c                                                                |
 |                                                                           |
 | Implement a few FPU instructions.                                         |
 |                                                                           |
 | Copyright (C) 1992,1993,1994,1997                                         |
 |                       W. Metzenthen, 22 Parker St, Ormond, Vic 3163,      |
 |                       Australia.  E-mail   billm@suburbia.net             |
 |                                                                           |
 |                                                                           |
 +---------------------------------------------------------------------------*/

#include "fpu_system.h"
#include "exception.h"
#include "fpu_emu.h"
#include "status_w.h"
#include "reg_constant.h"

static void fchs(FPU_REG *st0_ptr, u_char st0tag)
{
	if (st0tag ^ TAG_Empty) {
		signbyte(st0_ptr) ^= SIGN_NEG;
		clear_C1();
	} else
		FPU_stack_underflow();
}

static void fabs(FPU_REG *st0_ptr, u_char st0tag)
{
	if (st0tag ^ TAG_Empty) {
		setpositive(st0_ptr);
		clear_C1();
	} else
		FPU_stack_underflow();
}

static void ftst_(FPU_REG *st0_ptr, u_char st0tag)
{
	switch (st0tag) {
	case TAG_Zero:
		setcc(SW_C3);
		break;
	case TAG_Valid:
		if (getsign(st0_ptr) == SIGN_POS)
			setcc(0);
		else
			setcc(SW_C0);
		break;
	case TAG_Special:
		switch (FPU_Special(st0_ptr)) {
		case TW_Denormal:
			if (getsign(st0_ptr) == SIGN_POS)
				setcc(0);
			else
				setcc(SW_C0);
			if (denormal_operand() < 0) {
#ifdef PECULIAR_486
				/* This is weird! */
				if (getsign(st0_ptr) == SIGN_POS)
					setcc(SW_C3);
#endif /* PECULIAR_486 */
				return;
			}
			break;
		case TW_NaN:
			setcc(SW_C0 | SW_C2 | SW_C3);	/* Operand is not comparable */
			EXCEPTION(EX_Invalid);
			break;
		case TW_Infinity:
			if (getsign(st0_ptr) == SIGN_POS)
				setcc(0);
			else
				setcc(SW_C0);
			break;
		default:
			setcc(SW_C0 | SW_C2 | SW_C3);	/* Operand is not comparable */
			EXCEPTION(EX_INTERNAL | 0x14);
			break;
		}
		break;
	case TAG_Empty:
		setcc(SW_C0 | SW_C2 | SW_C3);
		EXCEPTION(EX_StackUnder);
		break;
	}
}

static void fxam(FPU_REG *st0_ptr, u_char st0tag)
{
	int c = 0;
	switch (st0tag) {
	case TAG_Empty:
		c = SW_C3 | SW_C0;
		break;
	case TAG_Zero:
		c = SW_C3;
		break;
	case TAG_Valid:
		c = SW_C2;
		break;
	case TAG_Special:
		switch (FPU_Special(st0_ptr)) {
		case TW_Denormal:
			c = SW_C2 | SW_C3;	/* Denormal */
			break;
		case TW_NaN:
			/* We also use NaN for unsupported types. */
			if ((st0_ptr->sigh & 0x80000000)
			    && (exponent(st0_ptr) == EXP_OVER))
				c = SW_C0;
			break;
		case TW_Infinity:
			c = SW_C2 | SW_C0;
			break;
		}
	}
	if (getsign(st0_ptr) == SIGN_NEG)
		c |= SW_C1;
	setcc(c);
}

static FUNC_ST0 const fp_etc_table[] = {
	fchs, fabs, (FUNC_ST0) FPU_illegal, (FUNC_ST0) FPU_illegal,
	ftst_, fxam, (FUNC_ST0) FPU_illegal, (FUNC_ST0) FPU_illegal
};

void FPU_etc(void)
{
	(fp_etc_table[FPU_rm]) (&st(0), FPU_gettag0());
}
