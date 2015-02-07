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

/* rwsem.S: RW semaphore assembler.
 *
 * Written by David S. Miller (davem@redhat.com), 2001.
 * Derived from asm-i386/rwsem.h
 */

#include <asm/rwsem-const.h>

	.section	.sched.text, "ax"

	.globl		__down_read
__down_read:
1:	lduw		[%o0], %g1
	add		%g1, 1, %g7
	cas		[%o0], %g1, %g7
	cmp		%g1, %g7
	bne,pn		%icc, 1b
	 add		%g7, 1, %g7
	cmp		%g7, 0
	bl,pn		%icc, 3f
	 nop
2:
	retl
	 nop
3:
	save		%sp, -192, %sp
	call		rwsem_down_read_failed
	 mov		%i0, %o0
	ret
	 restore
	.size		__down_read, .-__down_read

	.globl		__down_read_trylock
__down_read_trylock:
1:	lduw		[%o0], %g1
	add		%g1, 1, %g7
	cmp		%g7, 0
	bl,pn		%icc, 2f
	 mov		0, %o1
	cas		[%o0], %g1, %g7
	cmp		%g1, %g7
	bne,pn		%icc, 1b
	 mov		1, %o1
2:	retl
	 mov		%o1, %o0
	.size		__down_read_trylock, .-__down_read_trylock

	.globl		__down_write
__down_write:
	sethi		%hi(RWSEM_ACTIVE_WRITE_BIAS), %g1
	or		%g1, %lo(RWSEM_ACTIVE_WRITE_BIAS), %g1
1:
	lduw		[%o0], %g3
	add		%g3, %g1, %g7
	cas		[%o0], %g3, %g7
	cmp		%g3, %g7
	bne,pn		%icc, 1b
	 cmp		%g7, 0
	bne,pn		%icc, 3f
	 nop
2:	retl
	 nop
3:
	save		%sp, -192, %sp
	call		rwsem_down_write_failed
	 mov		%i0, %o0
	ret
	 restore
	.size		__down_write, .-__down_write

	.globl		__down_write_trylock
__down_write_trylock:
	sethi		%hi(RWSEM_ACTIVE_WRITE_BIAS), %g1
	or		%g1, %lo(RWSEM_ACTIVE_WRITE_BIAS), %g1
1:
	lduw		[%o0], %g3
	cmp		%g3, 0
	bne,pn		%icc, 2f
	 mov		0, %o1
	add		%g3, %g1, %g7
	cas		[%o0], %g3, %g7
	cmp		%g3, %g7
	bne,pn		%icc, 1b
	 mov		1, %o1
2:	retl
	 mov		%o1, %o0
	.size		__down_write_trylock, .-__down_write_trylock

	.globl		__up_read
__up_read:
1:
	lduw		[%o0], %g1
	sub		%g1, 1, %g7
	cas		[%o0], %g1, %g7
	cmp		%g1, %g7
	bne,pn		%icc, 1b
	 cmp		%g7, 0
	bl,pn		%icc, 3f
	 nop
2:	retl
	 nop
3:	sethi		%hi(RWSEM_ACTIVE_MASK), %g1
	sub		%g7, 1, %g7
	or		%g1, %lo(RWSEM_ACTIVE_MASK), %g1
	andcc		%g7, %g1, %g0
	bne,pn		%icc, 2b
	 nop
	save		%sp, -192, %sp
	call		rwsem_wake
	 mov		%i0, %o0
	ret
	 restore
	.size		__up_read, .-__up_read

	.globl		__up_write
__up_write:
	sethi		%hi(RWSEM_ACTIVE_WRITE_BIAS), %g1
	or		%g1, %lo(RWSEM_ACTIVE_WRITE_BIAS), %g1
1:
	lduw		[%o0], %g3
	sub		%g3, %g1, %g7
	cas		[%o0], %g3, %g7
	cmp		%g3, %g7
	bne,pn		%icc, 1b
	 sub		%g7, %g1, %g7
	cmp		%g7, 0
	bl,pn		%icc, 3f
	 nop
2:
	retl
	 nop
3:
	save		%sp, -192, %sp
	call		rwsem_wake
	 mov		%i0, %o0
	ret
	 restore
	.size		__up_write, .-__up_write

	.globl		__downgrade_write
__downgrade_write:
	sethi		%hi(RWSEM_WAITING_BIAS), %g1
	or		%g1, %lo(RWSEM_WAITING_BIAS), %g1
1:
	lduw		[%o0], %g3
	sub		%g3, %g1, %g7
	cas		[%o0], %g3, %g7
	cmp		%g3, %g7
	bne,pn		%icc, 1b
	 sub		%g7, %g1, %g7
	cmp		%g7, 0
	bl,pn		%icc, 3f
	 nop
2:
	retl
	 nop
3:
	save		%sp, -192, %sp
	call		rwsem_downgrade_wake
	 mov		%i0, %o0
	ret
	 restore
	.size		__downgrade_write, .-__downgrade_write
