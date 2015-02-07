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
 *  arch/s390/kernel/reipl.S
 *
 *  S390 version
 *    Copyright (C) 2000 IBM Deutschland Entwicklung GmbH, IBM Corporation
 *    Author(s): Holger Smolinski (Holger.Smolinski@de.ibm.com)
		 Denis Joseph Barrow (djbarrow@de.ibm.com,barrow_dj@yahoo.com)
 */

#include <asm/lowcore.h>

#
# do_reipl_asm
# Parameter: r2 = schid of reipl device
#

		.globl	do_reipl_asm
do_reipl_asm:	basr	%r13,0
.Lpg0:		lpswe	.Lnewpsw-.Lpg0(%r13)
.Lpg1:		# do store status of all registers

		stg	%r1,.Lregsave-.Lpg0(%r13)
		lghi	%r1,0x1000
		stmg	%r0,%r15,__LC_GPREGS_SAVE_AREA-0x1000(%r1)
		lg	%r0,.Lregsave-.Lpg0(%r13)
		stg	%r0,__LC_GPREGS_SAVE_AREA-0x1000+8(%r1)
		stctg	%c0,%c15,__LC_CREGS_SAVE_AREA-0x1000(%r1)
		stam	%a0,%a15,__LC_AREGS_SAVE_AREA-0x1000(%r1)
		lg	%r10,.Ldump_pfx-.Lpg0(%r13)
		mvc	__LC_PREFIX_SAVE_AREA-0x1000(4,%r1),0(%r10)
		stfpc	__LC_FP_CREG_SAVE_AREA-0x1000(%r1)
		stckc	.Lclkcmp-.Lpg0(%r13)
		mvc	__LC_CLOCK_COMP_SAVE_AREA-0x1000(8,%r1),.Lclkcmp-.Lpg0(%r13)
		stpt	__LC_CPU_TIMER_SAVE_AREA-0x1000(%r1)
		stg	%r13, __LC_PSW_SAVE_AREA-0x1000+8(%r1)

		lctlg	%c6,%c6,.Lall-.Lpg0(%r13)
		lgr	%r1,%r2
		mvc	__LC_PGM_NEW_PSW(16),.Lpcnew-.Lpg0(%r13)
		stsch	.Lschib-.Lpg0(%r13)
		oi	.Lschib+5-.Lpg0(%r13),0x84
.Lecs:  	xi	.Lschib+27-.Lpg0(%r13),0x01
		msch	.Lschib-.Lpg0(%r13)
		lghi	%r0,5
.Lssch:		ssch	.Liplorb-.Lpg0(%r13)
		jz	.L001
		brct	%r0,.Lssch
		bas	%r14,.Ldisab-.Lpg0(%r13)
.L001:		mvc	__LC_IO_NEW_PSW(16),.Lionew-.Lpg0(%r13)
.Ltpi:		lpswe	.Lwaitpsw-.Lpg0(%r13)
.Lcont:		c	%r1,__LC_SUBCHANNEL_ID
		jnz	.Ltpi
		clc	__LC_IO_INT_PARM(4),.Liplorb-.Lpg0(%r13)
		jnz	.Ltpi
		tsch	.Liplirb-.Lpg0(%r13)
		tm	.Liplirb+9-.Lpg0(%r13),0xbf
		jz	.L002
		bas	%r14,.Ldisab-.Lpg0(%r13)
.L002:		tm	.Liplirb+8-.Lpg0(%r13),0xf3
		jz	.L003
		bas	%r14,.Ldisab-.Lpg0(%r13)
.L003:		st	%r1,__LC_SUBCHANNEL_ID
		lhi	%r1,0		 # mode 0 = esa
		slr	%r0,%r0 	 # set cpuid to zero
		sigp	%r1,%r0,0x12	 # switch to esa mode
		lpsw	0
.Ldisab:	sll	%r14,1
		srl	%r14,1		 # need to kill hi bit to avoid specification exceptions.
		st	%r14,.Ldispsw+12-.Lpg0(%r13)
		lpswe	.Ldispsw-.Lpg0(%r13)
		.align	8
.Lclkcmp:	.quad	0x0000000000000000
.Lall:		.quad	0x00000000ff000000
.Ldump_pfx:	.quad	dump_prefix_page
.Lregsave:	.quad	0x0000000000000000
		.align	16
/*
 * These addresses have to be 31 bit otherwise
 * the sigp will throw a specifcation exception
 * when switching to ESA mode as bit 31 be set
 * in the ESA psw.
 * Bit 31 of the addresses has to be 0 for the
 * 31bit lpswe instruction a fact they appear to have
 * ommited from the pop.
 */
.Lnewpsw:	.quad	0x0000000080000000
		.quad	.Lpg1
.Lpcnew:	.quad	0x0000000080000000
		.quad	.Lecs
.Lionew:	.quad	0x0000000080000000
		.quad	.Lcont
.Lwaitpsw:	.quad	0x0202000080000000
		.quad	.Ltpi
.Ldispsw:	.quad	0x0002000080000000
		.quad	0x0000000000000000
.Liplccws:	.long	0x02000000,0x60000018
		.long	0x08000008,0x20000001
.Liplorb:	.long	0x0049504c,0x0040ff80
		.long	0x00000000+.Liplccws
.Lschib:	.long	0x00000000,0x00000000
		.long	0x00000000,0x00000000
		.long	0x00000000,0x00000000
		.long	0x00000000,0x00000000
		.long	0x00000000,0x00000000
		.long	0x00000000,0x00000000
.Liplirb:	.long	0x00000000,0x00000000
		.long	0x00000000,0x00000000
		.long	0x00000000,0x00000000
		.long	0x00000000,0x00000000
		.long	0x00000000,0x00000000
		.long	0x00000000,0x00000000
		.long	0x00000000,0x00000000
		.long	0x00000000,0x00000000
