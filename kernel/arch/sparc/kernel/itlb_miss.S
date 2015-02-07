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

/* ITLB ** ICACHE line 1: Context 0 check and TSB load	*/
	ldxa	[%g0] ASI_IMMU_TSB_8KB_PTR, %g1	! Get TSB 8K pointer
	ldxa	[%g0] ASI_IMMU, %g6		! Get TAG TARGET
	srlx	%g6, 48, %g5			! Get context
	sllx	%g6, 22, %g6			! Zero out context
	brz,pn	%g5, kvmap_itlb			! Context 0 processing
	 srlx	%g6, 22, %g6			! Delay slot
	TSB_LOAD_QUAD(%g1, %g4)			! Load TSB entry
	cmp	%g4, %g6			! Compare TAG

/* ITLB ** ICACHE line 2: TSB compare and TLB load	*/
	bne,pn	%xcc, tsb_miss_itlb		! Miss
	 mov	FAULT_CODE_ITLB, %g3
	sethi	%hi(_PAGE_EXEC_4U), %g4
	andcc	%g5, %g4, %g0			! Executable?
	be,pn	%xcc, tsb_do_fault
	 nop					! Delay slot, fill me
	stxa	%g5, [%g0] ASI_ITLB_DATA_IN	! Load TLB
	retry					! Trap done

/* ITLB ** ICACHE line 3: 				*/
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop

/* ITLB ** ICACHE line 4: 				*/
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
