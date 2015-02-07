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

ieee754sp ieee754sp_fdp(ieee754dp x)
{
	COMPXDP;
	ieee754sp nan;

	EXPLODEXDP;

	CLEARCX;

	FLUSHXDP;

	switch (xc) {
	case IEEE754_CLASS_SNAN:
		SETCX(IEEE754_INVALID_OPERATION);
		return ieee754sp_nanxcpt(ieee754sp_indef(), "fdp");
	case IEEE754_CLASS_QNAN:
		nan = buildsp(xs, SP_EMAX + 1 + SP_EBIAS, (u32)
				(xm >> (DP_MBITS - SP_MBITS)));
		if (!ieee754sp_isnan(nan))
			nan = ieee754sp_indef();
		return ieee754sp_nanxcpt(nan, "fdp", x);
	case IEEE754_CLASS_INF:
		return ieee754sp_inf(xs);
	case IEEE754_CLASS_ZERO:
		return ieee754sp_zero(xs);
	case IEEE754_CLASS_DNORM:
		/* can't possibly be sp representable */
		SETCX(IEEE754_UNDERFLOW);
		SETCX(IEEE754_INEXACT);
		if ((ieee754_csr.rm == IEEE754_RU && !xs) ||
				(ieee754_csr.rm == IEEE754_RD && xs))
			return ieee754sp_xcpt(ieee754sp_mind(xs), "fdp", x);
		return ieee754sp_xcpt(ieee754sp_zero(xs), "fdp", x);
	case IEEE754_CLASS_NORM:
		break;
	}

	{
		u32 rm;

		/* convert from DP_MBITS to SP_MBITS+3 with sticky right shift
		 */
		rm = (xm >> (DP_MBITS - (SP_MBITS + 3))) |
		    ((xm << (64 - (DP_MBITS - (SP_MBITS + 3)))) != 0);

		SPNORMRET1(xs, xe, rm, "fdp", x);
	}
}
