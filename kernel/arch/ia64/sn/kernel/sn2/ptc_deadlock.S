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
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2000-2005 Silicon Graphics, Inc. All rights reserved.
 */

#include <asm/types.h>
#include <asm/sn/shub_mmr.h>

#define DEADLOCKBIT	SH_PIO_WRITE_STATUS_WRITE_DEADLOCK_SHFT
#define WRITECOUNTMASK	SH_PIO_WRITE_STATUS_PENDING_WRITE_COUNT_MASK
#define ALIAS_OFFSET	8


	.global	sn2_ptc_deadlock_recovery_core
	.proc  	sn2_ptc_deadlock_recovery_core

sn2_ptc_deadlock_recovery_core:
	.regstk 6,0,0,0

	ptc0  	 = in0
	data0 	 = in1
	ptc1  	 = in2
	data1 	 = in3
	piowc 	 = in4
	zeroval  = in5
	piowcphy = r30
	psrsave  = r2
	scr1	 = r16
	scr2	 = r17
	mask	 = r18


	extr.u	piowcphy=piowc,0,61;;	// Convert piowc to uncached physical address
	dep	piowcphy=-1,piowcphy,63,1
	movl	mask=WRITECOUNTMASK
	mov	r8=r0

1:
	cmp.ne  p8,p9=r0,ptc1		// Test for shub type (ptc1 non-null on shub1)
					// p8 = 1 if shub1, p9 = 1 if shub2

	add	scr2=ALIAS_OFFSET,piowc	// Address of WRITE_STATUS alias register 
	mov	scr1=7;;		// Clear DEADLOCK, WRITE_ERROR, MULTI_WRITE_ERROR
(p8)	st8.rel	[scr2]=scr1;;
(p9)	ld8.acq	scr1=[scr2];;

5:	ld8.acq	scr1=[piowc];;		// Wait for PIOs to complete.
	hint	@pause
	and	scr2=scr1,mask;;	// mask of writecount bits
	cmp.ne	p6,p0=zeroval,scr2
(p6)	br.cond.sptk 5b
	


	////////////// BEGIN PHYSICAL MODE ////////////////////
	mov psrsave=psr			// Disable IC (no PMIs)
	rsm psr.i | psr.dt | psr.ic;;
	srlz.i;;

	st8.rel [ptc0]=data0		// Write PTC0 & wait for completion.

5:	ld8.acq	scr1=[piowcphy];;	// Wait for PIOs to complete.
	hint	@pause
	and	scr2=scr1,mask;;	// mask of writecount bits
	cmp.ne	p6,p0=zeroval,scr2
(p6)	br.cond.sptk 5b;;

	tbit.nz	p8,p7=scr1,DEADLOCKBIT;;// Test for DEADLOCK
(p7)	cmp.ne p7,p0=r0,ptc1;;		// Test for non-null ptc1
	
(p7)	st8.rel [ptc1]=data1;;		// Now write PTC1.

5:	ld8.acq	scr1=[piowcphy];;	// Wait for PIOs to complete.
	hint	@pause
	and	scr2=scr1,mask;;	// mask of writecount bits
	cmp.ne	p6,p0=zeroval,scr2
(p6)	br.cond.sptk 5b
	
	tbit.nz	p8,p0=scr1,DEADLOCKBIT;;// Test for DEADLOCK

	mov psr.l=psrsave;;		// Reenable IC
	srlz.i;;
	////////////// END   PHYSICAL MODE ////////////////////

(p8)	add	r8=1,r8
(p8)	br.cond.spnt 1b;;		// Repeat if DEADLOCK occurred.

	br.ret.sptk	rp
	.endp sn2_ptc_deadlock_recovery_core
