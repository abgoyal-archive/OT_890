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
 * linux/arch/blackfin/mcount.S
 *
 * Copyright (C) 2006 Analog Devices Inc.
 *
 * 2007/04/12 Save index, length, modify and base registers. --rpm
 */

#include <linux/linkage.h>
#include <asm/blackfin.h>

.text

.align 4 	/* just in case */

ENTRY(__mcount)
	[--sp] = i0;
	[--sp] = i1;
	[--sp] = i2;
	[--sp] = i3;
	[--sp] = l0;
	[--sp] = l1;
	[--sp] = l2;
	[--sp] = l3;
	[--sp] = m0;
	[--sp] = m1;
	[--sp] = m2;
	[--sp] = m3;
	[--sp] = b0;
	[--sp] = b1;
	[--sp] = b2;
	[--sp] = b3;
	[--sp] = ( r7:0, p5:0 );
	[--sp] = ASTAT;

	p1.L = _ipipe_trace_enable;
	p1.H = _ipipe_trace_enable;
	r7 = [p1];
	CC = r7 == 0;
	if CC jump out;
	link 0x10;
	r0 = 0x0;
	[sp + 0xc] = r0; /* v */
	r0 = 0x0;	/* type: IPIPE_TRACE_FN */
	r1 = rets;
	p0 = [fp];	/* p0: Prior FP */
	r2 = [p0 + 4];	/* r2: Prior RETS */
	call ___ipipe_trace;
	unlink;
out:
	ASTAT = [sp++];
	( r7:0, p5:0 ) = [sp++];
	b3 = [sp++];
	b2 = [sp++];
	b1 = [sp++];
	b0 = [sp++];
	m3 = [sp++];
	m2 = [sp++];
	m1 = [sp++];
	m0 = [sp++];
	l3 = [sp++];
	l2 = [sp++];
	l1 = [sp++];
	l0 = [sp++];
	i3 = [sp++];
	i2 = [sp++];
	i1 = [sp++];
	i0 = [sp++];
	rts;
ENDPROC(__mcount)
