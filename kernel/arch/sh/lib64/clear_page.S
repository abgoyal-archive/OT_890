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
   Copyright 2003 Richard Curnow, SuperH (UK) Ltd.

   This file is subject to the terms and conditions of the GNU General Public
   License.  See the file "COPYING" in the main directory of this archive
   for more details.

   Tight version of memset for the case of just clearing a page.  It turns out
   that having the alloco's spaced out slightly due to the increment/branch
   pair causes them to contend less for access to the cache.  Similarly,
   keeping the stores apart from the allocos causes less contention.  => Do two
   separate loops.  Do multiple stores per loop to amortise the
   increment/branch cost a little.

   Parameters:
   r2 : source effective address (start of page)

   Always clears 4096 bytes.

   Note : alloco guarded by synco to avoid TAKum03020 erratum

*/

	.section .text..SHmedia32,"ax"
	.little

	.balign 8
	.global clear_page
clear_page:
	pta/l 1f, tr1
	pta/l 2f, tr2
	ptabs/l r18, tr0

	movi 4096, r7
	add  r2, r7, r7
	add  r2, r63, r6
1:
	alloco r6, 0
	synco	! TAKum03020
	addi	r6, 32, r6
	bgt/l	r7, r6, tr1

	add  r2, r63, r6
2:
	st.q  r6,   0, r63
	st.q  r6,   8, r63
	st.q  r6,  16, r63
	st.q  r6,  24, r63
	addi r6, 32, r6
	bgt/l r7, r6, tr2

	blink tr0, r63


