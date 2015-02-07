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
 * Copyright (C) 1996 Paul Mackerras.
 *
 *      This program is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU General Public License
 *      as published by the Free Software Foundation; either version
 *      2 of the License, or (at your option) any later version.
 *
 * NOTE: assert(sizeof(buf) > 23 * sizeof(long))
 */
#include <asm/processor.h>
#include <asm/ppc_asm.h>
#include <asm/asm-offsets.h>

/*
 * Grab the register values as they are now.
 * This won't do a particularily good job because we really
 * want our caller's caller's registers, and our caller has
 * already executed its prologue.
 * ToDo: We could reach back into the caller's save area to do
 * a better job of representing the caller's state (note that
 * that will be different for 32-bit and 64-bit, because of the
 * different ABIs, though).
 */
_GLOBAL(ppc_save_regs)
	PPC_STL	r0,0*SZL(r3)
	PPC_STL	r2,2*SZL(r3)
	PPC_STL	r3,3*SZL(r3)
	PPC_STL	r4,4*SZL(r3)
	PPC_STL	r5,5*SZL(r3)
	PPC_STL	r6,6*SZL(r3)
	PPC_STL	r7,7*SZL(r3)
	PPC_STL	r8,8*SZL(r3)
	PPC_STL	r9,9*SZL(r3)
	PPC_STL	r10,10*SZL(r3)
	PPC_STL	r11,11*SZL(r3)
	PPC_STL	r12,12*SZL(r3)
	PPC_STL	r13,13*SZL(r3)
	PPC_STL	r14,14*SZL(r3)
	PPC_STL	r15,15*SZL(r3)
	PPC_STL	r16,16*SZL(r3)
	PPC_STL	r17,17*SZL(r3)
	PPC_STL	r18,18*SZL(r3)
	PPC_STL	r19,19*SZL(r3)
	PPC_STL	r20,20*SZL(r3)
	PPC_STL	r21,21*SZL(r3)
	PPC_STL	r22,22*SZL(r3)
	PPC_STL	r23,23*SZL(r3)
	PPC_STL	r24,24*SZL(r3)
	PPC_STL	r25,25*SZL(r3)
	PPC_STL	r26,26*SZL(r3)
	PPC_STL	r27,27*SZL(r3)
	PPC_STL	r28,28*SZL(r3)
	PPC_STL	r29,29*SZL(r3)
	PPC_STL	r30,30*SZL(r3)
	PPC_STL	r31,31*SZL(r3)
	/* go up one stack frame for SP */
	PPC_LL	r4,0(r1)
	PPC_STL	r4,1*SZL(r3)
	/* get caller's LR */
	PPC_LL	r0,LRSAVE(r4)
	PPC_STL	r0,_NIP-STACK_FRAME_OVERHEAD(r3)
	PPC_STL	r0,_LINK-STACK_FRAME_OVERHEAD(r3)
	mfmsr	r0
	PPC_STL	r0,_MSR-STACK_FRAME_OVERHEAD(r3)
	mfctr	r0
	PPC_STL	r0,_CTR-STACK_FRAME_OVERHEAD(r3)
	mfxer	r0
	PPC_STL	r0,_XER-STACK_FRAME_OVERHEAD(r3)
	mfcr	r0
	PPC_STL	r0,_CCR-STACK_FRAME_OVERHEAD(r3)
	li	r0,0
	PPC_STL	r0,_TRAP-STACK_FRAME_OVERHEAD(r3)
	blr
