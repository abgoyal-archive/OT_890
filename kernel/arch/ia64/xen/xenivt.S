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
 * arch/ia64/xen/ivt.S
 *
 * Copyright (C) 2005 Hewlett-Packard Co
 *	Dan Magenheimer <dan.magenheimer@hp.com>
 *
 * Copyright (c) 2008 Isaku Yamahata <yamahata at valinux co jp>
 *                    VA Linux Systems Japan K.K.
 *                    pv_ops.
 */

#include <asm/asmmacro.h>
#include <asm/kregs.h>
#include <asm/pgtable.h>

#include "../kernel/minstate.h"

	.section .text,"ax"
GLOBAL_ENTRY(xen_event_callback)
	mov r31=pr		// prepare to save predicates
	;;
	SAVE_MIN_WITH_COVER	// uses r31; defines r2 and r3
	;;
	movl r3=XSI_PSR_IC
	mov r14=1
	;;
	st4 [r3]=r14
	;;
	adds r3=8,r2		// set up second base pointer for SAVE_REST
	srlz.i			// ensure everybody knows psr.ic is back on
	;;
	SAVE_REST
	;;
1:
	alloc r14=ar.pfs,0,0,1,0 // must be first in an insn group
	add out0=16,sp		// pass pointer to pt_regs as first arg
	;;
	br.call.sptk.many b0=xen_evtchn_do_upcall
	;;
	movl r20=XSI_PSR_I_ADDR
	;;
	ld8 r20=[r20]
	;;
	adds r20=-1,r20		// vcpu_info->evtchn_upcall_pending
	;;
	ld1 r20=[r20]
	;;
	cmp.ne p6,p0=r20,r0	// if there are pending events,
	(p6) br.spnt.few 1b	// call evtchn_do_upcall again.
	br.sptk.many xen_leave_kernel	// we know ia64_leave_kernel is
					// paravirtualized as xen_leave_kernel
END(xen_event_callback)
