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
 * File:        mca_drv_asm.S
 * Purpose:     Assembly portion of Generic MCA handling
 *
 * Copyright (C) 2004 FUJITSU LIMITED
 * Copyright (C) 2004 Hidetoshi Seto <seto.hidetoshi@jp.fujitsu.com>
 */
#include <linux/threads.h>

#include <asm/asmmacro.h>
#include <asm/processor.h>
#include <asm/ptrace.h>

GLOBAL_ENTRY(mca_handler_bhhook)
	invala				// clear RSE ?
	cover
	;;
	clrrrb
	;;						
	alloc	r16=ar.pfs,0,2,3,0	// make a new frame
	mov	ar.rsc=0
	mov	r13=IA64_KR(CURRENT)	// current task pointer
	;;
	mov	r2=r13
	;;
	addl	r22=IA64_RBS_OFFSET,r2
	;;
	mov	ar.bspstore=r22
	addl	sp=IA64_STK_OFFSET-IA64_PT_REGS_SIZE,r2
	;;
	adds	r2=IA64_TASK_THREAD_ON_USTACK_OFFSET,r13
	;;
	st1	[r2]=r0		// clear current->thread.on_ustack flag
	mov	loc0=r16
	movl	loc1=mca_handler_bh	// recovery C function
	;;
	mov	out0=r8			// poisoned address
	mov	out1=r9			// iip
	mov	out2=r10		// psr
	mov	b6=loc1
	;;
	mov	loc1=rp
	ssm	psr.ic
	;;
	srlz.i
	;;
	ssm	psr.i
	br.call.sptk.many rp=b6		// does not return ...
	;;
	mov	ar.pfs=loc0
	mov 	rp=loc1
	;;
	mov	r8=r0
	br.ret.sptk.many rp
END(mca_handler_bhhook)
