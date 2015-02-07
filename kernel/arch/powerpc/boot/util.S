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
 * Copied from <file:arch/powerpc/kernel/misc_32.S>
 *
 * This file contains miscellaneous low-level functions.
 *    Copyright (C) 1995-1996 Gary Thomas (gdt@linuxppc.org)
 *
 * Largely rewritten by Cort Dougan (cort@cs.nmt.edu)
 * and Paul Mackerras.
 *
 * kexec bits:
 * Copyright (C) 2002-2003 Eric Biederman  <ebiederm@xmission.com>
 * GameCube/ppc32 port Copyright (C) 2004 Albert Herranz
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 *
 */
#include "ppc_asm.h"

#define SPRN_PVR        0x11F   /* Processor Version Register */

	.text

/* udelay (on non-601 processors) needs to know the period of the
 * timebase in nanoseconds.  This used to be hardcoded to be 60ns
 * (period of 66MHz/4).  Now a variable is used that is initialized to
 * 60 for backward compatibility, but it can be overridden as necessary
 * with code something like this:
 *    extern unsigned long timebase_period_ns;
 *    timebase_period_ns = 1000000000 / bd->bi_tbfreq;
 */
	.data
	.globl timebase_period_ns
timebase_period_ns:
	.long	60

	.text
/*
 * Delay for a number of microseconds
 */
	.globl	udelay
udelay:
	mfspr	r4,SPRN_PVR
	srwi	r4,r4,16
	cmpwi	0,r4,1		/* 601 ? */
	bne	.udelay_not_601
00:	li	r0,86	/* Instructions / microsecond? */
	mtctr	r0
10:	addi	r0,r0,0 /* NOP */
	bdnz	10b
	subic.	r3,r3,1
	bne	00b
	blr

.udelay_not_601:
	mulli	r4,r3,1000	/* nanoseconds */
	/*  Change r4 to be the number of ticks using:
	 *	(nanoseconds + (timebase_period_ns - 1 )) / timebase_period_ns
	 *  timebase_period_ns defaults to 60 (16.6MHz) */
	mflr	r5
	bl	0f
0:	mflr	r6
	mtlr	r5
	lis	r5,0b@ha
	addi	r5,r5,0b@l
	subf	r5,r5,r6	/* In case we're relocated */
	addis	r5,r5,timebase_period_ns@ha
	lwz	r5,timebase_period_ns@l(r5)
	add	r4,r4,r5
	addi	r4,r4,-1
	divw	r4,r4,r5	/* BUS ticks */
1:	mftbu	r5
	mftb	r6
	mftbu	r7
	cmpw	0,r5,r7
	bne	1b		/* Get [synced] base time */
	addc	r9,r6,r4	/* Compute end time */
	addze	r8,r5
2:	mftbu	r5
	cmpw	0,r5,r8
	blt	2b
	bgt	3f
	mftb	r6
	cmpw	0,r6,r9
	blt	2b
3:	blr
