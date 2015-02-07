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
 * This file contains sequences of code that will be copied to a
 * fixed location, defined in <asm/fixed_code.h>.  The interrupt
 * handlers ensure that these sequences appear to be atomic when
 * executed from userspace.
 * These are aligned to 16 bytes, so that we have some space to replace
 * these sequences with something else (e.g. kernel traps if we ever do
 * BF561 SMP).
 */
#include <linux/linkage.h>
#include <linux/init.h>
#include <linux/unistd.h>
#include <asm/entry.h>

__INIT

ENTRY(_fixed_code_start)

.align 16
ENTRY(_sigreturn_stub)
	P0 = __NR_rt_sigreturn;
	EXCPT 0;
	/* Speculative execution paranoia.  */
0:	JUMP.S 0b;
ENDPROC (_sigreturn_stub)

.align 16
	/*
	 * Atomic swap, 8 bit.
	 * Inputs:	P0: memory address to use
	 *		R1: value to store
	 * Output:	R0: old contents of the memory address, zero extended.
	 */
ENTRY(_atomic_xchg32)
	R0 = [P0];
	[P0] = R1;
	rts;
ENDPROC (_atomic_xchg32)

.align 16
	/*
	 * Compare and swap, 32 bit.
	 * Inputs:	P0: memory address to use
	 *		R1: compare value
	 *		R2: new value to store
	 * The new value is stored if the contents of the memory
	 * address is equal to the compare value.
	 * Output:	R0: old contents of the memory address.
	 */
ENTRY(_atomic_cas32)
	R0 = [P0];
	CC = R0 == R1;
	IF !CC JUMP 1f;
	[P0] = R2;
1:
	rts;
ENDPROC (_atomic_cas32)

.align 16
	/*
	 * Atomic add, 32 bit.
	 * Inputs:	P0: memory address to use
	 *		R0: value to add
	 * Outputs:	R0: new contents of the memory address.
	 *		R1: previous contents of the memory address.
	 */
ENTRY(_atomic_add32)
	R1 = [P0];
	R0 = R1 + R0;
	[P0] = R0;
	rts;
ENDPROC (_atomic_add32)

.align 16
	/*
	 * Atomic sub, 32 bit.
	 * Inputs:	P0: memory address to use
	 *		R0: value to subtract
	 * Outputs:	R0: new contents of the memory address.
	 *		R1: previous contents of the memory address.
	 */
ENTRY(_atomic_sub32)
	R1 = [P0];
	R0 = R1 - R0;
	[P0] = R0;
	rts;
ENDPROC (_atomic_sub32)

.align 16
	/*
	 * Atomic ior, 32 bit.
	 * Inputs:	P0: memory address to use
	 *		R0: value to ior
	 * Outputs:	R0: new contents of the memory address.
	 *		R1: previous contents of the memory address.
	 */
ENTRY(_atomic_ior32)
	R1 = [P0];
	R0 = R1 | R0;
	[P0] = R0;
	rts;
ENDPROC (_atomic_ior32)

.align 16
	/*
	 * Atomic and, 32 bit.
	 * Inputs:	P0: memory address to use
	 *		R0: value to and
	 * Outputs:	R0: new contents of the memory address.
	 *		R1: previous contents of the memory address.
	 */
ENTRY(_atomic_and32)
	R1 = [P0];
	R0 = R1 & R0;
	[P0] = R0;
	rts;
ENDPROC (_atomic_and32)

.align 16
	/*
	 * Atomic xor, 32 bit.
	 * Inputs:	P0: memory address to use
	 *		R0: value to xor
	 * Outputs:	R0: new contents of the memory address.
	 *		R1: previous contents of the memory address.
	 */
ENTRY(_atomic_xor32)
	R1 = [P0];
	R0 = R1 ^ R0;
	[P0] = R0;
	rts;
ENDPROC (_atomic_xor32)

.align 16
	/*
	 * safe_user_instruction
	 * Four NOPS are enough to allow the pipeline to speculativily load
	 * execute anything it wants. After that, things have gone bad, and
	 * we are stuck - so panic. Since we might be in user space, we can't
	 * call panic, so just cause a unhandled exception, this should cause
	 * a dump of the trace buffer so we can tell were we are, and a reboot
	 */
ENTRY(_safe_user_instruction)
	NOP; NOP; NOP; NOP;
	EXCPT 0x4;
ENDPROC(_safe_user_instruction)

ENTRY(_fixed_code_end)

__FINIT
