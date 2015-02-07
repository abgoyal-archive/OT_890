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
 * File:         arch/blackfin/lib/modsi3.S
 * Based on:
 * Author:
 *
 * Created:
 * Description:  This program computes 32 bit signed remainder. It calls div32 function
 *               for quotient estimation.
 *
 *               Registers used :
 *               Numerator/ Denominator in  R0, R1
 *                 R0  -  returns remainder.
 *                 R2-R7
 *
 * Modified:
 *               Copyright 2004-2006 Analog Devices Inc.
 *
 * Bugs:         Enter bugs at http://blackfin.uclinux.org/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see the file COPYING, or write
 * to the Free Software Foundation, Inc.,
 * 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

.global ___modsi3;
.type ___modsi3, STT_FUNC;
.extern ___divsi3;
.type ___divsi3, STT_FUNC;

#ifdef CONFIG_ARITHMETIC_OPS_L1
.section .l1.text
#else
.text
#endif

___modsi3:

	CC=R0==0;
	IF CC JUMP .LRETURN_R0;		/* Return 0, if numerator  == 0 */
	CC=R1==0;
	IF CC JUMP .LRETURN_ZERO;		/* Return 0, if denominator == 0 */
	CC=R0==R1;
	IF CC JUMP .LRETURN_ZERO;		/* Return 0, if numerator == denominator */
	CC = R1 == 1;
	IF CC JUMP .LRETURN_ZERO;		/* Return 0, if denominator ==  1 */
	CC = R1 == -1;
	IF CC JUMP .LRETURN_ZERO;		/* Return 0, if denominator == -1 */

	/* Valid input. Use __divsi3() to compute the quotient, and then
	 * derive the remainder from that. */

	[--SP] = (R7:6);		/* Push  R7 and R6 */
	[--SP] = RETS;			/* and return address */
	R7 = R0;			/* Copy of R0 */
	R6 = R1;			/* Save for later */
	SP += -12;			/* Should always provide this space */
	CALL ___divsi3;			/* Compute signed quotient using ___divsi3()*/
	SP += 12;
	R0 *= R6;			/* Quotient * divisor */
	R0 = R7 - R0;			/* Dividend - (quotient * divisor) */
	RETS = [SP++];			/* Get back return address */
	(R7:6) = [SP++];		/* Pop registers R7 and R4 */
	RTS;				/* Store remainder    */

.LRETURN_ZERO:
	R0 = 0;
.LRETURN_R0:
	RTS;

.size ___modsi3, .-___modsi3
