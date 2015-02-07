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

/* atomic.S: Move this stuff here for better ICACHE hit rates.
 *
 * Copyright (C) 1996 David S. Miller (davem@caipfs.rutgers.edu)
 */

#include <asm/ptrace.h>
#include <asm/psr.h>

	.text
	.align	4

	.globl  __atomic_begin
__atomic_begin:

#ifndef CONFIG_SMP
	.globl	___xchg32_sun4c
___xchg32_sun4c:
	rd	%psr, %g3
	andcc	%g3, PSR_PIL, %g0
	bne	1f
	 nop
	wr	%g3, PSR_PIL, %psr
	nop; nop; nop
1:
	andcc	%g3, PSR_PIL, %g0
	ld	[%g1], %g7
	bne	1f
	 st	%g2, [%g1]
	wr	%g3, 0x0, %psr
	nop; nop; nop
1:
	mov	%g7, %g2
	jmpl	%o7 + 8, %g0
	 mov	%g4, %o7

	.globl	___xchg32_sun4md
___xchg32_sun4md:
	swap	[%g1], %g2
	jmpl	%o7 + 8, %g0
	 mov	%g4, %o7
#endif

	/* Read asm-sparc/atomic.h carefully to understand how this works for SMP.
	 * Really, some things here for SMP are overly clever, go read the header.
	 */
	.globl	___atomic24_add
___atomic24_add:
	rd	%psr, %g3		! Keep the code small, old way was stupid
	nop; nop; nop;			! Let the bits set
	or	%g3, PSR_PIL, %g7	! Disable interrupts
	wr	%g7, 0x0, %psr		! Set %psr
	nop; nop; nop;			! Let the bits set
#ifdef CONFIG_SMP
1:	ldstub	[%g1 + 3], %g7		! Spin on the byte lock for SMP.
	orcc	%g7, 0x0, %g0		! Did we get it?
	bne	1b			! Nope...
	 ld	[%g1], %g7		! Load locked atomic24_t
	sra	%g7, 8, %g7		! Get signed 24-bit integer
	add	%g7, %g2, %g2		! Add in argument
	sll	%g2, 8, %g7		! Transpose back to atomic24_t
	st	%g7, [%g1]		! Clever: This releases the lock as well.
#else
	ld	[%g1], %g7		! Load locked atomic24_t
	add	%g7, %g2, %g2		! Add in argument
	st	%g2, [%g1]		! Store it back
#endif
	wr	%g3, 0x0, %psr		! Restore original PSR_PIL
	nop; nop; nop;			! Let the bits set
	jmpl	%o7, %g0		! NOTE: not + 8, see callers in atomic.h
	 mov	%g4, %o7		! Restore %o7

	.globl	___atomic24_sub
___atomic24_sub:
	rd	%psr, %g3		! Keep the code small, old way was stupid
	nop; nop; nop;			! Let the bits set
	or	%g3, PSR_PIL, %g7	! Disable interrupts
	wr	%g7, 0x0, %psr		! Set %psr
	nop; nop; nop;			! Let the bits set
#ifdef CONFIG_SMP
1:	ldstub	[%g1 + 3], %g7		! Spin on the byte lock for SMP.
	orcc	%g7, 0x0, %g0		! Did we get it?
	bne	1b			! Nope...
	 ld	[%g1], %g7		! Load locked atomic24_t
	sra	%g7, 8, %g7		! Get signed 24-bit integer
	sub	%g7, %g2, %g2		! Subtract argument
	sll	%g2, 8, %g7		! Transpose back to atomic24_t
	st	%g7, [%g1]		! Clever: This releases the lock as well
#else
	ld	[%g1], %g7		! Load locked atomic24_t
	sub	%g7, %g2, %g2		! Subtract argument
	st	%g2, [%g1]		! Store it back
#endif
	wr	%g3, 0x0, %psr		! Restore original PSR_PIL
	nop; nop; nop;			! Let the bits set
	jmpl	%o7, %g0		! NOTE: not + 8, see callers in atomic.h
	 mov	%g4, %o7		! Restore %o7

	.globl  __atomic_end
__atomic_end:
