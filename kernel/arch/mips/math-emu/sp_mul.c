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
 * single precision
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

ieee754sp ieee754sp_mul(ieee754sp x, ieee754sp y)
{
	COMPXSP;
	COMPYSP;

	EXPLODEXSP;
	EXPLODEYSP;

	CLEARCX;

	FLUSHXSP;
	FLUSHYSP;

	switch (CLPAIR(xc, yc)) {
	case CLPAIR(IEEE754_CLASS_SNAN, IEEE754_CLASS_QNAN):
	case CLPAIR(IEEE754_CLASS_QNAN, IEEE754_CLASS_SNAN):
	case CLPAIR(IEEE754_CLASS_SNAN, IEEE754_CLASS_SNAN):
	case CLPAIR(IEEE754_CLASS_ZERO, IEEE754_CLASS_SNAN):
	case CLPAIR(IEEE754_CLASS_NORM, IEEE754_CLASS_SNAN):
	case CLPAIR(IEEE754_CLASS_DNORM, IEEE754_CLASS_SNAN):
	case CLPAIR(IEEE754_CLASS_INF, IEEE754_CLASS_SNAN):
	case CLPAIR(IEEE754_CLASS_SNAN, IEEE754_CLASS_ZERO):
	case CLPAIR(IEEE754_CLASS_SNAN, IEEE754_CLASS_NORM):
	case CLPAIR(IEEE754_CLASS_SNAN, IEEE754_CLASS_DNORM):
	case CLPAIR(IEEE754_CLASS_SNAN, IEEE754_CLASS_INF):
		SETCX(IEEE754_INVALID_OPERATION);
		return ieee754sp_nanxcpt(ieee754sp_indef(), "mul", x, y);

	case CLPAIR(IEEE754_CLASS_ZERO, IEEE754_CLASS_QNAN):
	case CLPAIR(IEEE754_CLASS_NORM, IEEE754_CLASS_QNAN):
	case CLPAIR(IEEE754_CLASS_DNORM, IEEE754_CLASS_QNAN):
	case CLPAIR(IEEE754_CLASS_INF, IEEE754_CLASS_QNAN):
		return y;

	case CLPAIR(IEEE754_CLASS_QNAN, IEEE754_CLASS_QNAN):
	case CLPAIR(IEEE754_CLASS_QNAN, IEEE754_CLASS_ZERO):
	case CLPAIR(IEEE754_CLASS_QNAN, IEEE754_CLASS_NORM):
	case CLPAIR(IEEE754_CLASS_QNAN, IEEE754_CLASS_DNORM):
	case CLPAIR(IEEE754_CLASS_QNAN, IEEE754_CLASS_INF):
		return x;


		/* Infinity handling */

	case CLPAIR(IEEE754_CLASS_INF, IEEE754_CLASS_ZERO):
	case CLPAIR(IEEE754_CLASS_ZERO, IEEE754_CLASS_INF):
		SETCX(IEEE754_INVALID_OPERATION);
		return ieee754sp_xcpt(ieee754sp_indef(), "mul", x, y);

	case CLPAIR(IEEE754_CLASS_NORM, IEEE754_CLASS_INF):
	case CLPAIR(IEEE754_CLASS_DNORM, IEEE754_CLASS_INF):
	case CLPAIR(IEEE754_CLASS_INF, IEEE754_CLASS_NORM):
	case CLPAIR(IEEE754_CLASS_INF, IEEE754_CLASS_DNORM):
	case CLPAIR(IEEE754_CLASS_INF, IEEE754_CLASS_INF):
		return ieee754sp_inf(xs ^ ys);

	case CLPAIR(IEEE754_CLASS_ZERO, IEEE754_CLASS_ZERO):
	case CLPAIR(IEEE754_CLASS_ZERO, IEEE754_CLASS_NORM):
	case CLPAIR(IEEE754_CLASS_ZERO, IEEE754_CLASS_DNORM):
	case CLPAIR(IEEE754_CLASS_NORM, IEEE754_CLASS_ZERO):
	case CLPAIR(IEEE754_CLASS_DNORM, IEEE754_CLASS_ZERO):
		return ieee754sp_zero(xs ^ ys);


	case CLPAIR(IEEE754_CLASS_DNORM, IEEE754_CLASS_DNORM):
		SPDNORMX;

	case CLPAIR(IEEE754_CLASS_NORM, IEEE754_CLASS_DNORM):
		SPDNORMY;
		break;

	case CLPAIR(IEEE754_CLASS_DNORM, IEEE754_CLASS_NORM):
		SPDNORMX;
		break;

	case CLPAIR(IEEE754_CLASS_NORM, IEEE754_CLASS_NORM):
		break;
	}
	/* rm = xm * ym, re = xe+ye basicly */
	assert(xm & SP_HIDDEN_BIT);
	assert(ym & SP_HIDDEN_BIT);

	{
		int re = xe + ye;
		int rs = xs ^ ys;
		unsigned rm;

		/* shunt to top of word */
		xm <<= 32 - (SP_MBITS + 1);
		ym <<= 32 - (SP_MBITS + 1);

		/* multiply 32bits xm,ym to give high 32bits rm with stickness
		 */
		{
			unsigned short lxm = xm & 0xffff;
			unsigned short hxm = xm >> 16;
			unsigned short lym = ym & 0xffff;
			unsigned short hym = ym >> 16;
			unsigned lrm;
			unsigned hrm;

			lrm = lxm * lym;	/* 16 * 16 => 32 */
			hrm = hxm * hym;	/* 16 * 16 => 32 */

			{
				unsigned t = lxm * hym;	/* 16 * 16 => 32 */
				{
					unsigned at = lrm + (t << 16);
					hrm += at < lrm;
					lrm = at;
				}
				hrm = hrm + (t >> 16);
			}

			{
				unsigned t = hxm * lym;	/* 16 * 16 => 32 */
				{
					unsigned at = lrm + (t << 16);
					hrm += at < lrm;
					lrm = at;
				}
				hrm = hrm + (t >> 16);
			}
			rm = hrm | (lrm != 0);
		}

		/*
		 * sticky shift down to normal rounding precision
		 */
		if ((int) rm < 0) {
			rm = (rm >> (32 - (SP_MBITS + 1 + 3))) |
			    ((rm << (SP_MBITS + 1 + 3)) != 0);
			re++;
		} else {
			rm = (rm >> (32 - (SP_MBITS + 1 + 3 + 1))) |
			    ((rm << (SP_MBITS + 1 + 3 + 1)) != 0);
		}
		assert(rm & (SP_HIDDEN_BIT << 3));

		SPNORMRET2(rs, re, rm, "mul", x, y);
	}
}
