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
 * single precision square root
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


#include "ieee754sp.h"

ieee754sp ieee754sp_sqrt(ieee754sp x)
{
	int ix, s, q, m, t, i;
	unsigned int r;
	COMPXSP;

	/* take care of Inf and NaN */

	EXPLODEXSP;
	CLEARCX;
	FLUSHXSP;

	/* x == INF or NAN? */
	switch (xc) {
	case IEEE754_CLASS_QNAN:
		/* sqrt(Nan) = Nan */
		return ieee754sp_nanxcpt(x, "sqrt");
	case IEEE754_CLASS_SNAN:
		SETCX(IEEE754_INVALID_OPERATION);
		return ieee754sp_nanxcpt(ieee754sp_indef(), "sqrt");
	case IEEE754_CLASS_ZERO:
		/* sqrt(0) = 0 */
		return x;
	case IEEE754_CLASS_INF:
		if (xs) {
			/* sqrt(-Inf) = Nan */
			SETCX(IEEE754_INVALID_OPERATION);
			return ieee754sp_nanxcpt(ieee754sp_indef(), "sqrt");
		}
		/* sqrt(+Inf) = Inf */
		return x;
	case IEEE754_CLASS_DNORM:
	case IEEE754_CLASS_NORM:
		if (xs) {
			/* sqrt(-x) = Nan */
			SETCX(IEEE754_INVALID_OPERATION);
			return ieee754sp_nanxcpt(ieee754sp_indef(), "sqrt");
		}
		break;
	}

	ix = x.bits;

	/* normalize x */
	m = (ix >> 23);
	if (m == 0) {		/* subnormal x */
		for (i = 0; (ix & 0x00800000) == 0; i++)
			ix <<= 1;
		m -= i - 1;
	}
	m -= 127;		/* unbias exponent */
	ix = (ix & 0x007fffff) | 0x00800000;
	if (m & 1)		/* odd m, double x to make it even */
		ix += ix;
	m >>= 1;		/* m = [m/2] */

	/* generate sqrt(x) bit by bit */
	ix += ix;
	q = s = 0;		/* q = sqrt(x) */
	r = 0x01000000;		/* r = moving bit from right to left */

	while (r != 0) {
		t = s + r;
		if (t <= ix) {
			s = t + r;
			ix -= t;
			q += r;
		}
		ix += ix;
		r >>= 1;
	}

	if (ix != 0) {
		SETCX(IEEE754_INEXACT);
		switch (ieee754_csr.rm) {
		case IEEE754_RP:
			q += 2;
			break;
		case IEEE754_RN:
			q += (q & 1);
			break;
		}
	}
	ix = (q >> 1) + 0x3f000000;
	ix += (m << 23);
	x.bits = ix;
	return x;
}
