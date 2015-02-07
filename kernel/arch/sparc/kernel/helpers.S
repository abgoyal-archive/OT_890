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

	.align	32
	.globl	__flushw_user
	.type	__flushw_user,#function
__flushw_user:
	rdpr	%otherwin, %g1
	brz,pn	%g1, 2f
	 clr	%g2
1:	save	%sp, -128, %sp
	rdpr	%otherwin, %g1
	brnz,pt	%g1, 1b
	 add	%g2, 1, %g2
1:	sub	%g2, 1, %g2
	brnz,pt	%g2, 1b
	 restore %g0, %g0, %g0
2:	retl
	 nop
	.size	__flushw_user,.-__flushw_user

	/* Flush %fp and %i7 to the stack for all register
	 * windows active inside of the cpu.  This allows
	 * show_stack_trace() to avoid using an expensive
	 * 'flushw'.
	 */
	.globl		stack_trace_flush
	.type		stack_trace_flush,#function
stack_trace_flush:
	rdpr		%pstate, %o0
	wrpr		%o0, PSTATE_IE, %pstate

	rdpr		%cwp, %g1
	rdpr		%canrestore, %g2
	sub		%g1, 1, %g3

1:	brz,pn		%g2, 2f
	 sub		%g2, 1, %g2
	wrpr		%g3, %cwp
	stx		%fp, [%sp + STACK_BIAS + RW_V9_I6]
	stx		%i7, [%sp + STACK_BIAS + RW_V9_I7]
	ba,pt		%xcc, 1b
	 sub		%g3, 1, %g3

2:	wrpr		%g1, %cwp
	wrpr		%o0, %pstate

	retl
	 nop
	.size		stack_trace_flush,.-stack_trace_flush

#ifdef CONFIG_SMP
	.globl		hard_smp_processor_id
	.type		hard_smp_processor_id,#function
hard_smp_processor_id:
#endif
	.globl		real_hard_smp_processor_id
	.type		real_hard_smp_processor_id,#function
real_hard_smp_processor_id:
	__GET_CPUID(%o0)
	retl
	 nop
#ifdef CONFIG_SMP
	.size		hard_smp_processor_id,.-hard_smp_processor_id
#endif
	.size		real_hard_smp_processor_id,.-real_hard_smp_processor_id
