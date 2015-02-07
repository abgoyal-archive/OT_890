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
 * r6000_fpu.S: Save/restore floating point context for signal handlers.
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1996 by Ralf Baechle
 *
 * Multi-arch abstraction and asm macros for easier reading:
 * Copyright (C) 1996 David S. Miller (dm@engr.sgi.com)
 */
#include <asm/asm.h>
#include <asm/fpregdef.h>
#include <asm/mipsregs.h>
#include <asm/asm-offsets.h>
#include <asm/regdef.h>

	.set	noreorder
	.set	mips2
	/* Save floating point context */
	LEAF(_save_fp_context)
	mfc0	t0,CP0_STATUS
	sll	t0,t0,2
	bgez	t0,1f
	 nop

	cfc1	t1,fcr31
	/* Store the 16 double precision registers */
	sdc1	$f0,(SC_FPREGS+0)(a0)
	sdc1	$f2,(SC_FPREGS+16)(a0)
	sdc1	$f4,(SC_FPREGS+32)(a0)
	sdc1	$f6,(SC_FPREGS+48)(a0)
	sdc1	$f8,(SC_FPREGS+64)(a0)
	sdc1	$f10,(SC_FPREGS+80)(a0)
	sdc1	$f12,(SC_FPREGS+96)(a0)
	sdc1	$f14,(SC_FPREGS+112)(a0)
	sdc1	$f16,(SC_FPREGS+128)(a0)
	sdc1	$f18,(SC_FPREGS+144)(a0)
	sdc1	$f20,(SC_FPREGS+160)(a0)
	sdc1	$f22,(SC_FPREGS+176)(a0)
	sdc1	$f24,(SC_FPREGS+192)(a0)
	sdc1	$f26,(SC_FPREGS+208)(a0)
	sdc1	$f28,(SC_FPREGS+224)(a0)
	sdc1	$f30,(SC_FPREGS+240)(a0)
	jr	ra
	 sw	t0,SC_FPC_CSR(a0)
1:	jr	ra
	 nop
	END(_save_fp_context)

/* Restore FPU state:
 *  - fp gp registers
 *  - cp1 status/control register
 *
 * We base the decision which registers to restore from the signal stack
 * frame on the current content of c0_status, not on the content of the
 * stack frame which might have been changed by the user.
 */
	LEAF(_restore_fp_context)
	mfc0	t0,CP0_STATUS
	sll	t0,t0,2

	bgez	t0,1f
	 lw	t0,SC_FPC_CSR(a0)
	/* Restore the 16 double precision registers */
	ldc1	$f0,(SC_FPREGS+0)(a0)
	ldc1	$f2,(SC_FPREGS+16)(a0)
	ldc1	$f4,(SC_FPREGS+32)(a0)
	ldc1	$f6,(SC_FPREGS+48)(a0)
	ldc1	$f8,(SC_FPREGS+64)(a0)
	ldc1	$f10,(SC_FPREGS+80)(a0)
	ldc1	$f12,(SC_FPREGS+96)(a0)
	ldc1	$f14,(SC_FPREGS+112)(a0)
	ldc1	$f16,(SC_FPREGS+128)(a0)
	ldc1	$f18,(SC_FPREGS+144)(a0)
	ldc1	$f20,(SC_FPREGS+160)(a0)
	ldc1	$f22,(SC_FPREGS+176)(a0)
	ldc1	$f24,(SC_FPREGS+192)(a0)
	ldc1	$f26,(SC_FPREGS+208)(a0)
	ldc1	$f28,(SC_FPREGS+224)(a0)
	ldc1	$f30,(SC_FPREGS+240)(a0)
	jr	ra
	 ctc1	t0,fcr31
1:	jr	ra
	 nop
	END(_restore_fp_context)
