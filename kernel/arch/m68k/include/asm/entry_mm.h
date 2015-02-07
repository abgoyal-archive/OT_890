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

#ifndef __M68K_ENTRY_H
#define __M68K_ENTRY_H

#include <asm/setup.h>
#include <asm/page.h>

/*
 * Stack layout in 'ret_from_exception':
 *
 *	This allows access to the syscall arguments in registers d1-d5
 *
 *	 0(sp) - d1
 *	 4(sp) - d2
 *	 8(sp) - d3
 *	 C(sp) - d4
 *	10(sp) - d5
 *	14(sp) - a0
 *	18(sp) - a1
 *	1C(sp) - a2
 *	20(sp) - d0
 *	24(sp) - orig_d0
 *	28(sp) - stack adjustment
 *	2C(sp) - sr
 *	2E(sp) - pc
 *	32(sp) - format & vector
 */

/*
 * 97/05/14 Andreas: Register %a2 is now set to the current task throughout
 *		     the whole kernel.
 */

/* the following macro is used when enabling interrupts */
#if defined(MACH_ATARI_ONLY)
	/* block out HSYNC on the atari */
#define ALLOWINT	(~0x400)
#define	MAX_NOINT_IPL	3
#else
	/* portable version */
#define ALLOWINT	(~0x700)
#define	MAX_NOINT_IPL	0
#endif /* machine compilation types */

#ifdef __ASSEMBLY__

#define curptr a2

LFLUSH_I_AND_D = 0x00000808
LSIGTRAP = 5

/* process bits for task_struct.ptrace */
PT_TRACESYS_OFF = 3
PT_TRACESYS_BIT = 1
PT_PTRACED_OFF = 3
PT_PTRACED_BIT = 0
PT_DTRACE_OFF = 3
PT_DTRACE_BIT = 2

#define SAVE_ALL_INT save_all_int
#define SAVE_ALL_SYS save_all_sys
#define RESTORE_ALL restore_all
/*
 * This defines the normal kernel pt-regs layout.
 *
 * regs a3-a6 and d6-d7 are preserved by C code
 * the kernel doesn't mess with usp unless it needs to
 */

/*
 * a -1 in the orig_d0 field signifies
 * that the stack frame is NOT for syscall
 */
.macro	save_all_int
	clrl	%sp@-		| stk_adj
	pea	-1:w		| orig d0
	movel	%d0,%sp@-	| d0
	moveml	%d1-%d5/%a0-%a1/%curptr,%sp@-
.endm

.macro	save_all_sys
	clrl	%sp@-		| stk_adj
	movel	%d0,%sp@-	| orig d0
	movel	%d0,%sp@-	| d0
	moveml	%d1-%d5/%a0-%a1/%curptr,%sp@-
.endm

.macro	restore_all
	moveml	%sp@+,%a0-%a1/%curptr/%d1-%d5
	movel	%sp@+,%d0
	addql	#4,%sp		| orig d0
	addl	%sp@+,%sp	| stk adj
	rte
.endm

#define SWITCH_STACK_SIZE (6*4+4)	/* includes return address */

#define SAVE_SWITCH_STACK save_switch_stack
#define RESTORE_SWITCH_STACK restore_switch_stack
#define GET_CURRENT(tmp) get_current tmp

.macro	save_switch_stack
	moveml	%a3-%a6/%d6-%d7,%sp@-
.endm

.macro	restore_switch_stack
	moveml	%sp@+,%a3-%a6/%d6-%d7
.endm

.macro	get_current reg=%d0
	movel	%sp,\reg
	andw	#-THREAD_SIZE,\reg
	movel	\reg,%curptr
	movel	%curptr@,%curptr
.endm

#else /* C source */

#define STR(X) STR1(X)
#define STR1(X) #X

#define PT_OFF_ORIG_D0	 0x24
#define PT_OFF_FORMATVEC 0x32
#define PT_OFF_SR	 0x2C
#define SAVE_ALL_INT				\
	"clrl	%%sp@-;"    /* stk_adj */	\
	"pea	-1:w;"	    /* orig d0 = -1 */	\
	"movel	%%d0,%%sp@-;" /* d0 */		\
	"moveml	%%d1-%%d5/%%a0-%%a2,%%sp@-"
#define GET_CURRENT(tmp) \
	"movel	%%sp,"#tmp"\n\t" \
	"andw	#-"STR(THREAD_SIZE)","#tmp"\n\t" \
	"movel	"#tmp",%%a2\n\t" \
	"movel	%%a2@,%%a2"

#endif

#endif /* __M68K_ENTRY_H */
