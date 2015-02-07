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

.align 2
.global ___muldi3;
.type ___muldi3, STT_FUNC;

#ifdef CONFIG_ARITHMETIC_OPS_L1
.section .l1.text
#else
.text
#endif

/*
	   R1:R0 * R3:R2
	 = R1.h:R1.l:R0.h:R0.l * R3.h:R3.l:R2.h:R2.l
[X]	 = (R1.h * R3.h) * 2^96
[X]	   + (R1.h * R3.l + R1.l * R3.h) * 2^80
[X]	   + (R1.h * R2.h + R1.l * R3.l + R3.h * R0.h) * 2^64
[T1]	   + (R1.h * R2.l + R3.h * R0.l + R1.l * R2.h + R3.l * R0.h) * 2^48
[T2]	   + (R1.l * R2.l + R3.l * R0.l + R0.h * R2.h) * 2^32
[T3]	   + (R0.l * R2.h + R2.l * R0.h) * 2^16
[T4]	   + (R0.l * R2.l)

	We can discard the first three lines marked "X" since we produce
	only a 64 bit result.  So, we need ten 16-bit multiplies.

	Individual mul-acc results:
[E1]	 =  R1.h * R2.l + R3.h * R0.l + R1.l * R2.h + R3.l * R0.h
[E2]	 =  R1.l * R2.l + R3.l * R0.l + R0.h * R2.h
[E3]	 =  R0.l * R2.h + R2.l * R0.h
[E4]	 =  R0.l * R2.l

	We also need to add high parts from lower-level results to higher ones:
	E[n]c = E[n] + (E[n+1]c >> 16), where E4c := E4

	One interesting property is that all parts of the result that depend
	on the sign of the multiplication are discarded.  Those would be the
	multiplications involving R1.h and R3.h, but only the top 16 bit of
	the 32 bit result depend on the sign, and since R1.h and R3.h only
	occur in E1, the top half of these results is cut off.
	So, we can just use FU mode for all of the 16-bit multiplies, and
	ignore questions of when to use mixed mode.  */

___muldi3:
	/* [SP] technically is part of the caller's frame, but we can
	   use it as scratch space.  */
	A0 = R2.H * R1.L, A1 = R2.L * R1.H (FU) || R3 = [SP + 12];	/* E1 */
	A0 += R3.H * R0.L, A1 += R3.L * R0.H (FU) || [SP] = R4;		/* E1 */
	A0 += A1;							/* E1 */
	R4 = A0.w;
	A0 = R0.l * R3.l (FU);						/* E2 */
	A0 += R2.l * R1.l (FU);						/* E2 */

	A1 = R2.L * R0.L (FU);						/* E4 */
	R3 = A1.w;
	A1 = A1 >> 16;							/* E3c */
	A0 += R2.H * R0.H, A1 += R2.L * R0.H (FU);			/* E2, E3c */
	A1 += R0.L * R2.H (FU);						/* E3c */
	R0 = A1.w;
	A1 = A1 >> 16;							/* E2c */
	A0 += A1;							/* E2c */
	R1 = A0.w;

	/* low(result) = low(E3c):low(E4) */
	R0 = PACK (R0.l, R3.l);
	/* high(result) = E2c + (E1 << 16) */
	R1.h = R1.h + R4.l (NS) || R4 = [SP];
	RTS;

.size ___muldi3, .-___muldi3
