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

/* cif.S: PROM entry/exit assembler trampolines.
 *
 * Copyright (C) 1996, 1997 Jakub Jelinek (jj@sunsite.mff.cuni.cz)
 * Copyright (C) 2005, 2006 David S. Miller <davem@davemloft.net>
 */

#include <asm/pstate.h>
#include <asm/cpudata.h>
#include <asm/thread_info.h>

	.text
	.globl	prom_cif_interface
prom_cif_interface:
	sethi	%hi(p1275buf), %o0
	or	%o0, %lo(p1275buf), %o0
	ldx	[%o0 + 0x010], %o1	! prom_cif_stack
	save	%o1, -192, %sp
	ldx	[%i0 + 0x008], %l2	! prom_cif_handler
	mov	%g4, %l0
	mov	%g5, %l1
	mov	%g6, %l3
	call	%l2
	 add	%i0, 0x018, %o0		! prom_args
	mov	%l0, %g4
	mov	%l1, %g5
	mov	%l3, %g6
	ret
	 restore

	.globl	prom_cif_callback
prom_cif_callback:
	sethi	%hi(p1275buf), %o1
	or	%o1, %lo(p1275buf), %o1
	save	%sp, -192, %sp
	TRAP_LOAD_THREAD_REG(%g6, %g1)
	LOAD_PER_CPU_BASE(%g5, %g6, %g4, %g3, %o0)
	ldx	[%g6 + TI_TASK], %g4
	call	prom_world
	 mov	0, %o0
	ldx	[%i1 + 0x000], %l2
	call	%l2
	 mov	%i0, %o0
	mov	%o0, %l1
	call	prom_world
	 mov	1, %o0
	ret
	 restore %l1, 0, %o0

