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
 *  This file contains the power_save function for 970-family CPUs.
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version
 *  2 of the License, or (at your option) any later version.
 */

#include <linux/threads.h>
#include <asm/processor.h>
#include <asm/page.h>
#include <asm/cputable.h>
#include <asm/thread_info.h>
#include <asm/ppc_asm.h>
#include <asm/asm-offsets.h>

#undef DEBUG

	.text

_GLOBAL(power4_idle)
BEGIN_FTR_SECTION
	blr
END_FTR_SECTION_IFCLR(CPU_FTR_CAN_NAP)
	/* Now check if user or arch enabled NAP mode */
	LOAD_REG_ADDRBASE(r3,powersave_nap)
	lwz	r4,ADDROFF(powersave_nap)(r3)
	cmpwi	0,r4,0
	beqlr

	/* Go to NAP now */
	mfmsr	r7
	rldicl	r0,r7,48,1
	rotldi	r0,r0,16
	mtmsrd	r0,1			/* hard-disable interrupts */
	li	r0,1
	stb	r0,PACASOFTIRQEN(r13)	/* we'll hard-enable shortly */
	stb	r0,PACAHARDIRQEN(r13)
BEGIN_FTR_SECTION
	DSSALL
	sync
END_FTR_SECTION_IFSET(CPU_FTR_ALTIVEC)
	clrrdi	r9,r1,THREAD_SHIFT	/* current thread_info */
	ld	r8,TI_LOCAL_FLAGS(r9)	/* set napping bit */
	ori	r8,r8,_TLF_NAPPING	/* so when we take an exception */
	std	r8,TI_LOCAL_FLAGS(r9)	/* it will return to our caller */
	ori	r7,r7,MSR_EE
	oris	r7,r7,MSR_POW@h
1:	sync
	isync
	mtmsrd	r7
	isync
	b	1b

_GLOBAL(power4_cpu_offline_powersave)
	/* Go to NAP now */
	mfmsr	r7
	rldicl	r0,r7,48,1
	rotldi	r0,r0,16
	mtmsrd	r0,1			/* hard-disable interrupts */
	li	r0,1
	li	r6,0
	stb	r0,PACAHARDIRQEN(r13)	/* we'll hard-enable shortly */
	stb	r6,PACASOFTIRQEN(r13)	/* soft-disable irqs */
BEGIN_FTR_SECTION
	DSSALL
	sync
END_FTR_SECTION_IFSET(CPU_FTR_ALTIVEC)
	ori	r7,r7,MSR_EE
	oris	r7,r7,MSR_POW@h
	sync
	isync
	mtmsrd	r7
	isync
	blr
