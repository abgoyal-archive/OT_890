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

/* IEEE754 floating point arithmetic
 * double precision: common utilities
 */
/*
 * MIPS floating point support
 * Copyright (C) 1994-2000 Algorithmics Ltd.
 * http://www.algor.co.uk
 *
 * ########################################################################
 *
 *  This program is free software; you can distribute it and/or modify it
 *  under the terms of the GNU General Public License (Version 2) as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place - Suite 330, Boston MA 02111-1307, USA.
 *
 * ########################################################################
 */


#include "ieee754dp.h"

int ieee754dp_finite(ieee754dp x)
{
	return DPBEXP(x) != DP_EMAX + 1 + DP_EBIAS;
}

ieee754dp ieee754dp_copysign(ieee754dp x, ieee754dp y)
{
	CLEARCX;
	DPSIGN(x) = DPSIGN(y);
	return x;
}


ieee754dp ieee754dp_neg(ieee754dp x)
{
	COMPXDP;

	EXPLODEXDP;
	CLEARCX;
	FLUSHXDP;

	/*
	 * Invert the sign ALWAYS to prevent an endless recursion on
	 * pow() in libc.
	 */
	/* quick fix up */
	DPSIGN(x) ^= 1;

	if (xc == IEEE754_CLASS_SNAN) {
		ieee754dp y = ieee754dp_indef();
		SETCX(IEEE754_INVALID_OPERATION);
		DPSIGN(y) = DPSIGN(x);
		return ieee754dp_nanxcpt(y, "neg");
	}

	if (ieee754dp_isnan(x))	/* but not infinity */
		return ieee754dp_nanxcpt(x, "neg", x);
	return x;
}


ieee754dp ieee754dp_abs(ieee754dp x)
{
	COMPXDP;

	EXPLODEXDP;
	CLEARCX;
	FLUSHXDP;

	if (xc == IEEE754_CLASS_SNAN) {
		SETCX(IEEE754_INVALID_OPERATION);
		return ieee754dp_nanxcpt(ieee754dp_indef(), "neg");
	}

	if (ieee754dp_isnan(x))	/* but not infinity */
		return ieee754dp_nanxcpt(x, "abs", x);

	/* quick fix up */
	DPSIGN(x) = 0;
	return x;
}
