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

###############################################################################
#
# MN10300 Context switch operation
#
# Copyright (C) 2007 Red Hat, Inc. All Rights Reserved.
# Written by David Howells (dhowells@redhat.com)
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public Licence
# as published by the Free Software Foundation; either version
# 2 of the Licence, or (at your option) any later version.
#
###############################################################################
#include <linux/sys.h>
#include <linux/linkage.h>
#include <asm/thread_info.h>
#include <asm/cpu-regs.h>

	.text

###############################################################################
#
# struct task_struct *__switch_to(struct thread_struct *prev,
#				  struct thread_struct *next,
#				  struct task_struct *prev_task)
#
###############################################################################
ENTRY(__switch_to)
	movm	[d2,d3,a2,a3,exreg1],(sp)
	or	EPSW_NMID,epsw

	mov	(44,sp),d2

	mov	d0,a0
	mov	d1,a1

	# save prev context
	mov	(__frame),d0
	mov	d0,(THREAD_FRAME,a0)
	mov	__switch_back,d0
	mov	d0,(THREAD_PC,a0)
	mov	sp,a2
	mov	a2,(THREAD_SP,a0)
	mov	a3,(THREAD_A3,a0)

	mov	(THREAD_A3,a1),a3
	mov	(THREAD_SP,a1),a2

	# switch
	mov	a2,sp

	# load next context
	GET_THREAD_INFO a2
	mov	a2,(__current_ti)
	mov	(TI_task,a2),a2
	mov	a2,(__current)
#ifdef CONFIG_MN10300_CURRENT_IN_E2
	mov	a2,e2
#endif

	mov	(THREAD_FRAME,a1),a2
	mov	a2,(__frame)
	mov	(THREAD_PC,a1),a2
	mov	d2,d0			# for ret_from_fork
	mov	d0,a0			# for __switch_to

	jmp	(a2)

__switch_back:
	and	~EPSW_NMID,epsw
	ret	[d2,d3,a2,a3,exreg1],32
