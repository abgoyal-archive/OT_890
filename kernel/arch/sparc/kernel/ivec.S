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

	/* The registers for cross calls will be:
	 *
	 * DATA 0: [low 32-bits]  Address of function to call, jmp to this
	 *         [high 32-bits] MMU Context Argument 0, place in %g5
	 * DATA 1: Address Argument 1, place in %g1
	 * DATA 2: Address Argument 2, place in %g7
	 *
	 * With this method we can do most of the cross-call tlb/cache
	 * flushing very quickly.
	 */
	.align		32
	.globl		do_ivec
	.type		do_ivec,#function
do_ivec:
	mov		0x40, %g3
	ldxa		[%g3 + %g0] ASI_INTR_R, %g3
	sethi		%hi(KERNBASE), %g4
	cmp		%g3, %g4
	bgeu,pn		%xcc, do_ivec_xcall
	 srlx		%g3, 32, %g5
	stxa		%g0, [%g0] ASI_INTR_RECEIVE
	membar		#Sync

	sethi		%hi(ivector_table_pa), %g2
	ldx		[%g2 + %lo(ivector_table_pa)], %g2
	sllx		%g3, 4, %g3
	add		%g2, %g3, %g3

	TRAP_LOAD_IRQ_WORK_PA(%g6, %g1)

	ldx		[%g6], %g5
	stxa		%g5, [%g3] ASI_PHYS_USE_EC
	stx		%g3, [%g6]
	wr		%g0, 1 << PIL_DEVICE_IRQ, %set_softint
	retry
do_ivec_xcall:
	mov		0x50, %g1
	ldxa		[%g1 + %g0] ASI_INTR_R, %g1
	srl		%g3, 0, %g3

	mov		0x60, %g7
	ldxa		[%g7 + %g0] ASI_INTR_R, %g7
	stxa		%g0, [%g0] ASI_INTR_RECEIVE
	membar		#Sync
	ba,pt		%xcc, 1f
	 nop

	.align		32
1:	jmpl		%g3, %g0
	 nop
	.size		do_ivec,.-do_ivec
