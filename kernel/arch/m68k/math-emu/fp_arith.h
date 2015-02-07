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

   fp_arith.h: floating-point math routines for the Linux-m68k
   floating point emulator.

   Copyright (c) 1998 David Huggins-Daines.

   Somewhat based on the AlphaLinux floating point emulator, by David
   Mosberger-Tang.

   You may copy, modify, and redistribute this file under the terms of
   the GNU General Public License, version 2, or any later version, at
   your convenience.

 */

#ifndef FP_ARITH_H
#define FP_ARITH_H

/* easy ones */
struct fp_ext *
fp_fabs(struct fp_ext *dest, struct fp_ext *src);
struct fp_ext *
fp_fneg(struct fp_ext *dest, struct fp_ext *src);

/* straightforward arithmetic */
struct fp_ext *
fp_fadd(struct fp_ext *dest, struct fp_ext *src);
struct fp_ext *
fp_fsub(struct fp_ext *dest, struct fp_ext *src);
struct fp_ext *
fp_fcmp(struct fp_ext *dest, struct fp_ext *src);
struct fp_ext *
fp_ftst(struct fp_ext *dest, struct fp_ext *src);
struct fp_ext *
fp_fmul(struct fp_ext *dest, struct fp_ext *src);
struct fp_ext *
fp_fdiv(struct fp_ext *dest, struct fp_ext *src);

/* ones that do rounding and integer conversions */
struct fp_ext *
fp_fmod(struct fp_ext *dest, struct fp_ext *src);
struct fp_ext *
fp_frem(struct fp_ext *dest, struct fp_ext *src);
struct fp_ext *
fp_fint(struct fp_ext *dest, struct fp_ext *src);
struct fp_ext *
fp_fintrz(struct fp_ext *dest, struct fp_ext *src);
struct fp_ext *
fp_fscale(struct fp_ext *dest, struct fp_ext *src);

#endif	/* FP_ARITH__H */
