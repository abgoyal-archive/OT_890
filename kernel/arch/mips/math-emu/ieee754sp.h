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

/*
 * IEEE754 floating point
 * double precision internal header file
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


#include "ieee754int.h"

#define assert(expr) ((void)0)

/* 3bit extended single precision sticky right shift */
#define SPXSRSXn(rs) \
  (xe += rs, \
   xm = (rs > (SP_MBITS+3))?1:((xm) >> (rs)) | ((xm) << (32-(rs)) != 0))

#define SPXSRSX1() \
  (xe++, (xm = (xm >> 1) | (xm & 1)))

#define SPXSRSYn(rs) \
   (ye+=rs, \
    ym = (rs > (SP_MBITS+3))?1:((ym) >> (rs)) | ((ym) << (32-(rs)) != 0))

#define SPXSRSY1() \
   (ye++, (ym = (ym >> 1) | (ym & 1)))

/* convert denormal to normalized with extended exponent */
#define SPDNORMx(m,e) \
  while( (m >> SP_MBITS) == 0) { m <<= 1; e--; }
#define SPDNORMX	SPDNORMx(xm, xe)
#define SPDNORMY	SPDNORMx(ym, ye)

static inline ieee754sp buildsp(int s, int bx, unsigned m)
{
	ieee754sp r;

	assert((s) == 0 || (s) == 1);
	assert((bx) >= SP_EMIN - 1 + SP_EBIAS
	       && (bx) <= SP_EMAX + 1 + SP_EBIAS);
	assert(((m) >> SP_MBITS) == 0);

	r.parts.sign = s;
	r.parts.bexp = bx;
	r.parts.mant = m;

	return r;
}

extern int ieee754sp_isnan(ieee754sp);
extern int ieee754sp_issnan(ieee754sp);
extern int ieee754si_xcpt(int, const char *, ...);
extern s64 ieee754di_xcpt(s64, const char *, ...);
extern ieee754sp ieee754sp_xcpt(ieee754sp, const char *, ...);
extern ieee754sp ieee754sp_nanxcpt(ieee754sp, const char *, ...);
extern ieee754sp ieee754sp_bestnan(ieee754sp, ieee754sp);
extern ieee754sp ieee754sp_format(int, int, unsigned);


#define SPNORMRET2(s, e, m, name, a0, a1) \
{ \
    ieee754sp V = ieee754sp_format(s, e, m); \
    if(TSTX()) \
      return ieee754sp_xcpt(V, name, a0, a1); \
    else \
      return V; \
}

#define SPNORMRET1(s, e, m, name, a0)  SPNORMRET2(s, e, m, name, a0, a0)
