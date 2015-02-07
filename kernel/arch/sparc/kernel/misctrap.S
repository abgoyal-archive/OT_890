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

#ifdef CONFIG_KGDB
	.globl		arch_kgdb_breakpoint
	.type		arch_kgdb_breakpoint,#function
arch_kgdb_breakpoint:
	ta		0x72
	retl
	 nop
	.size		arch_kgdb_breakpoint,.-arch_kgdb_breakpoint
#endif

	.type		__do_privact,#function
__do_privact:
	mov		TLB_SFSR, %g3
	stxa		%g0, [%g3] ASI_DMMU	! Clear FaultValid bit
	membar		#Sync
	sethi		%hi(109f), %g7
	ba,pt		%xcc, etrap
109:	or		%g7, %lo(109b), %g7
	call		do_privact
	 add		%sp, PTREGS_OFF, %o0
	ba,pt		%xcc, rtrap
	 nop
	.size		__do_privact,.-__do_privact

	.type		do_mna,#function
do_mna:
	rdpr		%tl, %g3
	cmp		%g3, 1

	/* Setup %g4/%g5 now as they are used in the
	 * winfixup code.
	 */
	mov		TLB_SFSR, %g3
	mov		DMMU_SFAR, %g4
	ldxa		[%g4] ASI_DMMU, %g4
	ldxa		[%g3] ASI_DMMU, %g5
	stxa		%g0, [%g3] ASI_DMMU	! Clear FaultValid bit
	membar		#Sync
	bgu,pn		%icc, winfix_mna
	 rdpr		%tpc, %g3

1:	sethi		%hi(109f), %g7
	ba,pt		%xcc, etrap
109:	 or		%g7, %lo(109b), %g7
	mov		%l4, %o1
	mov		%l5, %o2
	call		mem_address_unaligned
	 add		%sp, PTREGS_OFF, %o0
	ba,pt		%xcc, rtrap
	 nop
	.size		do_mna,.-do_mna

	.type		do_lddfmna,#function
do_lddfmna:
	sethi		%hi(109f), %g7
	mov		TLB_SFSR, %g4
	ldxa		[%g4] ASI_DMMU, %g5
	stxa		%g0, [%g4] ASI_DMMU	! Clear FaultValid bit
	membar		#Sync
	mov		DMMU_SFAR, %g4
	ldxa		[%g4] ASI_DMMU, %g4
	ba,pt		%xcc, etrap
109:	 or		%g7, %lo(109b), %g7
	mov		%l4, %o1
	mov		%l5, %o2
	call		handle_lddfmna
	 add		%sp, PTREGS_OFF, %o0
	ba,pt		%xcc, rtrap
	 nop
	.size		do_lddfmna,.-do_lddfmna

	.type		do_stdfmna,#function
do_stdfmna:
	sethi		%hi(109f), %g7
	mov		TLB_SFSR, %g4
	ldxa		[%g4] ASI_DMMU, %g5
	stxa		%g0, [%g4] ASI_DMMU	! Clear FaultValid bit
	membar		#Sync
	mov		DMMU_SFAR, %g4
	ldxa		[%g4] ASI_DMMU, %g4
	ba,pt		%xcc, etrap
109:	 or		%g7, %lo(109b), %g7
	mov		%l4, %o1
	mov		%l5, %o2
	call		handle_stdfmna
	 add		%sp, PTREGS_OFF, %o0
	ba,pt		%xcc, rtrap
	 nop
	.size		do_stdfmna,.-do_stdfmna

	.type		breakpoint_trap,#function
breakpoint_trap:
	call		sparc_breakpoint
	 add		%sp, PTREGS_OFF, %o0
	ba,pt		%xcc, rtrap
	 nop
	.size		breakpoint_trap,.-breakpoint_trap
