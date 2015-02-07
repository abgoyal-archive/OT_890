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
 |  reg_convert.c                                                            |
 |                                                                           |
 |  Convert register representation.                                         |
 |                                                                           |
 | Copyright (C) 1992,1993,1994,1996,1997                                    |
 |                  W. Metzenthen, 22 Parker St, Ormond, Vic 3163, Australia |
 |                  E-mail   billm@suburbia.net                              |
 |                                                                           |
 |                                                                           |
 +---------------------------------------------------------------------------*/

#include "exception.h"
#include "fpu_emu.h"

int FPU_to_exp16(FPU_REG const *a, FPU_REG *x)
{
	int sign = getsign(a);

	*(long long *)&(x->sigl) = *(const long long *)&(a->sigl);

	/* Set up the exponent as a 16 bit quantity. */
	setexponent16(x, exponent(a));

	if (exponent16(x) == EXP_UNDER) {
		/* The number is a de-normal or pseudodenormal. */
		/* We only deal with the significand and exponent. */

		if (x->sigh & 0x80000000) {
			/* Is a pseudodenormal. */
			/* This is non-80486 behaviour because the number
			   loses its 'denormal' identity. */
			addexponent(x, 1);
		} else {
			/* Is a denormal. */
			addexponent(x, 1);
			FPU_normalize_nuo(x);
		}
	}

	if (!(x->sigh & 0x80000000)) {
		EXCEPTION(EX_INTERNAL | 0x180);
	}

	return sign;
}
