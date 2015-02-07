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
 * Linux/PA-RISC Project (http://www.parisc-linux.org/)
 *
 * Floating-point emulation code
 *  Copyright (C) 2001 Hewlett-Packard (Paul Bame) <bame@debian.org>
 *
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2, or (at your option)
 *    any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
/*
 * BEGIN_DESC
 *
 *  File:
 *	@(#)	pa/spmath/dfcmp.c		$Revision$
 *
 *  Purpose:
 *	dbl_cmp: compare two values
 *
 *  External Interfaces:
 *	dbl_fcmp(leftptr, rightptr, cond, status)
 *
 *  Internal Interfaces:
 *
 *  Theory:
 *	<<please update with a overview of the operation of this file>>
 *
 * END_DESC
*/



#include "float.h"
#include "dbl_float.h"
    
/*
 * dbl_cmp: compare two values
 */
int
dbl_fcmp (dbl_floating_point * leftptr, dbl_floating_point * rightptr,
	  unsigned int cond, unsigned int *status)
                                           
                       /* The predicate to be tested */
                         
    {
    register unsigned int leftp1, leftp2, rightp1, rightp2;
    register int xorresult;
        
    /* Create local copies of the numbers */
    Dbl_copyfromptr(leftptr,leftp1,leftp2);
    Dbl_copyfromptr(rightptr,rightp1,rightp2);
    /*
     * Test for NaN
     */
    if(    (Dbl_exponent(leftp1) == DBL_INFINITY_EXPONENT)
        || (Dbl_exponent(rightp1) == DBL_INFINITY_EXPONENT) )
	{
	/* Check if a NaN is involved.  Signal an invalid exception when 
	 * comparing a signaling NaN or when comparing quiet NaNs and the
	 * low bit of the condition is set */
        if( ((Dbl_exponent(leftp1) == DBL_INFINITY_EXPONENT)
	    && Dbl_isnotzero_mantissa(leftp1,leftp2) 
	    && (Exception(cond) || Dbl_isone_signaling(leftp1)))
	   ||
	    ((Dbl_exponent(rightp1) == DBL_INFINITY_EXPONENT)
	    && Dbl_isnotzero_mantissa(rightp1,rightp2) 
	    && (Exception(cond) || Dbl_isone_signaling(rightp1))) )
	    {
	    if( Is_invalidtrap_enabled() ) {
	    	Set_status_cbit(Unordered(cond));
		return(INVALIDEXCEPTION);
	    }
	    else Set_invalidflag();
	    Set_status_cbit(Unordered(cond));
	    return(NOEXCEPTION);
	    }
	/* All the exceptional conditions are handled, now special case
	   NaN compares */
        else if( ((Dbl_exponent(leftp1) == DBL_INFINITY_EXPONENT)
	    && Dbl_isnotzero_mantissa(leftp1,leftp2))
	   ||
	    ((Dbl_exponent(rightp1) == DBL_INFINITY_EXPONENT)
	    && Dbl_isnotzero_mantissa(rightp1,rightp2)) )
	    {
	    /* NaNs always compare unordered. */
	    Set_status_cbit(Unordered(cond));
	    return(NOEXCEPTION);
	    }
	/* infinities will drop down to the normal compare mechanisms */
	}
    /* First compare for unequal signs => less or greater or
     * special equal case */
    Dbl_xortointp1(leftp1,rightp1,xorresult);
    if( xorresult < 0 )
        {
        /* left negative => less, left positive => greater.
         * equal is possible if both operands are zeros. */
        if( Dbl_iszero_exponentmantissa(leftp1,leftp2) 
	  && Dbl_iszero_exponentmantissa(rightp1,rightp2) )
            {
	    Set_status_cbit(Equal(cond));
	    }
	else if( Dbl_isone_sign(leftp1) )
	    {
	    Set_status_cbit(Lessthan(cond));
	    }
	else
	    {
	    Set_status_cbit(Greaterthan(cond));
	    }
        }
    /* Signs are the same.  Treat negative numbers separately
     * from the positives because of the reversed sense.  */
    else if(Dbl_isequal(leftp1,leftp2,rightp1,rightp2))
        {
        Set_status_cbit(Equal(cond));
        }
    else if( Dbl_iszero_sign(leftp1) )
        {
        /* Positive compare */
	if( Dbl_allp1(leftp1) < Dbl_allp1(rightp1) )
	    {
	    Set_status_cbit(Lessthan(cond));
	    }
	else if( Dbl_allp1(leftp1) > Dbl_allp1(rightp1) )
	    {
	    Set_status_cbit(Greaterthan(cond));
	    }
	else
	    {
	    /* Equal first parts.  Now we must use unsigned compares to
	     * resolve the two possibilities. */
	    if( Dbl_allp2(leftp2) < Dbl_allp2(rightp2) )
		{
		Set_status_cbit(Lessthan(cond));
		}
	    else 
		{
		Set_status_cbit(Greaterthan(cond));
		}
	    }
	}
    else
        {
        /* Negative compare.  Signed or unsigned compares
         * both work the same.  That distinction is only
         * important when the sign bits differ. */
	if( Dbl_allp1(leftp1) > Dbl_allp1(rightp1) )
	    {
	    Set_status_cbit(Lessthan(cond));
	    }
	else if( Dbl_allp1(leftp1) < Dbl_allp1(rightp1) )
	    {
	    Set_status_cbit(Greaterthan(cond));
	    }
	else
	    {
	    /* Equal first parts.  Now we must use unsigned compares to
	     * resolve the two possibilities. */
	    if( Dbl_allp2(leftp2) > Dbl_allp2(rightp2) )
		{
		Set_status_cbit(Lessthan(cond));
		}
	    else 
		{
		Set_status_cbit(Greaterthan(cond));
		}
	    }
        }
	return(NOEXCEPTION);
    }
