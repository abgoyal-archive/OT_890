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
 |  fpu_tags.c                                                               |
 |                                                                           |
 |  Set FPU register tags.                                                   |
 |                                                                           |
 | Copyright (C) 1997                                                        |
 |                  W. Metzenthen, 22 Parker St, Ormond, Vic 3163, Australia |
 |                  E-mail   billm@jacobi.maths.monash.edu.au                |
 |                                                                           |
 |                                                                           |
 +---------------------------------------------------------------------------*/

#include "fpu_emu.h"
#include "fpu_system.h"
#include "exception.h"

void FPU_pop(void)
{
	fpu_tag_word |= 3 << ((top & 7) * 2);
	top++;
}

int FPU_gettag0(void)
{
	return (fpu_tag_word >> ((top & 7) * 2)) & 3;
}

int FPU_gettagi(int stnr)
{
	return (fpu_tag_word >> (((top + stnr) & 7) * 2)) & 3;
}

int FPU_gettag(int regnr)
{
	return (fpu_tag_word >> ((regnr & 7) * 2)) & 3;
}

void FPU_settag0(int tag)
{
	int regnr = top;
	regnr &= 7;
	fpu_tag_word &= ~(3 << (regnr * 2));
	fpu_tag_word |= (tag & 3) << (regnr * 2);
}

void FPU_settagi(int stnr, int tag)
{
	int regnr = stnr + top;
	regnr &= 7;
	fpu_tag_word &= ~(3 << (regnr * 2));
	fpu_tag_word |= (tag & 3) << (regnr * 2);
}

void FPU_settag(int regnr, int tag)
{
	regnr &= 7;
	fpu_tag_word &= ~(3 << (regnr * 2));
	fpu_tag_word |= (tag & 3) << (regnr * 2);
}

int FPU_Special(FPU_REG const *ptr)
{
	int exp = exponent(ptr);

	if (exp == EXP_BIAS + EXP_UNDER)
		return TW_Denormal;
	else if (exp != EXP_BIAS + EXP_OVER)
		return TW_NaN;
	else if ((ptr->sigh == 0x80000000) && (ptr->sigl == 0))
		return TW_Infinity;
	return TW_NaN;
}

int isNaN(FPU_REG const *ptr)
{
	return ((exponent(ptr) == EXP_BIAS + EXP_OVER)
		&& !((ptr->sigh == 0x80000000) && (ptr->sigl == 0)));
}

int FPU_empty_i(int stnr)
{
	int regnr = (top + stnr) & 7;

	return ((fpu_tag_word >> (regnr * 2)) & 3) == TAG_Empty;
}

int FPU_stackoverflow(FPU_REG ** st_new_ptr)
{
	*st_new_ptr = &st(-1);

	return ((fpu_tag_word >> (((top - 1) & 7) * 2)) & 3) != TAG_Empty;
}

void FPU_copy_to_regi(FPU_REG const *r, u_char tag, int stnr)
{
	reg_copy(r, &st(stnr));
	FPU_settagi(stnr, tag);
}

void FPU_copy_to_reg1(FPU_REG const *r, u_char tag)
{
	reg_copy(r, &st(1));
	FPU_settagi(1, tag);
}

void FPU_copy_to_reg0(FPU_REG const *r, u_char tag)
{
	int regnr = top;
	regnr &= 7;

	reg_copy(r, &st(0));

	fpu_tag_word &= ~(3 << (regnr * 2));
	fpu_tag_word |= (tag & 3) << (regnr * 2);
}
