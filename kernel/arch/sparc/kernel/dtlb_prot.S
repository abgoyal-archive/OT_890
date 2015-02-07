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
 * dtlb_prot.S: DTLB protection trap strategy.
 *              This is included directly into the trap table.
 *
 * Copyright (C) 1996,1998 David S. Miller (davem@redhat.com)
 * Copyright (C) 1997,1998 Jakub Jelinek   (jj@ultra.linux.cz)
 */

/* Ways we can get here:
 *
 * [TL == 0] 1) User stores to readonly pages.
 * [TL == 0] 2) Nucleus stores to user readonly pages.
 * [TL >  0] 3) Nucleus stores to user readonly stack frame.
 */

/* PROT ** ICACHE line 1: User DTLB protection trap	*/
	mov		TLB_SFSR, %g1
	stxa		%g0, [%g1] ASI_DMMU		! Clear FaultValid bit
	membar		#Sync				! Synchronize stores
	rdpr		%pstate, %g5			! Move into alt-globals
	wrpr		%g5, PSTATE_AG|PSTATE_MG, %pstate
	rdpr		%tl, %g1			! Need a winfixup?
	cmp		%g1, 1				! Trap level >1?
	mov		TLB_TAG_ACCESS, %g4		! For reload of vaddr

/* PROT ** ICACHE line 2: More real fault processing */
	bgu,pn		%xcc, winfix_trampoline		! Yes, perform winfixup
	 ldxa		[%g4] ASI_DMMU, %g5		! Put tagaccess in %g5
	ba,pt		%xcc, sparc64_realfault_common	! Nope, normal fault
	 mov		FAULT_CODE_DTLB | FAULT_CODE_WRITE, %g4
	nop
	nop
	nop
	nop

/* PROT ** ICACHE line 3: Unused...	*/
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop

/* PROT ** ICACHE line 4: Unused...	*/
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
