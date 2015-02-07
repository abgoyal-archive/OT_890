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

/* MN10300 Low level FPU management operations
 *
 * Copyright (C) 2007 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version
 * 2 of the Licence, or (at your option) any later version.
 */
#include <asm/cpu-regs.h>

###############################################################################
#
# void fpu_init_state(void)
# - initialise the FPU
#
###############################################################################
	.globl	fpu_init_state
	.type	fpu_init_state,@function
fpu_init_state:
	mov	epsw,d0
	or	EPSW_FE,epsw

#ifdef CONFIG_MN10300_PROC_MN103E010
	nop
	nop
	nop
#endif
	fmov	0,fs0
	fmov	fs0,fs1
	fmov	fs0,fs2
	fmov	fs0,fs3
	fmov	fs0,fs4
	fmov	fs0,fs5
	fmov	fs0,fs6
	fmov	fs0,fs7
	fmov	fs0,fs8
	fmov	fs0,fs9
	fmov	fs0,fs10
	fmov	fs0,fs11
	fmov	fs0,fs12
	fmov	fs0,fs13
	fmov	fs0,fs14
	fmov	fs0,fs15
	fmov	fs0,fs16
	fmov	fs0,fs17
	fmov	fs0,fs18
	fmov	fs0,fs19
	fmov	fs0,fs20
	fmov	fs0,fs21
	fmov	fs0,fs22
	fmov	fs0,fs23
	fmov	fs0,fs24
	fmov	fs0,fs25
	fmov	fs0,fs26
	fmov	fs0,fs27
	fmov	fs0,fs28
	fmov	fs0,fs29
	fmov	fs0,fs30
	fmov	fs0,fs31
	fmov	FPCR_INIT,fpcr

#ifdef CONFIG_MN10300_PROC_MN103E010
	nop
	nop
	nop
#endif
	mov	d0,epsw
	ret	[],0

	.size	fpu_init_state,.-fpu_init_state

###############################################################################
#
# void fpu_save(struct fpu_state_struct *)
# - save the fpu state
# - note that an FPU Operational exception might occur during this process
#
###############################################################################
	.globl	fpu_save
	.type	fpu_save,@function
fpu_save:
	mov	epsw,d1
	or	EPSW_FE,epsw		/* enable the FPU so we can access it */

#ifdef CONFIG_MN10300_PROC_MN103E010
	nop
	nop
#endif
	mov	d0,a0
	fmov	fs0,(a0+)
	fmov	fs1,(a0+)
	fmov	fs2,(a0+)
	fmov	fs3,(a0+)
	fmov	fs4,(a0+)
	fmov	fs5,(a0+)
	fmov	fs6,(a0+)
	fmov	fs7,(a0+)
	fmov	fs8,(a0+)
	fmov	fs9,(a0+)
	fmov	fs10,(a0+)
	fmov	fs11,(a0+)
	fmov	fs12,(a0+)
	fmov	fs13,(a0+)
	fmov	fs14,(a0+)
	fmov	fs15,(a0+)
	fmov	fs16,(a0+)
	fmov	fs17,(a0+)
	fmov	fs18,(a0+)
	fmov	fs19,(a0+)
	fmov	fs20,(a0+)
	fmov	fs21,(a0+)
	fmov	fs22,(a0+)
	fmov	fs23,(a0+)
	fmov	fs24,(a0+)
	fmov	fs25,(a0+)
	fmov	fs26,(a0+)
	fmov	fs27,(a0+)
	fmov	fs28,(a0+)
	fmov	fs29,(a0+)
	fmov	fs30,(a0+)
	fmov	fs31,(a0+)
	fmov	fpcr,d0
	mov	d0,(a0)
#ifdef CONFIG_MN10300_PROC_MN103E010
	nop
	nop
#endif

	mov	d1,epsw
	ret	[],0

	.size	fpu_save,.-fpu_save

###############################################################################
#
# void fpu_restore(struct fpu_state_struct *)
# - restore the fpu state
# - note that an FPU Operational exception might occur during this process
#
###############################################################################
	.globl	fpu_restore
	.type	fpu_restore,@function
fpu_restore:
	mov	epsw,d1
	or	EPSW_FE,epsw		/* enable the FPU so we can access it */

#ifdef CONFIG_MN10300_PROC_MN103E010
	nop
	nop
#endif
	mov	d0,a0
	fmov	(a0+),fs0
	fmov	(a0+),fs1
	fmov	(a0+),fs2
	fmov	(a0+),fs3
	fmov	(a0+),fs4
	fmov	(a0+),fs5
	fmov	(a0+),fs6
	fmov	(a0+),fs7
	fmov	(a0+),fs8
	fmov	(a0+),fs9
	fmov	(a0+),fs10
	fmov	(a0+),fs11
	fmov	(a0+),fs12
	fmov	(a0+),fs13
	fmov	(a0+),fs14
	fmov	(a0+),fs15
	fmov	(a0+),fs16
	fmov	(a0+),fs17
	fmov	(a0+),fs18
	fmov	(a0+),fs19
	fmov	(a0+),fs20
	fmov	(a0+),fs21
	fmov	(a0+),fs22
	fmov	(a0+),fs23
	fmov	(a0+),fs24
	fmov	(a0+),fs25
	fmov	(a0+),fs26
	fmov	(a0+),fs27
	fmov	(a0+),fs28
	fmov	(a0+),fs29
	fmov	(a0+),fs30
	fmov	(a0+),fs31
	mov	(a0),d0
	fmov	d0,fpcr
#ifdef CONFIG_MN10300_PROC_MN103E010
	nop
	nop
	nop
#endif

	mov	d1,epsw
	ret	[],0

	.size	fpu_restore,.-fpu_restore
