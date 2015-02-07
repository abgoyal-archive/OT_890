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

/* Software floating-point emulation.
   Basic eight-word fraction declaration and manipulation.
   Copyright (C) 1997,1998,1999 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Richard Henderson (rth@cygnus.com),
		  Jakub Jelinek (jj@ultra.linux.cz) and
		  Peter Maydell (pmaydell@chiark.greenend.org.uk).
                                                         
   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the GNU C Library; see the file COPYING.LIB.  If
   not, write to the Free Software Foundation, Inc.,
   59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  */

#ifndef __MATH_EMU_OP_8_H__
#define __MATH_EMU_OP_8_H__

/* We need just a few things from here for op-4, if we ever need some
   other macros, they can be added. */
#define _FP_FRAC_DECL_8(X)	_FP_W_TYPE X##_f[8]
#define _FP_FRAC_HIGH_8(X)	(X##_f[7])
#define _FP_FRAC_LOW_8(X)	(X##_f[0])
#define _FP_FRAC_WORD_8(X,w)	(X##_f[w])

#define _FP_FRAC_SLL_8(X,N)						\
  do {									\
    _FP_I_TYPE _up, _down, _skip, _i;					\
    _skip = (N) / _FP_W_TYPE_SIZE;					\
    _up = (N) % _FP_W_TYPE_SIZE;					\
    _down = _FP_W_TYPE_SIZE - _up;					\
    if (!_up)								\
      for (_i = 7; _i >= _skip; --_i)					\
	X##_f[_i] = X##_f[_i-_skip];					\
    else								\
      {									\
	for (_i = 7; _i > _skip; --_i)					\
	  X##_f[_i] = X##_f[_i-_skip] << _up				\
		      | X##_f[_i-_skip-1] >> _down;			\
	X##_f[_i--] = X##_f[0] << _up; 					\
      }									\
    for (; _i >= 0; --_i)						\
      X##_f[_i] = 0;							\
  } while (0)

#define _FP_FRAC_SRL_8(X,N)						\
  do {									\
    _FP_I_TYPE _up, _down, _skip, _i;					\
    _skip = (N) / _FP_W_TYPE_SIZE;					\
    _down = (N) % _FP_W_TYPE_SIZE;					\
    _up = _FP_W_TYPE_SIZE - _down;					\
    if (!_down)								\
      for (_i = 0; _i <= 7-_skip; ++_i)					\
	X##_f[_i] = X##_f[_i+_skip];					\
    else								\
      {									\
	for (_i = 0; _i < 7-_skip; ++_i)				\
	  X##_f[_i] = X##_f[_i+_skip] >> _down				\
		      | X##_f[_i+_skip+1] << _up;			\
	X##_f[_i++] = X##_f[7] >> _down;				\
      }									\
    for (; _i < 8; ++_i)						\
      X##_f[_i] = 0;							\
  } while (0)


/* Right shift with sticky-lsb. 
 * What this actually means is that we do a standard right-shift,
 * but that if any of the bits that fall off the right hand side
 * were one then we always set the LSbit.
 */
#define _FP_FRAC_SRS_8(X,N,size)					\
  do {									\
    _FP_I_TYPE _up, _down, _skip, _i;					\
    _FP_W_TYPE _s;							\
    _skip = (N) / _FP_W_TYPE_SIZE;					\
    _down = (N) % _FP_W_TYPE_SIZE;					\
    _up = _FP_W_TYPE_SIZE - _down;					\
    for (_s = _i = 0; _i < _skip; ++_i)					\
      _s |= X##_f[_i];							\
    _s |= X##_f[_i] << _up;						\
/* s is now != 0 if we want to set the LSbit */				\
    if (!_down)								\
      for (_i = 0; _i <= 7-_skip; ++_i)					\
	X##_f[_i] = X##_f[_i+_skip];					\
    else								\
      {									\
	for (_i = 0; _i < 7-_skip; ++_i)				\
	  X##_f[_i] = X##_f[_i+_skip] >> _down				\
		      | X##_f[_i+_skip+1] << _up;			\
	X##_f[_i++] = X##_f[7] >> _down;				\
      }									\
    for (; _i < 8; ++_i)						\
      X##_f[_i] = 0;							\
    /* don't fix the LSB until the very end when we're sure f[0] is stable */	\
    X##_f[0] |= (_s != 0);						\
  } while (0)

#endif
