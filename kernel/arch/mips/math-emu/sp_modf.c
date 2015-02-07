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

/* modf function is always exact for a finite number
*/
ieee754sp ieee754sp_modf(ieee754sp x, ieee754sp * ip)
{
	COMPXSP;

	CLEARCX;

	EXPLODEXSP;

	switch (xc) {
	case IEEE754_CLASS_SNAN:
	case IEEE754_CLASS_QNAN:
	case IEEE754_CLASS_INF:
	case IEEE754_CLASS_ZERO:
		*ip = x;
		return x;
	case IEEE754_CLASS_DNORM:
		/* far to small */
		*ip = ieee754sp_zero(xs);
		return x;
	case IEEE754_CLASS_NORM:
		break;
	}
	if (xe < 0) {
		*ip = ieee754sp_zero(xs);
		return x;
	}
	if (xe >= SP_MBITS) {
		*ip = x;
		return ieee754sp_zero(xs);
	}
	/* generate ipart mantissa by clearing bottom bits
	 */
	*ip = buildsp(xs, xe + SP_EBIAS,
		      ((xm >> (SP_MBITS - xe)) << (SP_MBITS - xe)) &
		      ~SP_HIDDEN_BIT);

	/* generate fpart mantissa by clearing top bits
	 * and normalizing (must be able to normalize)
	 */
	xm = (xm << (32 - (SP_MBITS - xe))) >> (32 - (SP_MBITS - xe));
	if (xm == 0)
		return ieee754sp_zero(xs);

	while ((xm >> SP_MBITS) == 0) {
		xm <<= 1;
		xe--;
	}
	return buildsp(xs, xe + SP_EBIAS, xm & ~SP_HIDDEN_BIT);
}
